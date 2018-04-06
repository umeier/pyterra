cmake_minimum_required(VERSION 3.5)
project(pyterra)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_DIR "src/pyterra")

include_directories(${SOURCE_DIR})
set(SOURCES
        "${SOURCE_DIR}/Quadedge.cpp"
        "${SOURCE_DIR}/Subdivision.cpp"
        "${SOURCE_DIR}/Map.cpp"
        "${SOURCE_DIR}/Mask.cpp"
        "${SOURCE_DIR}/GreedyInsert.cpp"
        "${SOURCE_DIR}/Heap.cpp"
        "${SOURCE_DIR}/greedy.cpp"
        )

add_subdirectory(lib/pybind11)
pybind11_add_module(pyterra ${SOURCES} ${SOURCE_DIR}/terra.cpp)

SET(TEST_DIR "tests")
SET(TESTS ${SOURCES} "${TEST_DIR}/test_main.cpp" "${TEST_DIR}/test_terra.cpp")
include_directories(lib/catch)
add_executable("${PROJECT_NAME}_test" ${TESTS})





