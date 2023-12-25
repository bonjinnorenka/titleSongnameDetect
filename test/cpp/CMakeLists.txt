cmake_minimum_required(VERSION 3.13)
project(vsing CXX)

include_directories(C:\\custom_app\\msys2\\mingw32\\include)
link_directories(C:\\custom_app\\msys2\\mingw32\\lib)

add_executable(test cpptest.cpp ../../src/cpp/titleSongnameDetect.cpp)

set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -std=gnu++17 -fno-inline")
