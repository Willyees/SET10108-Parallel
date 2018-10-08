#include <math.h>
#include <thread>
#include <omp.h>
#include <iostream>

double f(unsigned int i) {
	//calcualte start and end values
	auto start = i * (i + 1) / 2;
	auto end = start + i;
	// Declare return value
	auto result = 0.0;

	// Loop for number of iterations, calculating sin
	for (auto j = start; j <= end; ++j)
		result += sin(j);

	// Return result
	return result;
}

int main() {
	// Get number of hardware threads
	auto thread_count = std::thread::hardware_concurrency();
	// Define number of iterations to calculate
	auto n = static_cast<size_t>(pow(2, 14));
	// Declare sum value
	auto sum = 0.0;

	//get start time
	auto start = std::chrono::system_clock::now();
#pragma omp parallel for num_threads(thread_count) reduction(+:sum) schedule(static, 2)
	//using many threads as thread_count, running them in total as many times as iterations of the loop. with static will run them 
	//round robin (thread1 has i = 1, then turn of thread2) static,2 will give the first the 2i to first thread (thread1 i = 1, thread 1 i =2, thread 2 i =3 thread 2 i = 4 ..)
	for (auto i = 0; i <= 64; ++i) {
		sum += f(i);
	}
	//get end time
	auto end = std::chrono::system_clock::now();
	auto total = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << "Total time: " << total << "ms" << std::endl;

//#pragma omp parallel for num_threads(thread_count) reduction(+:sum) schedule(static, 2)
//	for (auto i = 0; i <= n; ++i) {
//		sum += f(i);
//	}
//	//get end time
//	end = std::chrono::system_clock::now();
//	total = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//	std::cout << "Total time: " << total << "ms" << std::endl;
//
//#pragma omp parallel for num_threads(thread_count) reduction(+:sum) 
//	for (auto i = 0; i <= n; ++i) {
//		sum += f(i);
//	}
//	//get end time
//	end = std::chrono::system_clock::now();
//	total = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//	std::cout << "Total time: " << total << "ms" << std::endl;
	std::cin.get();
}