cmake_minimum_required(VERSION 3.14.0)
project(AmigaAceplay LANGUAGES ASM C)

set(CMAKE_C_STANDARD 17)
file(GLOB_RECURSE SOURCES src/*.c main.c)

if(ELF2HUNK)
	set(AMIGAACEPLAY_EXECUTABLE Amigaaceplay.elf)
	set(AMIGAACEPLAY_OUTPUT_EXECUTABLE Amigaaceplay.exe)
	add_executable(${AMIGAACEPLAY_EXECUTABLE} ${SOURCES})
	
	target_link_libraries(${AMIGAACEPLAY_EXECUTABLE} -Wl,-Map=Amigaaceplay.map)
	
	add_custom_command(
		TARGET ${AMIGAACEPLAY_EXECUTABLE} POST_BUILD
		COMMAND ${ELF2HUNK} ${AMIGAACEPLAY_EXECUTABLE} ${AMIGAACEPLAY_OUTPUT_EXECUTABLE}
	)
	add_custom_command(
		TARGET ${AMIGAACEPLAY_EXECUTABLE} POST_BUILD
		COMMAND ${OBJDUMP} --disassemble -S ${AMIGAACEPLAY_EXECUTABLE} > Amigaaceplay.s
	)
else()
	SET(AMIGAACEPLAY_EXECUTABLE AmigaAceplay)
	SET(AMIGAACEPLAY_OUTPUT_EXECUTABLE AmigaAceplay)
	
	add_executable(${AMIGAACEPLAY_EXECUTABLE} ${SOURCES})
endif()


add_subdirectory(deps/ace ace)

# Version stuff
string(TIMESTAMP YEAR "%y")
string(TIMESTAMP DAY "%d")
string(TIMESTAMP MONTH "%m")
MATH(EXPR VER_MAJOR "0 + ${YEAR}")
MATH(EXPR VER_MINOR "0 + ${MONTH}")
MATH(EXPR VER_FIX "0 + ${DAY}")
set(VERSION "${VER_MAJOR}.${VER_MINOR}.${VER_FIX}")

target_compile_definitions(${AMIGAACEPLAY_EXECUTABLE} PRIVATE AMIGAACEPLAY_VERSION="${VERSION}")
include_directories(${SDL2_INCLUDE_DIRS} include/)
target_link_libraries(${AMIGAACEPLAY_EXECUTABLE} ace )  
target_compile_options(${AMIGAACEPLAY_EXECUTABLE} PUBLIC -Wall)


if(GAME_DEBUG)
	target_compile_definitions(${AMIGAACEPLAY_EXECUTABLE} PRIVATE GAME_DEBUG)
	target_compile_definitions(ace PUBLIC ACE_DEBUG_ALL ACE_DEBUG_UAE)
endif()



set(RES_DIR ${CMAKE_CURRENT_LIST_DIR}/res)
set(DATA_DIR ${CMAKE_CURRENT_BINARY_DIR}/data)
set(GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)


# Generating ZIP
set(AMIGAACEPLAY_ZIP_NAME "${CMAKE_PROJECT_NAME} ${VER_MAJOR}_${VER_MINOR}_${VER_FIX}")
add_custom_target(generateZip COMMAND
	${CMAKE_COMMAND} -E tar "cf" "${AMIGAACEPLAY_ZIP_NAME}.zip" --format=zip
	"${CMAKE_CURRENT_BINARY_DIR}/${AMIGAACEPLAY_OUTPUT_EXECUTABLE}" "${CMAKE_CURRENT_BINARY_DIR}/myacefont.fnt"
	COMMENT "Generating ${AMIGAACEPLAY_ZIP_NAME}.zip"
)
# Generating ADF
set(ADF_DIR "${CMAKE_CURRENT_BINARY_DIR}/adf")
add_custom_target(generateAdf
	COMMAND ${CMAKE_COMMAND} -E make_directory "${ADF_DIR}/s"
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${AMIGAACEPLAY_OUTPUT_EXECUTABLE}" "${ADF_DIR}"
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/myacefont.fnt" "${ADF_DIR}"
	
	COMMAND ${CMAKE_COMMAND} -E echo "${AMIGAACEPLAY_OUTPUT_EXECUTABLE}" > "${ADF_DIR}/s/startup-sequence"
	COMMAND exe2adf -l ${CMAKE_PROJECT_NAME} -a "${CMAKE_PROJECT_NAME}.adf" -d ${ADF_DIR}
	COMMAND ${CMAKE_COMMAND} -E rm -rf "${ADF_DIR}"
	COMMENT "Generating ADF file"
)

