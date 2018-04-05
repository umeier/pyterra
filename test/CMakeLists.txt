find_package(Boost REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIR})

add_executable(test_map test_map.cpp)
target_link_libraries(test_map terra)

file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*.cpp)

configure_file(test_settings.h.in ${CMAKE_CURRENT_SOURCE_DIR}/test/test_settings.h)

foreach (testSrc ${TEST_SRCS})
    get_filename_component(testName ${testSrc} NAME_WE)

    add_executable(${testName} ${testSrc})
    target_link_libraries(${testName} ${Boost_LIBRARIES} terra)
    set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin)
    add_test(NAME ${testName} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/testBin/${testName})
endforeach (testSrc)
