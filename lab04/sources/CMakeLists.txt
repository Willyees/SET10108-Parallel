file(GLOB_RECURSE SOURCES "aligned_memory/*.cpp" "aligned_memory/*.h")
add_executable(aligned_memory ${SOURCES})

file(GLOB_RECURSE SOURCES "simd_operations/*.cpp" "simd_operations/*.h")
add_executable(simd_operations ${SOURCES})

file(GLOB_RECURSE SOURCES "normalize_vector/*.cpp" "normalize_vector/*.h")
add_executable(normalize_vector ${SOURCES})