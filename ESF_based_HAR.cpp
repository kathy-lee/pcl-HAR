//
// Created by kangleli on 1/13/20.
//
#include <iostream>
#include <string>
#include <experimental/filesystem>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/features/esf.h>
#include <pcl/features/impl/esf.hpp>
#include <pcl/visualization/pcl_plotter.h>

using namespace std;
namespace fs = std::experimental::filesystem;


//getESF();

vector<vector<vector<double>>> computeSample(vector<vector<double>> ESFsequence, int width, int slideStep){

    vector<vector<vector<double>>> samples;

    for(int i = 0; i < ESFsequence.size(); i += slideStep) {
        if((i + width + 1) > ESFsequence.size() )
            break;
        else{
            vector<vector<double>> sample( &ESFsequence[i], &ESFsequence[i + width] );
            samples.push_back(sample);
        }
    }
    return samples;
}

int main (int argc, char** argv) {

    // steps:
    // 1. load .pcd file in train/test data folder;
    // 2. get ESF feature of each pcd;
    // 4. save as train data set;
    // 5. do the same process from 1-3 to test data set.
    // 6. train SVM/LSTM model;
    // 7. predict and evaluate.

    string parent_dir = "/home/kangleli/Projects/RadHAR/PCD_Data/";
    string train_test_dir[2] = {"Train", "Test"};
    string action_dir[5] = {"boxing","jack","jump","squats","walk"};
    string sub_dir;
    pcl::PointCloud<pcl::PointXYZI>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZI>);
    int fileNo = 0;
    string fileName;
    vector<vector<double>> ESFsequence;
    vector<vector<vector<double>>> trainDataset, testDataset, samples;
    vector<int> trainLabels, testLabels, labels;
    int width, slideStep;

    //int timeStep = 0;
    //int slidWindowStep = 0;

    for(int i = 0; i < train_test_dir->size(); i++)
        for(int j = 0; j < action_dir->size(); j++) {
            sub_dir = parent_dir + train_test_dir[i] + "/" + action_dir[j];
            for (const auto &dirEntry : fs::directory_iterator(sub_dir)) {
                fileNo = 0;
                ESFsequence.empty();
                fileName = dirEntry.path().string() + "/" + to_string(fileNo) + ".pcd";
                while (fs::exists(fileName)) {
                    cout << fileName << endl;
                    // following will be later moved to a function, which input a pcd filename, output its ESF feature:
                    // getESF();
                    if (pcl::io::loadPCDFile<pcl::PointXYZI>(fileName, *cloud) == -1) {
                        cout << "Couldn't read pcd file. " << endl;
                        return 0;
                    }
                    //for(int k = 0; k < cloud->points.size(); k++){
                        //cout<< cloud->points[k]<<endl;
                    //}
                    cout << "Loaded " << cloud->points.size() << " data points from " + fileName << endl;
                    pcl::PointCloud<pcl::ESFSignature640>::Ptr descriptor(new pcl::PointCloud<pcl::ESFSignature640>);
                    pcl::ESFEstimation<pcl::PointXYZI, pcl::ESFSignature640> esf;
                    esf.setInputCloud(cloud);
                    esf.compute(*descriptor);
                    vector<double> feature(begin(descriptor->points[0].histogram),
                                           end(descriptor->points[0].histogram));

                    ESFsequence.push_back(feature);
                    fileNo++;
                    fileName = dirEntry.path().string() + "/" + to_string(fileNo) + ".pcd";
                }
                samples = computeSample(ESFsequence, 60, 10);
                labels = vector<int>(samples.size(), j);
                if ( i == 0){
                    trainDataset.insert(trainDataset.end(), begin(samples), end(samples));
                    trainLabels.insert(trainLabels.end(), begin(labels), end(labels));
                }
                else {
                    testDataset.insert(trainDataset.end(), begin(samples), end(samples));
                    testLabels.insert(trainLabels.end(), begin(labels), end(labels));
                }
            }
        }

    //model = trainModel(train_dataset,"SVM");
    //result = predict(test_dataset);
    //accuracy = evaluate(result,label);

    return 0;
}