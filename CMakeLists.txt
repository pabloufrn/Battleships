cmake_minimum_required(VERSION 3.6)
project (BATTLESHIPS VERSION 1.0.0 LANGUAGES CXX )

#=== FINDING PACKAGES ===#

#--------------------------------
# This is for old cmake versions
set (CMAKE_CXX_STANDARD 11)
#--------------------------------

#=== SETTING VARIABLES ===#
# Compiling flags
if (WIN32)
    set( GCC_COMPILE_FLAGS "-Wall -O3 -s" )
else ()
	set( GCC_COMPILE_FLAGS "-Wall -O3 -s -no-pie" )
endif (WIN32)

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS} ${PREPROCESSING_FLAGS}" )

# Set relative library path
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "$ORIGIN/lib")

# Set output Paths
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

#=== Main App ===

# prepare file and directories
include_directories( include . )
link_directories( lib )
file(GLOB SOURCES "src/*.cpp")

# prepare targets
add_executable(battleships ${SOURCES})
target_link_libraries(battleships sfml-system sfml-window sfml-graphics) 
