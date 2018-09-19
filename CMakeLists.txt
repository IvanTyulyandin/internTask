cmake_minimum_required(VERSION 3.10)
project(internTask C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -march=native")

add_subdirectory(dataReader)
add_subdirectory(boxFilter)
add_subdirectory(bandMatrixVector)