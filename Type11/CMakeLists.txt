link_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Type11 main.cpp)

target_link_libraries(Type11 glsc3d_3 basic)