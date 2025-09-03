cmake_minimum_required(VERSION 3.16)
project(baregl)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define options
option(BAREGL_GENERATE_EXAMPLES "Generate examples" OFF)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/baregl)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/baregl)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/baregl)

# Create the library
file(GLOB_RECURSE PRIVATE_SRC "src/*.cpp" "src/*.c" "src/*.h")
file(GLOB_RECURSE PUBLIC_SRC "include/*.h")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "src" FILES ${PRIVATE_SRC})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include PREFIX "include" FILES ${PUBLIC_SRC})
add_library(baregl STATIC ${PRIVATE_SRC} ${PUBLIC_SRC})

# Add include directory
target_include_directories(baregl
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Configuration-specific settings
target_compile_definitions(baregl
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
)

# Debug configuration
set_target_properties(baregl PROPERTIES
    DEBUG_POSTFIX ""
)

# Include examples if the option is enabled
if(BAREGL_GENERATE_EXAMPLES)
    message(STATUS "Generating BareGL examples...")
    add_subdirectory(examples)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT 1-triangle)
endif()
