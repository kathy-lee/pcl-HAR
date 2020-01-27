cmake_minimum_required(VERSION 3.15)
project(RadHAR_PCL)

set(CXX_FLAGS "-Wall -O0")
set(CMAKE_CXX_FLAGS "${CXX_FLAGS}")
#set(CMAKE_CXX_FLAGS_DEBUG "${CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
message(${CMAKE_CXX_FLAGS_DEBUG})
set(CMAKE_CXX_STANDARD 14)

# OpenMP
find_package(OpenMP)
if (OPENMP_FOUND)
    message("Found OpenMP")
    add_compile_options(-fopenmp)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
endif()

find_package(PCL 1.10 REQUIRED)
if (PCL_FOUND)
    include_directories(${PCL_INCLUDE_DIRS})
    link_directories(${PCL_LIBRARY_DIRS})
    message(${PCL_LIBRARY_DIRS})
    add_definitions(${PCL_DEFINITIONS})
    list(REMOVE_ITEM PCL_LIBRARIES "vtkproj4")
endif()

find_package(HDF5)
if(HDF5_FOUND)
    include_directories(${HDF5_INCLUDE_DIR})
    link_directories(${HDF5_LIBRARY_DIRS})
    add_definitions(${HDF5_DEFINITIONS})
endif()

# Find libmlpack.so (or equivalent) and store it in MLPACK_LIBRARY.
# If this example script were smarter, it would also find other dependencies of mlpack and store them in
# an MLPACK_LIBRARIES variable instead.
find_library(MLPACK_LIBRARY
        NAMES mlpack
        PATHS "$ENV{ProgramFiles}/mlpack/" /usr/local/lib/ /usr/lib64/ /usr/lib/ /usr/local/lib64/ /usr/local/
        )
include_directories(${MLPACK_INCLUDE_DIR})

link_libraries(stdc++fs)
add_executable(RadHAR_PCL esf_based_classification.cpp)
#add_executable(RadHAR_PCL ESF_based_HAR.cpp)
target_link_libraries (RadHAR_PCL ${PCL_LIBRARIES} ${HDF5_LIBRARIES} ${MLPACK_LIBRARY} armadillo)

