cmake_minimum_required(VERSION 3.8)
project(Calculator)

set(CMAKE_CXX_STANDARD 14)

add_executable(Calculator main.cpp RPN/RPN.cpp RPN/RPN.h)