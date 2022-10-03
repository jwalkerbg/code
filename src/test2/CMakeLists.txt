cmake_minimum_required(VERSION 3.5)

project(test2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test2 main.cpp)

install(TARGETS test2
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
