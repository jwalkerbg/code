cmake_minimum_required(VERSION 3.5)

project(test1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test1 main.cpp singlepulsetimer.cpp Logger.cpp)
