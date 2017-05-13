cmake_minimum_required(VERSION 2.8)
project(GLSC3D)

if (UNIX)
	set(CMAKE_C_FLAGS	" -O2 -std=c99 ")
	set(CMAKE_CXX_FLAGS	" -O2 -std=c++11 ")
endif()

include_directories(Include)

if (MSVC)
	include_directories(${CMAKE_SOURCE_DIR}/Windows_dependency/Include)
	link_directories(${CMAKE_SOURCE_DIR}/Windows_dependency/Lib)
endif()

add_subdirectory(Src)
add_subdirectory(Samples)
add_subdirectory(Advanced)
