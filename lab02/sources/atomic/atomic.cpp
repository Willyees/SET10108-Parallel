#include <atomic>
#include <memory>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

std::mutex mut;

void increment_atomic(std::shared_ptr<std::atomic<int>> value) {
	for (unsigned i = 0; i < 1000000; ++i) {
		++(*value);
	}
}

void increment_mutex(std::shared_ptr<std::atomic<int>> value) {
	for (unsigned i = 0; i < 1000000; ++i) {
		mut.lock();
		*value = *value + 1;
		mut.unlock();
	}
}


int main(int argc, char **argv)
{
	int stop;
	// Create a shared int value
	auto value = std::make_shared<std::atomic<int>>();

	// Create number of threads hardware natively supports
	auto num_threads = std::thread::hardware_concurrency();
	std::vector<std::thread> threads;
	//atomic test
	auto start_time = std::chrono::system_clock::now();
	for (unsigned int i = 0; i < num_threads; ++i)
		threads.push_back(std::thread(increment_atomic, value));

	// Join the threads
	for (auto &t : threads)
		t.join();
	auto end_time = std::chrono::system_clock::now();
	auto total_time = end_time - start_time;
	std::cout << "Total time atomic: " << total_time.count();
	std::cout << "Value = " << *value << std::endl;
	*value = 0;
	threads.clear();
	//mutex test
	start_time = std::chrono::system_clock::now();
	for (unsigned int i = 0; i < num_threads; ++i)
		threads.push_back(std::thread(increment_mutex, value));

	// Join the threads
	for (auto &t : threads)
		t.join();

	end_time = std::chrono::system_clock::now();
	total_time = end_time - start_time;
	std::cout << "Total time mutex: " << total_time.count();
	// Display the value
	std::cout << "Value = " << *value << std::endl;

	std::cin >> stop;
}