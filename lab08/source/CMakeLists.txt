###init

file(GLOB_RECURSE SOURCES cuda_init/*.h cuda_init/*.cpp cuda_init/*.cu)
find_package(CUDA 9.0 REQUIRED)
add_executable(cuda_init ${SOURCES})
include_directories(${CUDA_TOOLKIT_ROOT_DIR}/include)
#target_link_libraries(cuda_init ${CUDA_TOOLKIT_ROOT_DIR}/lib/x64/cudart.lib)
target_link_libraries(cuda_init ${CUDA_LIBRARIES})

###vec add

file(GLOB_RECURSE SOURCES vec_add/*.h vec_add/*.cpp vec_add/*.cu)
CUDA_ADD_EXECUTABLE(vec_add ${SOURCES})
#target_compile_features(vec_add PUBLIC cxx_std_11)
include_directories(${CUDA_TOOLKIT_ROOT_DIR}/include)
target_link_libraries(cuda_init ${CUDA_LIBRARIES})
