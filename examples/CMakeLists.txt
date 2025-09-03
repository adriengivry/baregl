cmake_minimum_required(VERSION 3.16)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# First, add the dependencies
add_subdirectory(deps/_glm)
add_subdirectory(deps/_glfw)

# Now add the example projects
add_subdirectory(1-triangle)
add_subdirectory(2-cube)
add_subdirectory(3-framebuffer)
add_subdirectory(4-geometry)
add_subdirectory(5-compute)
add_subdirectory(6-tessellation)
