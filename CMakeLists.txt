cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(tower-defense)

set(CMAKE_CXX_STANDARD 20)

include_directories(./include ./external/include)
aux_source_directory(./src SRCS)
aux_source_directory(./external/src EXT_SRCS)

add_executable(${PROJECT_NAME} ${EXT_SRCS} ${SRCS})
target_link_libraries(${PROJECT_NAME} -lGLEW -lglfw -lGL -ldl -lassimp)