cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0079 NEW)

# Project metadata
project(YDYTU1 LANGUAGES CXX)

# Use modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find pybind11 (required for Python bindings)
set(pybind11_DIR "C:/Users/WWW/AppData/Local/Programs/Python/Python39/Lib/site-packages/pybind11/share/cmake/pybind11")
set(PYBIND11_FINDPYTHON ON)
find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 REQUIRED)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(pystuff)