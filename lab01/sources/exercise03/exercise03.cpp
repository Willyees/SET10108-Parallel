#include <thread>
#include <iostream>
#include <chrono>
#include <random>
#include <functional>
#include <fstream>


using std::cout;
using std::cin;
using std::endl;
using namespace std;
using namespace std::chrono;

constexpr size_t num_threads = 100;

void pi(unsigned iterations) {
	std::random_device r;
	std::default_random_engine e(r());
	std::uniform_real_distribution<double> distr(0.0, 1.0);

	unsigned int in_circle = 0;
	for (unsigned i = 0; i < iterations; ++i) {
		auto x = distr(e);
		auto y = distr(e);
		auto length = sqrt((x * x) + (y * y));
		if (length <= 1)
			++in_circle;

	}
	auto pi = (4.0 * in_circle) / static_cast<double>(iterations);
	cout << pi << endl;
}
int main(int argc, char **argv)
{
	int stop;

	std::ofstream data("montecarlo_lambda.csv", std::ostream::out);
	for (size_t num_threads = 0; num_threads <= 6; ++num_threads) {
		auto total_threads = static_cast<unsigned>(pow(2.0, num_threads));
		cout << "Number of threads = " << total_threads << endl;
		data << "num_threads " << total_threads;
		unsigned iterations = static_cast<unsigned int>(pow(2.0, 24.0 - num_threads));
		//for (size_t iters = 0; iters < 100; ++iters) {
		auto start = std::chrono::system_clock::now();
		std::vector<std::thread> threads;
		for (size_t n = 0; n < total_threads; ++n) {
			
			threads.push_back(std::thread(
				[&iterations] {
				std::random_device r;
				std::default_random_engine e(r());
				std::uniform_real_distribution<double> distr(0.0, 1.0);

				unsigned int in_circle = 0;
				for (unsigned i = 0; i < iterations; ++i) {
					auto x = distr(e);
					auto y = distr(e);
					auto length = sqrt((x * x) + (y * y));
					if (length <= 1)
						++in_circle;

				}
				auto pi = (4.0 * in_circle) / static_cast<double>(iterations);
				cout << pi << endl;
			}));
		}
		for (auto &t : threads)
			t.join();
		auto end = std::chrono::system_clock::now();
		auto total = end - start;
		data << "," << std::chrono::duration_cast<std::chrono::milliseconds>(total).count();
		//}
		data << endl;
	}
	data.close();

	std::cin >> stop;

	return 0;
}