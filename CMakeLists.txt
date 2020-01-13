cmake_minimum_required(VERSION 3.15)
project(RadHAR_PCL)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")
set(CMAKE_CXX_STANDARD 14)

find_package(PCL 1.8 REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})
list(REMOVE_ITEM PCL_LIBRARIES "vtkproj4")

link_libraries(stdc++fs)
add_executable(RadHAR_PCL txt_to_pcd_conversion.cpp ESF_feature_extract.cpp ESF_based_HAR.cpp)
target_link_libraries (RadHAR_PCL ${PCL_LIBRARIES})

