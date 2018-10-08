include(FindOpenMP)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

add_executable(hello_world_omp hello_world_omp.cpp)

file(GLOB_RECURSE SOURCES "pi/*.cpp" "pi/*.h")
add_executable(pi ${SOURCES})

file(GLOB_RECURSE SOURCES "bubble_sort/*.cpp" "bubble_sort/*.h")
add_executable(bubble_sort ${SOURCES})

file(GLOB_RECURSE SOURCES "bubble_sort_parallel/*.cpp" "bubble_sort_parallel/*.h")
add_executable(bubble_sort_parallel ${SOURCES})

file(GLOB_RECURSE SOURCES "trapezoidal_curve/*.cpp" "trapezoidal_curve/*.h")
add_executable(trapezoidal_curve ${SOURCES})

file(GLOB_RECURSE SOURCES "scheduling/*.cpp" "scheduling/*.h")
add_executable(scheduling ${SOURCES})

file(GLOB_RECURSE SOURCES "fractal_omp/*.cpp" "fractal_omp/*.h")
add_executable(fractal_omp ${SOURCES})
find_library(FreeImage FreeImage fractal_omp/)
target_link_libraries(fractal_omp ${FreeImage})
target_include_directories(fractal_omp PUBLIC ${FreeImage})
configure_file(${CMAKE_SOURCE_DIR}/sources/fractal_omp/FreeImage.dll ${CMAKE_CURRENT_BINARY_DIR}/FreeImage.dll COPYONLY)