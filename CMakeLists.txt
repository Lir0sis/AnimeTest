cmake_minimum_required(VERSION 3.17)
project("AnimeTest" LANGUAGES C CXX)

set(lib_path ${CMAKE_SOURCE_DIR}/lib)
set(linked_lib glfw
	assimp
	glm
	glad
	stb
)

add_subdirectory(${lib_path})
add_subdirectory(${CMAKE_SOURCE_DIR}/src)
