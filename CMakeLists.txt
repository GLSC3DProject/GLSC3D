cmake_minimum_required(VERSION 2.8)
project(GLSC3D)

if (UNIX)
	list(APPEND CMAKE_C_FLAGS	" -O2 -std=c99 ")
	list(APPEND CMAKE_CXX_FLAGS	" -O2 -std=c++14 ")
endif()

include_directories(Include)

add_subdirectory(Src)
add_subdirectory(Samples)
add_subdirectory(Advanced)
