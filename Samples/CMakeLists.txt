set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Sample_g_area                Sample_g_area.c)
add_executable(Sample_g_arrow               Sample_g_arrow.c)
add_executable(Sample_g_bird_view           Sample_g_bird_view.c)
add_executable(Sample_g_box                 Sample_g_box.c)
add_executable(Sample_g_circle              Sample_g_circle.c)
add_executable(Sample_g_clipping            Sample_g_clipping.c)
add_executable(Sample_g_cone                Sample_g_cone.c)
add_executable(Sample_g_contln              Sample_g_contln.c)
add_executable(Sample_g_cylinder            Sample_g_cylinder.c)
add_executable(Sample_g_data_plot           Sample_g_data_plot.c)
add_executable(Sample_g_ellipse             Sample_g_ellipse.c)
add_executable(Sample_g_fan                 Sample_g_fan.c)
add_executable(Sample_g_init                Sample_g_init.c)
add_executable(Sample_g_input               Sample_g_input.c)
add_executable(Sample_g_isosurface          Sample_g_isosurface.c)
add_executable(Sample_g_light               Sample_g_light.c)
add_executable(Sample_g_line                Sample_g_line.c)
add_executable(Sample_g_marker              Sample_g_marker.c)
add_executable(Sample_g_move_g_plot         Sample_g_move_g_plot.c)
add_executable(Sample_g_polygon             Sample_g_polygon.c)
add_executable(Sample_g_polyline            Sample_g_polyline.c)
add_executable(Sample_g_prism               Sample_g_prism.c)
add_executable(Sample_g_pyramid             Sample_g_pyramid.c)
add_executable(Sample_g_rectangle           Sample_g_rectangle.c)
add_executable(Sample_g_sphere              Sample_g_sphere.c)
add_executable(Sample_g_text                Sample_g_text.c)
add_executable(Sample_g_transparent         Sample_g_transparent.c)
add_executable(Sample_g_triangle            Sample_g_triangle.c)
add_executable(Sample_off_screen_rendering  Sample_off_screen_rendering.c)

target_link_libraries(Sample_g_area                  glsc3d_3)
target_link_libraries(Sample_g_arrow                 glsc3d_3)
target_link_libraries(Sample_g_bird_view             glsc3d_3)
target_link_libraries(Sample_g_box                   glsc3d_3)
target_link_libraries(Sample_g_circle                glsc3d_3)
target_link_libraries(Sample_g_clipping              glsc3d_3)
target_link_libraries(Sample_g_cone                  glsc3d_3)
target_link_libraries(Sample_g_contln                glsc3d_3)
target_link_libraries(Sample_g_cylinder              glsc3d_3)
target_link_libraries(Sample_g_data_plot             glsc3d_3)
target_link_libraries(Sample_g_ellipse               glsc3d_3)
target_link_libraries(Sample_g_fan                   glsc3d_3)
target_link_libraries(Sample_g_init                  glsc3d_3)
target_link_libraries(Sample_g_input                 glsc3d_3)
target_link_libraries(Sample_g_isosurface            glsc3d_3)
target_link_libraries(Sample_g_light                 glsc3d_3)
target_link_libraries(Sample_g_line                  glsc3d_3)
target_link_libraries(Sample_g_marker                glsc3d_3)
target_link_libraries(Sample_g_move_g_plot           glsc3d_3)
target_link_libraries(Sample_g_polygon               glsc3d_3)
target_link_libraries(Sample_g_polyline              glsc3d_3)
target_link_libraries(Sample_g_prism                 glsc3d_3)
target_link_libraries(Sample_g_pyramid               glsc3d_3)
target_link_libraries(Sample_g_rectangle             glsc3d_3)
target_link_libraries(Sample_g_sphere                glsc3d_3)
target_link_libraries(Sample_g_text                  glsc3d_3)
target_link_libraries(Sample_g_transparent           glsc3d_3)
target_link_libraries(Sample_g_triangle              glsc3d_3)
target_link_libraries(Sample_off_screen_rendering    glsc3d_3)