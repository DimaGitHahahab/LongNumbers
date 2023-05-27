cmake_minimum_required(VERSION 3.25)
project(bigInteger)

set(CMAKE_CXX_STANDARD 23)

add_executable(bigInteger main.cpp LN.cpp MyVector.h MyVector.cpp return_codes.h)
