cmake_minimum_required(VERSION 3.20)
project(PKV_Keygen)

set(CMAKE_C_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

add_executable(validator validator.c pkv.h)
add_executable(keygen keygen.c pkv.h)
