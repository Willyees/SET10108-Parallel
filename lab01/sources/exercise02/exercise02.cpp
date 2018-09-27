#include <iostream>
#include <thread>
#include <random>
#include <chrono>
#include <vector>

//pass indexes of where to start to work on the matrix and the size to work on. result will hold the result calculations
void multiply_matrix(size_t start_index, size_t size, std::vector<std::vector<int>>& matrix1, std::vector<int>& matrix2, std::vector<int>& result) {
	size_t end = start_index + size;
	int total = 0;
	
	for (auto i = start_index; i < end; ++i){
		for (int j = 0; j < 1000; ++j){
			total += matrix1[i][j] * matrix2[j];
		}
		//store the total in the correct result address utilizing current index i
		result[i] = total;
		total = 0;
	}
}

int main() {
	int stop;
	
	//matrix 1000*1000
	std::vector<std::vector<int>> matrix(1000);
	//matrix 1000*1
	std::vector<int> vi;
	//result is fixed size (1000) because matrix1000*1000 * matrix1000*1 will create a matrix1000*1
	std::vector<int> result(1000);
	

	std::random_device r;
	std::default_random_engine e(r());
	//random vals into the matrices
	for (int i = 0; i < 1000; ++i)
		vi.push_back(e());
	for (int i = 0; i < 1000; ++i) {
		for (int j = 0; j < 1000; ++j) {
			matrix[i].push_back(e());
		}
	}
	size_t start_index = 0;
	//start off threads utilizing power of 2 (2^0 up to 2^6)
	for (size_t pow_thread = 0; pow_thread < 7; ++pow_thread) {
		unsigned max_thread = std::pow(2, pow_thread);
		std::cout << "Threads: " << max_thread << std::endl;
		std::vector<std::thread> threads;
		//calculate the size of the matrix 1000*1 that a each thread will need to work on
		size_t size = 1000 / max_thread;
		auto start_time = std::chrono::system_clock::now();
		for (size_t i = 0; i < max_thread; ++i) {
			threads.push_back(std::thread(multiply_matrix, start_index, size, matrix, vi, result));
			start_index += size;
		}
		//wait for all threads to finish
		for (auto& t : threads)
			t.join();
		auto end_time = std::chrono::system_clock::now();
		auto total_time = end_time - start_time;
		std::cout << "Total time: " << std::chrono::duration_cast<std::chrono::milliseconds>(total_time).count() << std::endl;
		start_index = 0;
	}
	std::cin >> stop;
	return 0;
}

