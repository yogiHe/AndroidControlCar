aux_source_directory(. DIR_LIB_SRCS)

link_libraries("libwiringPi.so" "wiringPiDev" "pthread")

add_library(MotorAction ${DIR_LIB_SRCS})