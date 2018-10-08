// Lab02.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

std::mutex mut;
unsigned iterations = 1000000;
//automatically mange mutex using a lock guard: destroyed at each iterations of loop
//void increment(std::shared_ptr<int> value) {
//	for (unsigned i = 0; i < 1000000; ++i) {
//		std::lock_guard<std::mutex> lock(mut);
//		*value = *value + 1;
//		
//	}
//}
void increment(std::shared_ptr<int> value, unsigned iterations) {
	for (unsigned int i = 0; i < iterations; ++i) {
		mut.lock();
		*value = *value + 1;
		//std::cout << "Thread num: " << std::this_thread::get_id() << std::endl;
		mut.unlock();
	} 
	
}
class test {
public:
	~test() {
		std::cout << "destroyed" << std::endl;
	}
};
int main()
{
	int stop;
	auto value = std::make_shared<int>(0);
	auto max_threads = std::thread::hardware_concurrency();
	
	for (unsigned pow_thread = 0; pow_thread < (log(max_threads)/log(2));++pow_thread){
		std::vector<std::thread> threads;
		unsigned total_threads = pow(2, pow_thread);
		std::cout << "Total treads: " << total_threads << std::endl;
		auto start = std::chrono::system_clock::now();
		for (unsigned int i = 0; i < total_threads; ++i)
			threads.push_back(std::thread(increment, value, iterations / total_threads));

		for (auto& t : threads)
			t.join();
		auto end = std::chrono::system_clock::now();
		auto total_time = end - start;
		std::cout << "Time: " << total_time.count() << std::endl;	
		std::cout << "Value = " << *value << std::endl;
		*value = 0;
	}
	auto start = std::chrono::system_clock::now();
	for (unsigned int i = 0; i < iterations; ++i) {
		
		*value = *value + 1;
		//std::cout << "Thread num: " << std::this_thread::get_id() << std::endl;
		
	}
	auto end = std::chrono::system_clock::now();
	auto total_time = end - start;
	std::cout << "Time: " << total_time.count() << std::endl;
	std::cout << "Value = " << *value << std::endl;
	std::cin >> stop;
	return 0;
}
