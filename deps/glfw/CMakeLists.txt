cmake_minimum_required(VERSION 3.16)

include(FetchContent)

set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS  OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL     OFF CACHE BOOL "" FORCE)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.4)

FetchContent_MakeAvailable(glfw)

set_target_properties(glfw PROPERTIES FOLDER "deps/GLFW")
set_target_properties(update_mappings PROPERTIES FOLDER "deps/GLFW")

