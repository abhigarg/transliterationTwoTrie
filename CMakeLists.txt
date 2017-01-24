cmake_minimum_required(VERSION 3.6)
project(testUnicode)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp utrie.h)
add_executable(testUnicode ${SOURCE_FILES})