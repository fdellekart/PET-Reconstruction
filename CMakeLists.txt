cmake_minimum_required(VERSION 3.28)
project(mmr2petsird)

set(SOURCE_FILES src/main.cpp src/PETLINKStream.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE inc)
