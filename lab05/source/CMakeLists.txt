find_library(MSMPI msmpi "C:/Program Files (x86)/Microsoft SDKs/MPI/Lib/x64")

if(MPI_COMPILE_FLAGS)
  set_target_properties(hello_mpi PROPERTIES
    COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
  set_target_properties(hello_mpi PROPERTIES
    LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()

add_executable(hello_mpi hello_mpi/hello_mpi.cpp)
target_include_directories(hello_mpi PUBLIC "C:/Program Files (x86)/Microsoft SDKs/MPI/Include")
target_link_libraries(hello_mpi ${MSMPI})

#part2

add_executable(send_receive send_receive/main.cpp)
target_include_directories(send_receive PUBLIC "C:/Program Files (x86)/Microsoft SDKs/MPI/Include")
target_link_libraries(send_receive ${MSMPI})

#part3
add_executable(map_reduce map_reduce/main.cpp)
target_include_directories(map_reduce PUBLIC "C:/Program Files (x86)/Microsoft SDKs/MPI/Include")
target_link_libraries(map_reduce ${MSMPI})

#part4
add_executable(scatter_gather scatter_gather/main.cpp)
target_include_directories(scatter_gather PUBLIC "C:/Program Files (x86)/Microsoft SDKs/MPI/Include")
target_link_libraries(scatter_gather ${MSMPI})

#part5
add_executable(broadcast broadcast/main.cpp)
target_include_directories(broadcast PUBLIC "C:/Program Files (x86)/Microsoft SDKs/MPI/Include")
target_link_libraries(broadcast ${MSMPI})