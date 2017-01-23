include_directories(${terra_SOURCE_DIR}/include/terra)
set(LIB_SOURCE_FILES
        Array.h
        Geom.h
        Vec2.h
        Vec3.h
        version.h
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

add_library(terralib SHARED ${LIB_SOURCE_FILES})
add_executable(terra ${EXEC_SOURCE_FILES})
target_link_libraries(terra terralib)