cmake_minimum_required(VERSION 3.14)
project(simplekvdb VERSION 1.0 LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  rpclib
  GIT_REPOSITORY https://github.com/rpclib/rpclib.git
  GIT_TAG        v2.3.0
)

FetchContent_MakeAvailable(rpclib)

add_subdirectory(simplekvdb)
add_subdirectory(kvserver)
add_subdirectory(kvclient)