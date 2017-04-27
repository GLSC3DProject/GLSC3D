#link_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Type11 main.cpp mt19937-64.c GetParam2.c Rainbow.c Embedded_Explicit_Runge_Kutta_Multiple.c)

target_link_libraries(Type11 glsc3d_3)