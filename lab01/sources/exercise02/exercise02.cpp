#include <iostream>
#include <thread>
#include <random>
#include <chrono>
#include <vector>

typedef std::vector<int>::iterator vec_it;
typedef std::vector<std::vector<int>>::iterator vec_vec_it;

void multiply_matrix(size_t start_index, size_t size, std::vector<std::vector<int>>& matrix1, std::vector<int>& matrix2, std::vector<int>& result) {
	size_t end = start_index + size;
	int total = 0;
	
	for (auto i = start_index; i < end; ++i){
		for (int j = 0; j < 1000; ++j){
			total += matrix1[i][j] * matrix2[j];
		}
		result[i] = total;
		total = 0;
	}
}

int main() {
	int stop;
	
	std::vector<std::vector<int>> matrix(1000);
	std::vector<int> vi;
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
	//start off threads
	for (size_t pow_thread = 0; pow_thread < 7; ++pow_thread) {
		unsigned max_thread = std::pow(2, pow_thread);
		std::cout << "Threads: " << max_thread << std::endl;
		std::vector<std::thread> threads;
		size_t size = 1000 / max_thread;
		auto start_time = std::chrono::system_clock::now();
		for (size_t i = 0; i < max_thread; ++i) {
			threads.push_back(std::thread(multiply_matrix, start_index, size, matrix, vi, result));
			start_index += size;
		}
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

