cmake_minimum_required(VERSION 3.5)
project(terra)

set(CMAKE_CXX_STANDARD 11)
set(terra_VERSION_MAJOR 0)
set(terra_VERSION_MINOR 7)
set(terra_VERSION_PATCH 0)
set(terra_VERSION ${terra_VERSION_MAJOR}.${terra_VERSION_MINOR}.${terra_VERSION_PATCH})


add_subdirectory(pybind11)
subdirs(src)
subdirs(test)

include_directories(${terra_SOURCE_DIR}/include/terra)

enable_testing()



