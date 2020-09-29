# SET10108-Parallel
Napier University 2018/2019. Lab repository

### Collection of laboratory exercises aimed at showing the understanding of concurrency computation. 

Multiple threads (c++ std::thread), mutex, atomic, OpenMP, SIMD, MPI, OpenCL, CUDA were utilized to achieve parallelisation.
Lab1: 
starting off multiple threads and splitting the workload amongst them. The aim of the lab is to show the time gained by splitting the work, rather than utilizing only 1 single thread.

Lab2:
Illustrate the problem of race-conditions and trying to avoid id.
Methods used: locking and unlocking mutex guards, condition variables, atomic data types, futures and promises.
Exercises are focused on the Mandelbrot algorithm, experimenting different techniques and splitting methods, calculating the time taken.

Lab3:
Utilized OpenMP and pre-processor commands to easily achieve concurrency.
Exercises focused again on Mandelbrot, calculating the benefit of using concurrency with OpenMP

Lab4:
Utilized processors support for SIMD operations available and their limitations.
Exercises compared SIMD with previous techniques.

Lab5:
Utilized Microsoft MPI to create a distributed parallelised environment. Additionally, limitation of this method were explored

Lab6:
Applied MPI to previous exercises and compared them. Using latency and bandwidth to evaluate performances

Lab7:
Moved to GPU data parallelisation using OpenCL. Loading and running kernels to be able to receive different share of data and concurrently process it

Lab8:
Utilized CUDA on Nvidia hardware. Obtaining concurrency using CUDA kernels
