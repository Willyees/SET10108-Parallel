#define __CL_ENABLE_EXCEPTIONS

#include <CL/cl.hpp>
#include <vector>
#include <iostream>

int main(int argc, char** argv) {
	try{
		//get platforms
		std::vector<cl::Platform> platforms;
		cl::Platform::get(&platforms);

		//iterate through each platform
		for (auto &p : platforms) {
			//print out platfrom name
			std::cout << "*****" << std::endl;
			std::cout << p.getInfo<CL_PLATFORM_NAME>() << std::endl;

			//get all devices for the platform
			std::vector<cl::Device> devices;
			p.getDevices(CL_DEVICE_TYPE_ALL, &devices);

			//iterate through all the devices
			for (auto &d : devices) {
				std::cout << std::endl;
				std::cout << d.getInfo<CL_DEVICE_NAME>() << std::endl;
				std::cout << "vendor: " << d.getInfo<CL_DEVICE_VENDOR>() << std::endl;
				std::cout << "max compute: " << d.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << std::endl;
				std::cout << "max memory: " << d.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>() / (1024 * 1024) << "MB" << std::endl;
				std::cout << "clock freqency: " << d.getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>() << std::endl;
				std::cout << "available: " << (d.getInfo<CL_DEVICE_AVAILABLE>() ? "True" : "False") << std::endl;

			}
			std::cout << std::endl;
			std::cout << "*****" << std::endl;
		}
	}
	catch (cl::Error error) {
		std::cout << error.what() << "(" << error.err() << ")" << std::endl;
	}

	return 0;
}