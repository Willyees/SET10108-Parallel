
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <vector>
#include <iostream>

using namespace std;

__global__ void vecadd(const int* A, const int* B, int* C) {
	//get block index
	unsigned int block_idx = blockIdx.x;
	//get thread index
	unsigned int thread_idx = threadIdx.x;
	//get the number of threads per block
	unsigned int block_dim = blockDim.x;
	//get the thread's unique ID (block_idx * block_dim)
	unsigned int idx = (block_idx * block_dim) + thread_idx;
	//add corresponding locations of A and B and store in C
	C[idx] = A[idx] + B[idx];
}

constexpr int ELEMENTS = 1024;

int main(int argc, char** argv) {

	//create host memory
	auto data_size = sizeof(int) * ELEMENTS;
	vector<int> A(ELEMENTS);
	vector<int> B(ELEMENTS);
	vector<int> C(ELEMENTS);

	//initialize input data
	for (size_t i = 0; i < ELEMENTS; ++i) {
		A[i] = B[i] = i;
	}

	//declare buffers
	int* buff_A, * buff_B, * buff_C;

	//initialize the buffers
	cudaMalloc((void**)&buff_A, data_size);
	cudaMalloc((void**)&buff_B, data_size);
	cudaMalloc((void**)&buff_C, data_size);

	cudaMemcpy(buff_A, &A[0], data_size, cudaMemcpyKind::cudaMemcpyHostToDevice);
	cudaMemcpy(buff_B, &B[0], data_size, cudaMemcpyKind::cudaMemcpyHostToDevice);

	//run kernel with one thread for each element
	//first value is number of blocks, secnod is threads per block. max 1024 threads per block
	vecadd<<<ELEMENTS / 1024, 1024>>>(buff_A, buff_B, buff_C);
	
	//wait for kernel to complete
	cudaDeviceSynchronize();

	//read output buffer back to the host
	cudaMemcpy(&C[0], buff_C, data_size, cudaMemcpyKind::cudaMemcpyDeviceToHost);
	
	for (auto element : C)
		cout << element << endl;

	//clean resources
	cudaFree(buff_A);
	cudaFree(buff_B);
	cudaFree(buff_C);
	return 0;
}