cmake_minimum_required (VERSION 2.6)
project (Lab01)
#Main output directory
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Ouput all DLLs from all libs into main build folder
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#execise 1
file(GLOB_RECURSE SOURCES exercise01/*.cpp exercise01/*.h)
add_executable(Exercise01 ${SOURCES})

#exercise 2
file(GLOB_RECURSE SOURCES exercise02/*.cpp exercise02/*.h)
add_executable(Exercise02 ${SOURCES})

#exercise 3
file(GLOB_RECURSE SOURCES exercise03/*.cpp exercise03/*.h)
add_executable(Exercise03 ${SOURCES})

#exercise 4
file(GLOB_RECURSE SOURCES exercise04/*.cpp exercise04/*.h)
add_executable(Exercise04 ${SOURCES})

#lab01
file(GLOB_RECURSE SOURCES lab01/*.cpp lab01/*.h)
add_executable(Lab01 ${SOURCES})

