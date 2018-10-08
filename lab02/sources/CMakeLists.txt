# CMakeList.txt : CMake project for Lab02, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
file(GLOB_RECURSE SOURCES "mutex/*.cpp" "mutex/*.h")
add_executable (mutex ${SOURCES})

file(GLOB_RECURSE SOURCES "condition_var/*.cpp" "condition_var/*.h")
add_executable (condition_var ${SOURCES})

file(GLOB_RECURSE SOURCES "guarded_obj/*.cpp" "guarded_obj/*.h")
add_executable (guarded_obj ${SOURCES})

file(GLOB_RECURSE SOURCES "thread_safe_stack/*.cpp" "thread_safe_stack/*.h")
add_executable (thread_safe_stack ${SOURCES})

file(GLOB_RECURSE SOURCES "exercise01_montecarlo/*.cpp" "exercise01_montecarlo/*.h")
add_executable (montecarlo_revisited ${SOURCES})

file(GLOB_RECURSE SOURCES "exercise01-3_deadlock/*.cpp" "exercise01-3_deadlock/*.h")
add_executable (deadlock ${SOURCES})

file(GLOB_RECURSE SOURCES "atomic/*.cpp" "atomic/*.h")
add_executable (atomic ${SOURCES})

file(GLOB_RECURSE SOURCES "atomic_flag/*.cpp" "atomic_flag/*.h")
add_executable (atomic_flag ${SOURCES})

file(GLOB_RECURSE SOURCES "exercise02-2_montecarlo_atomic/*.cpp" "exercise02-2_montecarlo_atomic/*.h")
add_executable (montecarlo_atomic ${SOURCES})

file(GLOB_RECURSE SOURCES "exercise03-2_mandelbrot/*.cpp" "exercise03-2_mandelbrot/*.h")
find_library(FreeImage FreeImage exercise03-2_mandelbrot/)
add_executable (mandelbrot ${SOURCES})
target_link_libraries(mandelbrot ${FreeImage})
target_include_directories(mandelbrot PUBLIC ${FreeImage})
configure_file(${CMAKE_SOURCE_DIR}/sources/exercise03-2_mandelbrot/FreeImage.dll ${CMAKE_CURRENT_BINARY_DIR}/FreeImage.dll COPYONLY)
# TODO: Add tests and install targets if needed.
