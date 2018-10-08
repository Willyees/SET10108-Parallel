#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include "guarded_obj.h"

using std::cout;
using std::endl;

constexpr unsigned int NUM_ITERATIONS = 1000000;
unsigned int NUM_THREADS = std::thread::hardware_concurrency();

void task(std::shared_ptr<guarded> g) {
	// Increment guarded object NUM_ITERATIONS times
	for (unsigned int i = 0; i < NUM_ITERATIONS; ++i)
		g->increment();
}

int main(int argc, char** argv) {
	int stop;

	//Create guarded object
	auto g = std::make_shared<guarded>();
	//Create threads
	std::vector<std::thread> threads;
	for (unsigned int i = 0; i < NUM_THREADS; ++i)
		threads.push_back(std::thread(task, g));
	//join threads
	for (auto& t : threads)
		t.join();
	//Dispaly value stored in guarded object
	cout << "Value = " << g->get_value() << endl;

	std::cin >> stop;
	return 0;
}