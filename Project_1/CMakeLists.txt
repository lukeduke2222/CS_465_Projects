cmake_minimum_required(VERSION 3.21)
project(Project_1 C)

set(CMAKE_C_STANDARD 99)

add_executable(Project_1 AES_Implementation.c aes_helper.c aes_helper.h)
