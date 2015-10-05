cmake_minimum_required(VERSION 2.8)
project(UTM)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "-Wall -std=c++11 -stdlib=libc++ ${CMAKE_CXX_FLAGS}")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "-Wall -Wno-enum-compare -Wno-sign-compare -std=c++11 ${CMAKE_CXX_FLAGS}")
endif()

make_directory("${CMAKE_BINARY_DIR}/include/UTM")
make_directory("${CMAKE_BINARY_DIR}/data")

include_directories(${CMAKE_BINARY_DIR}/include)

add_subdirectory(src)
add_subdirectory(data)

add_executable(cfgParser.bin tools/mainCFGParser.cpp)
target_link_libraries(cfgParser.bin cfglib pdalib)

add_executable(cfgToPda.bin tools/mainCFGtoPDA.cpp)
target_link_libraries(cfgToPda.bin cfglib pdalib)

add_executable(cnfTest.bin tools/mainCNFtest.cpp)
target_link_libraries(cnfTest.bin cfglib)

add_executable(cyk.bin tools/mainCYKTest.cpp)
target_link_libraries(cyk.bin cfglib)

add_executable(tmSimulator.bin tools/mainTMtest.cpp)
target_link_libraries(tmSimulator.bin tmlib)

add_executable(utmSimulator.bin tools/mainUTMtest.cpp)
target_link_libraries(utmSimulator.bin tmlib)

add_executable(tmConverterTest.bin tools/mainTMConverterTest.cpp)
target_link_libraries(tmConverterTest.bin tmlib)

add_executable(Tools_UI.bin tools/mainTools_UI.cpp)
target_link_libraries(Tools_UI.bin cfglib pdalib tmlib uilib)

enable_testing()
add_subdirectory(test)

## GTEST ##
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
add_executable( runUnitTests 
    test/all.cpp
    )
target_link_libraries(runUnitTests gtest gtest_main cfglib pdalib tmlib)
add_test( runUnitTests runUnitTests "${CMAKE_SOURCE_DIR}/data/")

INSTALL(TARGETS Space_Invaders DESTINATION ${PROJECT_SOURCE_DIR}/bin)
