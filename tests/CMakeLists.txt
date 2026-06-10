cmake_minimum_required(VERSION 3.16)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(tests)

# Reuse the project name as the target name to avoid repetition
set(TARGET_NAME ${PROJECT_NAME})

file(GLOB_RECURSE SOURCES "*.cpp" "*.h")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

# Create the executable
add_executable(${TARGET_NAME} ${SOURCES})

# Add include directory
target_include_directories(${TARGET_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set C++ standard
set_target_properties(${TARGET_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${TARGET_NAME}"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${TARGET_NAME}"
    FOLDER "tests"
)

# Link libraries
target_link_libraries(${TARGET_NAME}
    PRIVATE
        glfw
        glm
        baregl
        Catch2::Catch2WithMain
)

# Enable tests
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(${TARGET_NAME})

# Shared settings between baregl projects
include(${CMAKE_SOURCE_DIR}/cmake/TargetConfig.cmake)
baregl_configure_target(${TARGET_NAME})
