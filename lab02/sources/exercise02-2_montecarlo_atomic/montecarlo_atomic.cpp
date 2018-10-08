#include <iostream>
#include <atomic>
#include <random>
#include <thread>
#include <fstream>

using namespace std;
using namespace std::chrono;
//work in progress: can't use atomic double to add all the pi in each thread. Find out different way of doing usign atomic
void monte_carlo_pi(size_t iterations)
{
	// Seed with real random number if available
	random_device r;
	// Create random number generator
	default_random_engine e(r());
	// Create a distribution - we want doubles between 0.0 and 1.0
	uniform_real_distribution<double> distribution(0.0, 1.0);

	// Keep track of number of points in circle
	unsigned int in_circle = 0;
	// Iterate
	for (size_t i = 0; i < iterations; ++i)
	{
		// Generate random point
		auto x = distribution(e);
		auto y = distribution(e);
		// Get length of vector defined - use Pythagarous
		auto length = sqrt((x * x) + (y * y));
		// Check if in circle
		if (length <= 1.0)
			++in_circle;
	}
	// Calculate pi
	auto pi = (4.0 * in_circle) / static_cast<double>(iterations);

	
	
}

int main(int argc, char** argv) {
	int stop;

	// Create data file
	ofstream data("montecarlo.csv", ofstream::out);
	
	double pi;
	
	
	auto total_threads = std::thread::hardware_concurrency();
	// Write number of threads
	cout << "Number of threads = " << total_threads << endl;
	// Write number of threads to the file
	data << "num_threads_" << total_threads << std::endl;

	for (size_t iters_pow = 0; iters_pow < 28; ++iters_pow)
	{
		auto iters_total = pow(2, iters_pow);
		std::cout << "Total iterations: " << iters_total << std::endl;
		data << "iters: 2^" << iters_pow;
		// Get the start time
		auto start = system_clock::now();
		// We need to create total_threads threads
		vector<thread> threads;
		for (size_t n = 0; n < total_threads; ++n)
			// Working in base 2 to make things a bit easier
			threads.push_back(thread(monte_carlo_pi, iters_total));
		// Join the threads (wait for them to finish)
		for (auto &t : threads)
			t.join();
		//need to use double (not atomic) because arithmetic operators are not implemented
		
		// Get the end time
		auto end = system_clock::now();
		// Get the total time
		auto total = end - start;
		// Convert to milliseconds and output to file
		data << ", " << duration_cast<milliseconds>(total).count();
		data << "," << pi;
		data << endl;
	}
	// Close the file
	data.close();

	std::cin >> stop;
	return 0;
}