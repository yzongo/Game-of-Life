cmake_minimum_required(VERSION 4.3)
project(GameOfLife C)

set(CMAKE_C_STANDARD 90)

add_executable(GameOfLife main.c
        life.c
        life.h)
