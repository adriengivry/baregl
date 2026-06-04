cmake_minimum_required(VERSION 3.16)
project(baregl)

# Reuse the project name as the target name to avoid repetition
set(TARGET_NAME ${PROJECT_NAME})

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Define options
option(BAREGL_GENERATE_EXAMPLES "Generate examples" OFF)
option(BAREGL_GENERATE_TESTS "Generate tests" OFF)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${TARGET_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${TARGET_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${TARGET_NAME})

# Create the library
file(GLOB_RECURSE PRIVATE_SRC "src/*.cpp" "src/*.c" "src/*.h")
file(GLOB_RECURSE PUBLIC_SRC "include/*.h")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "src" FILES ${PRIVATE_SRC})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include PREFIX "include" FILES ${PUBLIC_SRC})
add_library(${TARGET_NAME} STATIC ${PRIVATE_SRC} ${PUBLIC_SRC})

# Add include directory
target_include_directories(${TARGET_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Configuration-specific settings
target_compile_definitions(${TARGET_NAME}
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
)

# Debug configuration
set_target_properties(${TARGET_NAME} PROPERTIES
    DEBUG_POSTFIX ""
)

# Add dependencies when either examples or tests projects are requested
if(BAREGL_GENERATE_EXAMPLES OR BAREGL_GENERATE_TESTS)
    add_subdirectory(deps/glfw)
    add_subdirectory(deps/glm)
endif()

# Include examples if the option is enabled
if(BAREGL_GENERATE_EXAMPLES)
    message(STATUS "Generating BareGL examples...")
    add_subdirectory(examples)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT 1-triangle)
endif()

if(BAREGL_GENERATE_TESTS)
    message(STATUS "Generating BareGL tests...")
    add_subdirectory(deps/Catch2)
    add_subdirectory(tests)
endif()
