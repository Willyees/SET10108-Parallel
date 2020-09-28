find_package(MPI)
include_directories(${MPI_CXX_INCLUDE_PATH})

file(GLOB_RECURSE SOURCES mandelbrot_mpi/*.h mandelbrot_mpi/*.cpp)
add_executable(madelbrot_mpi ${SOURCES})
target_link_libraries(madelbrot_mpi ${MPI_CXX_LIBRARIES})

###

file(GLOB_RECURSE SOURCES parallel_sort/*.cpp parallel_sort/*.h)
add_executable(parallel_sort ${SOURCES})
target_link_libraries(parallel_sort ${MPI_CXX_LIBRARIES})

###

file(GLOB_RECURSE SOURCES trapezoidal_rule_mpi/*.cpp trapezoidal_rule_mpi/*.h)
add_executable(trapezoidal_rule_mpi ${SOURCES})
target_link_libraries(trapezoidal_rule_mpi ${MPI_CXX_LIBRARIES})

###

file(GLOB_RECURSE SOURCES latency/*.cpp latency/*.h)
add_executable(latency_mpi ${SOURCES})
target_link_libraries(latency_mpi ${MPI_CXX_LIBRARIES})

###