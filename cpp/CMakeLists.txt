cmake_minimum_required(VERSION 3.13)

set(LINKS_PLATFORM_TESTS ON CACHE BOOL "Whether to compile tests")
set(LINKS_PLATFORM_BENCHMARKS ON CACHE BOOL "Whether to compile benchmarks")
set(LINKS_PLATFORM_EXTRA_FLAGS "" CACHE STRING "Extra compiler flags")

set(CONAN_DISABLE_CHECK_COMPILER TRUE)

project(Platform.Collections.Methods CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(GTest)
find_package(benchmark)
find_package(Platform.Interfaces)

add_library(${PROJECT_NAME}.Library INTERFACE)
target_include_directories(${PROJECT_NAME}.Library INTERFACE ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}.Library INTERFACE Platform.Interfaces::Platform.Interfaces)

if(${LINKS_PLATFORM_TESTS})
    add_executable(${PROJECT_NAME}.Tests ${PROJECT_NAME}.Tests/AllTests.cpp)
    set_target_properties(${PROJECT_NAME}.Tests PROPERTIES CXX_STANDARD 20)
    target_link_libraries(${PROJECT_NAME}.Tests PRIVATE GTest::gtest)
    target_link_libraries(${PROJECT_NAME}.Tests PRIVATE GTest::gtest_main)
    target_link_libraries(${PROJECT_NAME}.Tests PRIVATE ${PROJECT_NAME}.Library)
endif()

if(${LINKS_PLATFORM_BENCHMARKS})
    add_executable(${PROJECT_NAME}.Benchmarks ${PROJECT_NAME}.Benchmarks/AllBenchmarks.cpp)
    set_target_properties(${PROJECT_NAME}.Benchmarks PROPERTIES CXX_STANDARD 20)
    target_link_libraries(${PROJECT_NAME}.Benchmarks PRIVATE benchmark::benchmark)
    target_link_libraries(${PROJECT_NAME}.Benchmarks PRIVATE ${PROJECT_NAME}.Library)
endif()