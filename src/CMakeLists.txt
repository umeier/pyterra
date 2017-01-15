include_directories(${terra_SOURCE_DIR}/include/terra)
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