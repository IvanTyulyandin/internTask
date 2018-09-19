cmake_minimum_required(VERSION 3.10)
project(bandMatrixVector C)

include_directories(${internTask_SOURCE_DIR}/dataReader)
add_executable(bandMatrixVector bandMatrixVector.c bandMatrixVector.h bandMatrixVectorRunner.c)

target_link_libraries(bandMatrixVector dataReader)
