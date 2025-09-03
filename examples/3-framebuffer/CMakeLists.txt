cmake_minimum_required(VERSION 3.16)

project(3-framebuffer)

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

# Configuration-specific settings
target_compile_definitions(${TARGET_NAME}
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
)

# Platform-specific options
if(WIN32)
    target_compile_options(${TARGET_NAME}
        PRIVATE
            $<$<CONFIG:Debug>:/Zi /Od /MDd>
            $<$<CONFIG:Release>:/O2 /MD>
    )
else()
    target_compile_options(${TARGET_NAME}
        PRIVATE
            $<$<CONFIG:Debug>:-g -O0>
            $<$<CONFIG:Release>:-O3>
    )

    # Add platform-specific libraries
    if(UNIX AND NOT APPLE)
        if(DEFINED X11_LIBS)
            target_link_libraries(${TARGET_NAME} PRIVATE ${X11_LIBS})
        endif()
        if(DEFINED WAYLAND_LIBS)
            target_link_libraries(${TARGET_NAME} PRIVATE ${WAYLAND_LIBS})
        endif()
    endif()
endif()
