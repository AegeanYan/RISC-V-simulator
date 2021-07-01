cmake_minimum_required(VERSION 3.17)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(code
        simulator.h
        simulator.cpp
        Memory.cpp
        Memory.h
        ALU.cpp
        ALU.h
        Registor.cpp
        Registor.h
        mian.cpp)
