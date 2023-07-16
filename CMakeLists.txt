cmake_minimum_required(VERSION 3.16)

set(CMAKE_SYSTEM_NAME AmigaOS)
set(CMAKE_SYSTEM_VERSION 1)

project(AmigaAceplay VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

add_executable(AmigaAceplay main.c)

target_link_libraries(AmigaAceplay -lace)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)