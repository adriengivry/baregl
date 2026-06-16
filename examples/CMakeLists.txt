cmake_minimum_required(VERSION 3.16)

# Now add the example projects
add_subdirectory(1-triangle)
add_subdirectory(2-cube)
add_subdirectory(3-framebuffer)
add_subdirectory(4-geometry)
add_subdirectory(5-compute)
add_subdirectory(6-tessellation)

add_custom_target(examples DEPENDS 1-triangle 2-cube 3-framebuffer 4-geometry 5-compute 6-tessellation)
