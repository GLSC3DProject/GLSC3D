cmake_minimum_required(VERSION 3.1)
project(GLSC3D)

if (UNIX)
	list(APPEND CMAKE_C_FLAGS	" -W -Wall")
	list(APPEND CMAKE_CXX_FLAGS	" -W -Wall")
endif()
set(CMAKE_CXX_STANDARD 11)

include_directories(Include)

add_subdirectory(Src)
add_subdirectory(Samples)
add_subdirectory(Advanced)
