cmake_minimum_required(VERSION 3.1)
project(GLSC3D)

if (UNIX)
	list(APPEND CMAKE_C_FLAGS	" -O2 ")
	list(APPEND CMAKE_CXX_FLAGS	" -O2 ")
endif()
set(CMAKE_CXX_STANDARD 11)

include_directories(Include)

add_subdirectory(Src)
add_subdirectory(Samples)
add_subdirectory(Advanced)
