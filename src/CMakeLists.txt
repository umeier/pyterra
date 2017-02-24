include_directories(${terra_SOURCE_DIR}/include/terra)
set(LIB_SOURCE_FILES
        Quadedge.cpp
        Subdivision.cpp
        Map.cpp
        Mask.cpp
        GreedyInsert.cpp
        Heap.cpp
        greedy.cpp
        )

set (EXEC_SOURCE_FILES
        terra.cpp
        cmdline.cpp
        output.cpp
        )

add_library(terra SHARED ${LIB_SOURCE_FILES})
SET_TARGET_PROPERTIES(terra PROPERTIES VERSION ${terra_VERSION} SOVERSION ${terra_VERSION_MAJOR})
add_executable(terra-bin ${EXEC_SOURCE_FILES})
target_link_libraries(terra-bin terra)
SET_TARGET_PROPERTIES(terra-bin PROPERTIES OUTPUT_NAME terra)