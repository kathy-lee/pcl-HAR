//
// Created by kangleli on 1/19/20.
//
#include <iostream>
#include <string>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <mlpack/core.hpp>
#include <ensmallen.hpp>
#include <mlpack/methods/ann/layer/layer.hpp>
#include <mlpack/methods/ann/loss_functions/mean_squared_error.hpp>
#include <mlpack/methods/ann/rnn.hpp>
#include <mlpack/methods/ann/brnn.hpp>
#include <mlpack/core/data/binarize.hpp>
#include <mlpack/core/math/random.hpp>


using namespace std;
using namespace mlpack;
using namespace mlpack::ann;
using namespace ens;
using namespace mlpack::math;

template<class T>
void loadFeaturesFile(T &features, string &filename) {
    ifstream in(filename.c_str());
    boost::archive::binary_iarchive ia(in);
    ia >> features;
    in.close();
}

int main(int argc, char **argv) {

    string data_dir = "/home/kangleli/Projects/RadHAR_PCL/";
    vector<vector<vector<float>>> trainDataset, testDataset, samples;
    vector<int> trainLabels, testLabels;

    string fileName = data_dir + "train_feature_data.bin";
    loadFeaturesFile(trainDataset, fileName);
    fileName = data_dir + "test_feature_data.bin";
    loadFeaturesFile(testDataset, fileName );
    fileName = data_dir + "train_label.bin";
    loadFeaturesFile(trainLabels, fileName);
    fileName = data_dir + "test_label.bin";
    loadFeaturesFile(testLabels, fileName);
    cout << "finished loading data." << endl;

    arma::cube training_data(640,60,25);
    arma::cube test_data(640,60,25);
    arma::vec training_labels(25);
    arma::vec test_labels(25);

    for(unsigned i = 0; i < 25; i++ ){
        training_labels[i] = trainLabels[i];
        test_labels[i] = testLabels[i];
        for(unsigned j = 0; j < 60; j++ )
            for(unsigned k = 0; k < 640; k++){
                training_data.slice(i).row(k).col(j) = trainDataset[i][j][k];
                test_data.slice(i).row(k).col(j) = testDataset[i][j][k];
            }
    }

//    training_data.save(arma::hdf5_name("/home/kangleli/Projects/RadHAR_PCL/training_data001.h5", "train_dataset"));
//    arma::cube my_training;
//    my_training.load(arma::hdf5_name("/home/kangleli/Projects/RadHAR_PCL/training_data001.h5", "train_dataset"));
//
//    test_data.save(arma::hdf5_name("/home/kangleli/Projects/RadHAR_PCL/test_data001.h5", "test_dataset"));
//    arma::cube my_test;
//    my_test.load(arma::hdf5_name("/home/kangleli/Projects/RadHAR_PCL/test_data001.h5", "test_dataset"));
//
//    training_labels.save(arma::hdf5_name("/home/kangleli/Projects/RadHAR_PCL/training_labels001.h5", "train_labels"));
//    arma::vec my_trainlabels;
//    my_trainlabels.load(arma::hdf5_name("/home/kangleli/Projects/RadHAR_PCL/training_labels001.h5", "train_labels"));
//    test_labels.save(arma::hdf5_name("/home/kangleli/Projects/RadHAR_PCL/test_labels001.h5", "test_labels"));
//    arma::vec my_testlabels;
//    my_testlabels.load(arma::hdf5_name("/home/kangleli/Projects/RadHAR_PCL/test_labels001.h5", "test_labels"));


    const int n_timesteps = 60;
    const int n_samples = 25;
    const int n_feature = 640;
    arma::cube prediction;
    arma::cube labels = arma::zeros<arma::cube>(1, n_samples, n_timesteps);
    for(size_t i = 0; i < n_samples; i++) {
        const int value = training_labels[i] + 1;
        labels.tube(0,i).fill(value);
    }

    for(size_t i=0;i<labels.n_slices;++i)
        cout<<labels.slice(i) <<endl;

    BRNN<> model(n_samples);
    model.Add<IdentityLayer<>>();
    model.Add<LSTM<>>(n_feature, 64, n_samples);
    model.Add<Dropout<>>(0.1);
    model.Add<Linear<>>(64, 5);
    ens::SGD<> sgd(0.01, 1, 100 );
    model.Train(training_data, labels, sgd);
    model.Predict(test_data, prediction);


    return 0;
}
