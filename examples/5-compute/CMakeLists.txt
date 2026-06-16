cmake_minimum_required(VERSION 3.16)

project(5-compute)

# Reuse the project name as the target name to avoid repetition
set(TARGET_NAME ${PROJECT_NAME})

# Find source files
file(GLOB_RECURSE SOURCES "*.cpp" "*.h")

# Create the executable
add_executable(${TARGET_NAME} ${SOURCES})

# Set C++ standard
set_target_properties(${TARGET_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${TARGET_NAME}"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${TARGET_NAME}"
    FOLDER "examples"
)

# Link libraries
target_link_libraries(${TARGET_NAME}
    PRIVATE
        glfw
        glm
        baregl
)

# Shared settings between baregl projects
include(${CMAKE_SOURCE_DIR}/cmake/TargetConfig.cmake)
baregl_configure_target(${TARGET_NAME})
