# Based on ../external/pico-sdk/tools/FindPioasm.cmake
# Finds (or builds) the Picotool executable
#
# This will define the following variables
#
#    Picotool_FOUND
#
# and the following imported targets
#
#     Picotool
#

if (NOT Picotool_FOUND)
    include(ExternalProject)

    set(PICOTOOL_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/picotool)
    set(PICOTOOL_BINARY_DIR ${CMAKE_BINARY_DIR}/picotool)

    set(PicotoolBuild_TARGET PicotoolBuild)
    set(Picotool_TARGET Picotool)

    if (NOT TARGET ${PicotoolBuild_TARGET})
        pico_message_debug("PICOTOOL will need to be built")
        ExternalProject_Add(${PicotoolBuild_TARGET}
                PREFIX picotool
                SOURCE_DIR ${PICOTOOL_SOURCE_DIR}
                BINARY_DIR ${PICOTOOL_BINARY_DIR}
                CMAKE_ARGS "-DCMAKE_MAKE_PROGRAM:FILEPATH=${CMAKE_MAKE_PROGRAM}"
                CMAKE_CACHE_ARGS "-DPICO_SDK_PATH:STRING=${PICO_SDK_PATH}"
                BUILD_ALWAYS 1 # force dependency checking
                INSTALL_COMMAND ""
                )
    endif()

    if (CMAKE_HOST_WIN32)
        set(Picotool_EXECUTABLE ${PICOTOOL_BINARY_DIR}/picotool/picotool.exe)
    else()
        set(Picotool_EXECUTABLE ${PICOTOOL_BINARY_DIR}/picotool/picotool)
    endif()
    if(NOT TARGET ${Picotool_TARGET})
        add_executable(${Picotool_TARGET} IMPORTED)
    endif()
    set_property(TARGET ${Picotool_TARGET} PROPERTY IMPORTED_LOCATION
            ${Picotool_EXECUTABLE})

    add_dependencies(${Picotool_TARGET} ${PicotoolBuild_TARGET})
    set(Picotool_FOUND 1)
endif()
