cmake_minimum_required(VERSION 3.16)

include(FetchContent)

set(GLM_BUILD_TESTS   OFF CACHE BOOL "" FORCE)
set(GLM_ENABLE_CXX_20 ON  CACHE BOOL "" FORCE)
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG 1.0.3)

FetchContent_MakeAvailable(glm)

set_target_properties(glm PROPERTIES FOLDER "deps")

