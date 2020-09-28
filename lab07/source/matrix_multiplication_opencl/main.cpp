#define __CL_ENABLE_EXCEPTIONS

#include <iostream>
#include <vector>
#include <CL/cl.hpp>
#include <array>
#include <fstream>

using namespace std;

constexpr int ELEMENTS = 1024;
constexpr size_t MATRIX_SIZE = sizeof(int) * ELEMENTS;
constexpr size_t width_A = 32;
constexpr size_t width_B = 32;
constexpr size_t height_A = 32;
constexpr size_t height_B = 32;

int main(int argc, char** argv) {

	array<int, ELEMENTS> array_A;
	array<int, ELEMENTS> array_B;
	array<int, ELEMENTS> array_C;

	//store elemets into arrays
	for (size_t i = 0; i < ELEMENTS; ++i) {
		array_A[i] = array_B[i] = i;
	}

	try {
		//get the platforms
		vector<cl::Platform> platforms;
		cl::Platform::get(&platforms);

		
		vector<cl::Device> devices;
		platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);

		// Just to test, print out device 0 name
		cout << devices[0].getInfo<CL_DEVICE_NAME>() << endl;
	
		cl::Context context(devices);

		cl::CommandQueue queue(context, devices[0]);
		
		cl::Buffer buff_output(context, CL_MEM_WRITE_ONLY, MATRIX_SIZE);
		cl::Buffer buff_A(context, CL_MEM_WRITE_ONLY, MATRIX_SIZE);
		cl::Buffer buff_B(context, CL_MEM_WRITE_ONLY, MATRIX_SIZE);
		
		//copy data to GPU
		queue.enqueueWriteBuffer(buff_A, CL_TRUE, 0, ELEMENTS, &array_A);
		queue.enqueueWriteBuffer(buff_B, CL_TRUE, 0, ELEMENTS, &array_B);

		//read in kernel source
		ifstream file("matrix_mult.cl");
		if (!file)
			cout << "Error in loading file" << endl;
		string code(istreambuf_iterator<char>(file), (istreambuf_iterator<char>()));

		//create program
		cl::Program::Sources source(1, make_pair(code.c_str(), code.length() + 1));
		cl::Program program(context, source);

		//build program for all devices
		program.build(devices);

		//create kernel
		cl::Kernel matrix_mult_kernel(program, "simple_multiply");

		//set kernel arguments
		matrix_mult_kernel.setArg(0, buff_A);
		matrix_mult_kernel.setArg(1, buff_B);
		matrix_mult_kernel.setArg(2, buff_output);

		//execute kernel
		cl::NDRange global(ELEMENTS);
		cl::NDRange local(width_A);
		queue.enqueueNDRangeKernel(matrix_mult_kernel, cl::NullRange, global, local);
		
		//copy results back
		queue.enqueueReadBuffer(buff_output, CL_TRUE, 0, ELEMENTS, &array_C);

		for (int i = 0; i < 1024; ++i)
			if (array_C[i] != width_A)
				cout << "Error: " << i << endl;

	}
	catch (cl::Error error) {
		cout << error.what() << "(" << error.err() << ")" << endl;
	}
	return 0;
}