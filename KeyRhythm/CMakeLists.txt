cmake_minimum_required(VERSION 3.28)
project(KeyRhythm)

set(CMAKE_CXX_STANDARD 17)

add_executable(KeyRhythm main.cpp
        "Try_Catch template.cpp"
        Chart.h
        Chart.cpp
        ChartWork.h
        ChartWork.cpp
        ChartError.h
        ChartRead.h
        ChartRead.cpp
)
