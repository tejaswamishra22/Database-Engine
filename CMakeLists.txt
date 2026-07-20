cmake_minimum_required(VERSION 3.16)
project(DatabaseEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(database_engine
    storage/disk_manager.cpp
)
