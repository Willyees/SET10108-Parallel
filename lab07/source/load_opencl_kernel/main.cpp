#define __CL_ENABLE_EXCEPTIONS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <CL/cl.hpp>

using namespace std;

constexpr int ELEMENTS = 2048;
constexpr size_t DATA_SIZE = sizeof(int) * ELEMENTS;

int main(int argc, char** agrv) {
	cout << "Starting" << endl;
	//initialise memory
	array<int, ELEMENTS> A;
	array<int, ELEMENTS> B;
	array<int, ELEMENTS> C;
	for (size_t i = 0; i < ELEMENTS; ++i) {
		A[i] = B[i] = i;
	}
	try {
		//get platforms
		vector<cl::Platform> platforms;
		cl::Platform::get(&platforms);

		//assume only one platform, get GPU devices
		vector<cl::Device> devices;
		platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);
		
		cout << devices[0].getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>()[0] << endl;

		cout << devices[0].getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>() << endl;

		//just to test, print out device 0 name
		cout << devices[0].getInfo<CL_DEVICE_NAME>() << endl;

		//create context with these devices
		cl::Context context(devices);

		//create a command queque for device 0
		cl::CommandQueue queue(context, devices[0]);

		//create the buffers
		cl::Buffer bufA(context, CL_MEM_READ_ONLY, DATA_SIZE);
		cl::Buffer bufB(context, CL_MEM_READ_ONLY, DATA_SIZE);
		cl::Buffer bufC(context, CL_MEM_WRITE_ONLY, DATA_SIZE);
		
		//copy data to the GPU
		queue.enqueueWriteBuffer(bufA, CL_TRUE, 0, DATA_SIZE, &A);
		queue.enqueueWriteBuffer(bufB, CL_TRUE, 0, DATA_SIZE, &B);

		//read in kernel source
		ifstream file("vec-add.cl");
		if (!file)
			cout << "Error initializing kernel" << endl;
		string code(istreambuf_iterator<char>(file), (istreambuf_iterator<char>()));

		//create program 
		cl::Program::Sources source(1, make_pair(code.c_str(), code.length() + 1));
		cl::Program program(context, source);
	
		//build program for devices
		program.build(devices);

		//create the kernel
		cl::Kernel vecadd_kernel(program, "vecadd");

		//set kernel arguments
		vecadd_kernel.setArg(0, bufA);
		vecadd_kernel.setArg(1, bufB);
		vecadd_kernel.setArg(2, bufC);

		//execute the kernel
		cl::NDRange global(ELEMENTS);
		cl::NDRange local(256);
		queue.enqueueNDRangeKernel(vecadd_kernel, cl::NullRange, global, local);

		//copy result back
		queue.enqueueReadBuffer(bufC, CL_TRUE, 0, DATA_SIZE, &C);

		//test that the results are correct
		for (int i = 0; i < 2048; ++i) {
			if (C[i] != i + i)
				cout << "Error: " << i << endl;
			cout << C[i] << endl;
			
		}
		cout << "Finished" << endl;
}
	catch (cl::Error error) {
		cout << error.what() << "(" << error.err() << ")" << endl;
	}


	return 0;
}