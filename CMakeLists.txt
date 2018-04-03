cmake_minimum_required(VERSION 3.5)
project(terra)

set(CMAKE_CXX_STANDARD 11)
set(terra_VERSION_MAJOR 0)
set(terra_VERSION_MINOR 7)
set(terra_VERSION_PATCH 0)
set(terra_VERSION ${terra_VERSION_MAJOR}.${terra_VERSION_MINOR}.${terra_VERSION_PATCH})

set(terra_VERSION_MAJOR 0)
set(terra_VERSION_MINOR 7)
set(terra_VERSION_PATCH 0)
set(terra_VERSION ${terra_VERSION_MAJOR}.${terra_VERSION_MINOR}.${terra_VERSION_PATCH})

subdirs(src)
include_directories(${terra_SOURCE_DIR}/include/terra)

enable_testing()
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*.cpp)

foreach (testSrc ${TEST_SRCS})
    get_filename_component(testName ${testSrc} NAME_WE)

    add_executable(${testName} ${testSrc})
    target_link_libraries(${testName} ${Boost_LIBRARIES} terra)
    set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin)
    add_test(NAME ${testName} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/testBin/${testName})
endforeach (testSrc)

