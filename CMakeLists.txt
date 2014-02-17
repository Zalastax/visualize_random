cmake_minimum_required(VERSION 2.6)
project(VISUALIZE_RANDOM_DISTRIBUTION)

set(EXECUTABLE_NAME "Visualize_random_distribution")

###############################################################
# Files to include
###############################################################

include_directories(${CMAKE_SOURCE_DIR}/include)

FILE(GLOB_RECURSE ALL_SOURCE_FILES 
	"${CMAKE_SOURCE_DIR}/SDL2/*.h"
	"${CMAKE_SOURCE_DIR}/source/*.h"
	"${CMAKE_SOURCE_DIR}/source/*.c"
)

SET( PROJ_SOURCES
	${ALL_SOURCE_FILES}
	)

###############################################################
# Compiler settings
###############################################################

# Initialize CXXFLAGS.
set(CMAKE_C_FLAGS			"-Wall -std=c99 -pedantic -Werror -Wextra")
set(CMAKE_C_FLAGS_DEBUG		"-O0 -g")
set(CMAKE_C_FLAGS_MINSIZEREL		"-Os -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE		"-O4 -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO	"-O2 -g")


IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./Debug")
ELSEIF(CMAKE_BUILD_TYPE MATCHES RELEASE)
   SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./Release")
ELSE()
   SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./Output")
ENDIF()
add_executable(${EXECUTABLE_NAME} ${PROJ_SOURCES})
target_link_libraries(${EXECUTABLE_NAME} ${LIB})
target_link_libraries(${EXECUTABLE_NAME} -lmingw32 -lSDL2main -lSDL2 )
