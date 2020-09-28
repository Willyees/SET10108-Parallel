## find cuda and set include directory for opencl

find_package(CUDA)
include_directories(${CUDA_TOOLKIT_ROOT_DIR}/include)

###

file(GLOB_RECURSE SOURCES opencl_init/*.h opencl_init/*.cpp)
add_executable(opencl_init ${SOURCES})

target_link_libraries(opencl_init ${CUDA_TOOLKIT_ROOT_DIR}/lib/x64/OpenCL.lib)

###

file(GLOB_RECURSE SOURCES opencl_info/*.h opencl_info/*.cpp)

add_executable(opencl_info ${SOURCES})

target_link_libraries(opencl_info ${CUDA_TOOLKIT_ROOT_DIR}/lib/x64/OpenCL.lib)

###

file(GLOB_RECURSE SOURCES load_opencl_kernel/*.h load_opencl_kernel/*.cpp load_opencl_kernel/*.cl)

add_executable(load_opencl_kernel ${SOURCES})

target_link_libraries(load_opencl_kernel ${CUDA_TOOLKIT_ROOT_DIR}/lib/x64/OpenCL.lib)

###

file(GLOB_RECURSE SOURCES matrix_multiplication_opencl/*.h matrix_multiplication_opencl/*.cpp matrix_multiplication_opencl/*.cl)

add_executable(matrix_multiplication_opencl ${SOURCES})

target_link_libraries(matrix_multiplication_opencl ${CUDA_TOOLKIT_ROOT_DIR}/lib/x64/OpenCL.lib)