cmake_minimum_required(VERSION 3.15)
project(reader_writer C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(reader_writer
        init.h
        Main_file.c
        read.h
        write.h)
