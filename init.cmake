file(WRITE ${CMAKE_BINARY_DIR}/.gitignore *)

if (NOT PICO_BOARD)
    set(PICO_BOARD none)
endif()
if (NOT PICO_COMPILER)
    set(PICO_COMPILER pico_arm_gcc)
endif()
if (NOT PICO_PLATFORM)
    set(PICO_PLATFORM rp2040)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/external/pico-sdk/pico_sdk_init.cmake)

list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_LIST_DIR}/../core/cmake
    ${CMAKE_CURRENT_LIST_DIR}/cmake
)
include(pico_add_flash_target)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
include(Wall)
add_compile_definitions(
    LIB_CMSIS_CORE=0
)
if (NOT MSVC)
    add_compile_options(
        -Wdouble-promotion
    )
endif()
