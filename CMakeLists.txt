cmake_minimum_required(VERSION 3.16)

set(CMAKE_SYSTEM_NAME AmigaOS)
set(CMAKE_SYSTEM_VERSION 1)

project(AmigaAceplay VERSION 0.1.0 LANGUAGES C)

add_executable(AmigaAceplay main.c)

add_subdirectory(deps/ace ace)
target_link_libraries(AmigaAceplay ace)

