cmake_minimum_required(VERSION 3.28)
project(mmr2petsird)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests tests/test_petlinkstream.cpp src/PETLINKStream.cpp tests/test_lookuptable.cpp src/LookupTable.cpp)
target_include_directories(tests PRIVATE inc)
target_link_libraries(tests GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(tests PROPERTIES DISCOVERY_TIMEOUT 600)

set(SOURCE_FILES src/main.cpp src/PETLINKStream.cpp src/LookupTable.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE inc)

add_executable(extract_timeframe src/extract_timeframe.cpp src/PETLINKStream.cpp)
target_include_directories(extract_timeframe PRIVATE inc)
