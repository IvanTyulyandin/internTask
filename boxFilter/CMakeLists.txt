cmake_minimum_required(VERSION 3.10)
project(boxFilter C)

include_directories(${internTask_SOURCE_DIR}/dataReader)
add_executable(boxFilter boxFilter.c boxFilter.h boxFilterRunner.c)

target_link_libraries(boxFilter dataReader)
