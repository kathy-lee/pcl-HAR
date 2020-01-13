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
// 1. read each .txt file from train data folder into a pcd object;
// 2. write each pcd object into pcd file;
// 3. do the same process to test data folder.
int main (int argc, char** argv)
{
  pcl::PointCloud<pcl::PointXYZI> cloud;

  //read point cloud data from txt file
  string parent_dir = "/home/kangleli/Projects/RadHAR/Data/Train/";
  string sub_dirs[5] = {"boxing","jack","jump","squats","walk"};
  string data_path;
  ifstream currentFile;
  vector<vector<double>> data;
  vector<double> tmpVec(4);
  string tmpString, fileName;
  double tmpNum;


  for(int i = 0; i < 5; i++){
      data_path = parent_dir + sub_dirs[i];
      for (const auto& dirEntry : fs::recursive_directory_iterator(data_path)) {
          cout << dirEntry.path() <<endl;
          if (string(dirEntry.path()) == "/home/kangleli/Projects/RadHAR/Data/Train/boxing/20_boxing_1.txt")
              cout << "!" << endl;
          if ( dirEntry.path().extension() == ".txt" ) {
              currentFile.open(dirEntry.path());
              if(currentFile.is_open()) {
                  while (!(currentFile.eof())) {
                      currentFile >> tmpString;
                      if(tmpString == "x:") {
                          currentFile >> tmpNum;
                          tmpVec[0] = tmpNum;
                      }
                      if(tmpString == "y:") {
                          currentFile >> tmpNum;
                          tmpVec[1] = tmpNum;
                      }
                      if(tmpString == "z:") {
                          currentFile >> tmpNum;
                          tmpVec[2] = tmpNum;
                      }
                      if(tmpString == "intensity:") {
                          currentFile >> tmpNum;
                          tmpVec[3] = tmpNum;
                          data.push_back(tmpVec);
                      }
                      //int mkdir (char *filename)
                  }
                  //write data into pcd file
                  cloud.width = 1;
                  cloud.height = data.size();
                  cloud.is_dense = false;
                  cloud.points.resize(cloud.width * cloud.height);
                  for(int i = 0; i < data.size(); i++){
                      cloud.points[i].x = data[i][0];
                      cloud.points[i].y = data[i][1];
                      cloud.points[i].z = data[i][2];
                      cloud.points[i].intensity = data[i][3];
                  }
                  fileName = string(dirEntry.path());
                  fileName.erase(fileName.end()-3, fileName.end());
                  fileName = fileName + "pcd";
                  pcl::io::savePCDFileASCII (fileName, cloud);
                  cout << "Saved " << cloud.points.size () << " data points to " << fileName << endl;
                  currentFile.close();
              }
          }
      }
  }

  return (0);
}
