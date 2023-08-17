cmake_minimum_required(VERSION 3.5)

project(test1 LANGUAGES CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES main.cpp singlepulsetimer.cpp Logger.cpp)
set(HEADER_FILES Logger.h singlepulsetimer.h)

add_executable(test1 ${SOURCE_FILES} ${HEADER_FILES})
