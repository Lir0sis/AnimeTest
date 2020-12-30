cmake_minimum_required(VERSION 3.17)
project("AnimeTest" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(lib_path ${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(${lib_path})
add_subdirectory(${CMAKE_SOURCE_DIR}/src)
