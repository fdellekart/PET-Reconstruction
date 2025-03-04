cmake_minimum_required(VERSION 3.28)
project(mmr2petsird)

find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(TBB REQUIRED COMPONENTS tbb)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests tests/test_petlinkstream.cpp src/PETLINKStream.cpp tests/test_lookuptable.cpp src/LookupTable.cpp tests/test_sinogram.cpp src/Sinogram.cpp tests/test_raytracer.cpp src/RayTracer.cpp src/Geometry.cpp tests/test_geometry.cpp)
target_include_directories(tests PRIVATE inc)
target_link_libraries(tests GTest::gtest_main tbb)
target_compile_definitions(tests PRIVATE PARALLEL)
target_compile_definitions(tests PRIVATE _GLIBCXX_PARALLEL)
target_compile_options(tests PRIVATE -fopenmp)

include(GoogleTest)
gtest_discover_tests(tests PROPERTIES DISCOVERY_TIMEOUT 600)

set(SOURCE_FILES src/main.cpp src/PETLINKStream.cpp src/LookupTable.cpp src/Sinogram.cpp src/Geometry.cpp src/RayTracer.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE inc)
target_link_libraries(${PROJECT_NAME} tbb)
target_compile_definitions(${PROJECT_NAME} PRIVATE PARALLEL)
target_compile_definitions(${PROJECT_NAME} PRIVATE _GLIBCXX_PARALLEL)
target_compile_options(${PROJECT_NAME} PRIVATE -fopenmp)

add_executable(extract_timeframe src/extract_timeframe.cpp src/PETLINKStream.cpp)
target_include_directories(extract_timeframe PRIVATE inc)
