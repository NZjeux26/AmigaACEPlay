# Install script for directory: /home/nzjeux/local/AmigaACEPlay/deps/ace

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/nzjeux/local/amiga/usr")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/objects-Debug/ace" TYPE FILE FILES
    "src/ace/managers/audio.c.obj"
    "src/ace/managers/blit.c.obj"
    "src/ace/managers/bob.c.obj"
    "src/ace/managers/copper.c.obj"
    "src/ace/managers/game.c.obj"
    "src/ace/managers/joy.c.obj"
    "src/ace/managers/key.c.obj"
    "src/ace/managers/log.c.obj"
    "src/ace/managers/memory.c.obj"
    "src/ace/managers/mouse.c.obj"
    "src/ace/managers/ptplayer.c.obj"
    "src/ace/managers/rand.c.obj"
    "src/ace/managers/sprite.c.obj"
    "src/ace/managers/state.c.obj"
    "src/ace/managers/system.c.obj"
    "src/ace/managers/timer.c.obj"
    "src/ace/managers/viewport/camera.c.obj"
    "src/ace/managers/viewport/scrollbuffer.c.obj"
    "src/ace/managers/viewport/simplebuffer.c.obj"
    "src/ace/managers/viewport/tilebuffer.c.obj"
    "src/ace/utils/bitmap.c.obj"
    "src/ace/utils/bmframe.c.obj"
    "src/ace/utils/chunky.c.obj"
    "src/ace/utils/custom.c.obj"
    "src/ace/utils/dir.c.obj"
    "src/ace/utils/extview.c.obj"
    "src/ace/utils/file.c.obj"
    "src/ace/utils/font.c.obj"
    "src/ace/utils/palette.c.obj"
    "src/ace/utils/sprite.c.obj"
    "src/ace/utils/string.c.obj"
    "src/ace/utils/tag.c.obj"
    "src/fixmath/fix16.c.obj"
    "src/fixmath/fix16_exp.c.obj"
    "src/fixmath/fix16_sqrt.c.obj"
    "src/fixmath/fix16_str.c.obj"
    "src/fixmath/fix16_trig.c.obj"
    "src/fixmath/fract32.c.obj"
    "src/fixmath/uint32.c.obj"
    FILES_FROM_DIR "/home/nzjeux/local/AmigaACEPlay/build/ace/CMakeFiles/ace.dir/")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ace" TYPE FILE FILES
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/macros.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/types.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ace/generic" TYPE FILE FILES
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/generic/main.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/generic/screen.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ace/utils" TYPE FILE FILES
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/utils/bitmap.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/utils/bmframe.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/utils/chunky.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/utils/custom.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/utils/dir.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/utils/endian.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/utils/extview.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/utils/file.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/utils/font.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/utils/mini_std.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/utils/palette.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/utils/sprite.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/utils/string.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/utils/tag.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ace/managers" TYPE FILE FILES
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/managers/audio.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/managers/blit.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/managers/bob.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/managers/copper.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/managers/game.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/managers/joy.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/managers/key.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/managers/log.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/managers/memory.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/managers/mouse.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/managers/ptplayer.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/managers/rand.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/managers/sprite.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/managers/state.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/managers/system.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/managers/timer.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ace/managers/viewport" TYPE FILE FILES
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/managers/viewport/camera.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/managers/viewport/scrollbuffer.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/managers/viewport/simplebuffer.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/ace/managers/viewport/tilebuffer.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/fixmath" TYPE FILE FILES
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/fixmath/fix16.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/fixmath/fix16_trig_sin_lut.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/fixmath/fixmath.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/fixmath/fract32.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/fixmath/int64.h"
    "/home/nzjeux/local/AmigaACEPlay/deps/ace/include/fixmath/uint32.h"
    )
endif()

