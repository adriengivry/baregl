function(baregl_configure_target TARGET_NAME)
    if(MSVC)
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
    endif()

    target_compile_definitions(${TARGET_NAME}
        PRIVATE
            $<$<CONFIG:Debug>:DEBUG>
            $<$<CONFIG:Release>:NDEBUG>
    )

    if(UNIX AND NOT APPLE)
        if(DEFINED X11_LIBS)
            target_link_libraries(${TARGET_NAME} PRIVATE ${X11_LIBS})
        endif()
        if(DEFINED WAYLAND_LIBS)
            target_link_libraries(${TARGET_NAME} PRIVATE ${WAYLAND_LIBS})
        endif()
    endif()
endfunction()
