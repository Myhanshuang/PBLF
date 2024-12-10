cmake_minimum_required(VERSION 3.28)
project(KeyRhythm)

set(CMAKE_CXX_STANDARD 17)

add_executable(KeyRhythm main.cpp
        ChartAnalyzer.h
        ChartAnalyzer.cpp
        "Try_Catch templete.cpp")
