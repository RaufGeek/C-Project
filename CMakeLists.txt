cmake_minimum_required(VERSION 2.8)
project(MySweetHome)

# Enforce C++98 standard
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98")

# Add your main system files
add_executable(MSH_App main.cpp)