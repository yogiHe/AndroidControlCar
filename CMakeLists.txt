cmake_minimum_required(VERSION 2.8)

project(auto_car)

aux_source_directory(. DIR_SRCS)

add_subdirectory(motor)

add_executable(auto_car ${DIR_SRCS})

target_link_libraries(auto_car MotorAction)