add_library(boot3 STATIC boot3.c)
target_link_libraries(boot3 hardware_flash_headers hardware_regs_headers pico_bootrom_headers pico_platform_headers)

function(pico_enable_boot3 TARGET)
    target_link_libraries(${TARGET} boot3)
    pico_set_linker_script(${TARGET} ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/memmap.ld)
endfunction()
