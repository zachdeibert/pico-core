function (pico_add_flash_target TARGET)
    if (NOT ${ARGC} EQUAL 1)
        message(FATAL_ERROR "Expect exactly one argument")
    endif()

    get_target_property(${TARGET}_archive_directory ${TARGET} ARCHIVE_OUTPUT_DIRECTORY)
    if (${TARGET}_archive_directory)
        get_filename_component(output_path "${${TARGET}_archive_directory}"
                REALPATH BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}")
        file(MAKE_DIRECTORY "${output_path}")
        set(output_path "${output_path}/")
    else()
        set(output_path "")
    endif()

    if (NOT Picotool_FOUND)
        find_package(Picotool)
    endif()

    if (Picotool_FOUND)
        add_custom_target(flash
            COMMAND Picotool reboot -u -f
            COMMAND ${CMAKE_COMMAND} -E sleep 1
            COMMAND Picotool load -v -x ${output_path}$<IF:$<BOOL:$<TARGET_PROPERTY:${TARGET},OUTPUT_NAME>>,$<TARGET_PROPERTY:${TARGET},OUTPUT_NAME>,$<TARGET_PROPERTY:${TARGET},NAME>>.uf2
            DEPENDS
                ${TARGET}
                Picotool
            USES_TERMINAL
        )
    endif()
endfunction()
