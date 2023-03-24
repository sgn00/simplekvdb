cmake_minimum_required(VERSION 3.14)
project(simplekvdb VERSION 1.0 LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(simplekvdb)
add_subdirectory(cli_app)
add_subdirectory(tests)