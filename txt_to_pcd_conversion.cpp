//
// Created by kangleli on 1/8/20.
//
#include <iostream>
#include <string>
#include <experimental/filesystem>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>

using namespace std;
namespace fs = std::experimental::filesystem;

// steps:
// 1. iterate and read each .txt file from train/test data folder into a pcd object;
// 2. write each pcd object into pcd file, save to pcd data path.
int main(int argc, char **argv) {
    pcl::PointCloud<pcl::PointXYZI> cloud;

    //read point cloud data from txt file
    string parent_dir = "/home/kangleli/Projects/RadHAR/Data/";
    //string sub_dirs[5] = {"boxing","jack","jump","squats","walk"};
    string data_path;
    ifstream currentFile;
    string pcd_sub_dir = "PCD_Data/";
    vector<vector<double>> data;
    vector<double> tmpVec(4);
    string tmpString, fileName, dirName;
    double tmpNum;
    int fileCount;

    for (const auto &dirEntry : fs::recursive_directory_iterator(parent_dir)) {
        cout << dirEntry.path() << endl;
        if (dirEntry.path().extension() == ".txt") {
            currentFile.open(dirEntry.path());
            if (currentFile.is_open()) {
                dirName = string(dirEntry.path());
                dirName.erase(dirName.end() - 4, dirName.end());
                dirName.insert(31, "PCD_");  //not good
                if (!fs::create_directories(dirName)) {
                    cerr << "Create dir failed.";
                    return 0;
                }
                fileCount = 0;
                while (!(currentFile.eof())) {
                    currentFile >> tmpString;
                    if (tmpString == "point_id:") {
                        currentFile >> tmpString;
                        if (tmpString == "0" && !data.empty()) {
                            cloud.width = 1;
                            cloud.height = data.size();
                            cloud.is_dense = false;
                            cloud.points.resize(cloud.width * cloud.height);
                            for (int i = 0; i < data.size(); i++) {
                                cloud.points[i].x = data[i][0];
                                cloud.points[i].y = data[i][1];
                                cloud.points[i].z = data[i][2];
                                cloud.points[i].intensity = data[i][3];
                            }
                            fileName = dirName + "/" + to_string(fileCount) + ".pcd";
                            pcl::io::savePCDFileASCII(fileName, cloud);
                            //cout << "Saved " << cloud.points.size () << " data points to " << fileName << endl;
                            fileCount++;
                            data.clear();
                        }
                    }
                    if (tmpString == "x:") {
                        currentFile >> tmpNum;
                        tmpVec[0] = tmpNum;
                    }
                    if (tmpString == "y:") {
                        currentFile >> tmpNum;
                        tmpVec[1] = tmpNum;
                    }
                    if (tmpString == "z:") {
                        currentFile >> tmpNum;
                        tmpVec[2] = tmpNum;
                    }
                    if (tmpString == "intensity:") {
                        currentFile >> tmpNum;
                        tmpVec[3] = tmpNum;
                        data.push_back(tmpVec);
                    }
                }
                cout << "Generated " << fileCount << " pcd files." << endl;
                currentFile.close();
            }
        }
    }

    return (0);
}
