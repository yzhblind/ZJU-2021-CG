cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(tower-defense LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

include_directories(./include)
aux_source_directory(./src SRCS)

add_executable(${PROJECT_NAME} ${SRCS})