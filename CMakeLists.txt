cmake_minimum_required(VERSION 3.6)
project(terra)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        terra.cpp
        Quadedge.cpp
        Subdivision.cpp
        Map.cpp
        Mask.cpp
        cmdline.cpp
        GreedyInsert.cpp
        Heap.cpp
        greedy.cpp
        output.cpp
        )

add_executable(terra ${SOURCE_FILES})