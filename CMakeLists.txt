cmake_minimum_required(VERSION 3.16)

set(CMAKE_SYSTEM_NAME AmigaOS)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_PROJECT_NAME Amigablockgame)

project(AmigaAceplay VERSION 0.1.0 LANGUAGES C)
add_definitions(-DACE_DEBUG)

add_executable(AmigaAceplay main.c src/game.c src/menu.c)

add_subdirectory(deps/ace ace)
target_link_libraries(AmigaAceplay ace)

# Generating ADF
set(ADF_DIR "${CMAKE_CURRENT_BINARY_DIR}/adf")
add_custom_target(generateAdf
    COMMAND ${CMAKE_COMMAND} -E make_directory "${ADF_DIR}/s"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/AmigaAceplay" "${ADF_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/myacefont.fnt" "${ADF_DIR}"
    COMMAND ${CMAKE_COMMAND} -E echo "AmigaAceplay" > "${ADF_DIR}/s/startup-sequence"
    COMMAND exe2adf -l ${CMAKE_PROJECT_NAME} -a "${CMAKE_PROJECT_NAME}.adf" -d ${ADF_DIR}
#    COMMAND ${CMAKE_COMMAND} -E rm -rf "${ADF_DIR}"
    COMMENT "Generating ADF file"
)
