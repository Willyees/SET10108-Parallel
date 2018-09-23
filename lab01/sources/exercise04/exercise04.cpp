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

void work() {
	int n = 0;
	for (int i = 0; i < 1000000; ++i)
		++n;
}
void task(size_t n, int val) {
	cout << "Thread: " << n << " Random value: " << val << endl;
}
void task_one() {
	cout << "Task one starting" << endl;
	cout << "Task one sleeping for 3 seconds" << endl;
	std::this_thread::sleep_for(std::chrono::seconds(3));
	cout << "Task one awake again" << endl;
	cout << "Task one sleeping for 5 seconds" << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	cout << "Task one awake again" << endl;
	cout << "Task one ending" << endl;
}

void task_two() {
	cout << "Task two starting" << endl;
	cout << "Task two sleeping for 2 seconds" << endl;
	std::this_thread::sleep_for(std::chrono::microseconds(2000000));
	cout << "Task two awake again" << endl;
	cout << "Task two sleeping for 10 seconds" << endl;
	std::this_thread::sleep_for(std::chrono::seconds(10));
	cout << "Task two awake again" << endl;
	cout << "Task two ending" << endl;
}
void hello_world()
{
	std::cout << "Hello from thread " << std::this_thread::get_id() << std::endl;
}
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

	std::ofstream data("montecarlo_finer_times.csv", std::ostream::out);
	for (size_t num_threads = 0; num_threads <= 6; ++num_threads) {
		auto total_threads = static_cast<unsigned>(pow(2.0, num_threads));
		cout << "Number of threads = " << total_threads << endl;
		data << "num_threads " << total_threads;

		//for (size_t iters = 0; iters < 100; ++iters) {
		auto start = std::chrono::high_resolution_clock::now();
		std::vector<std::thread> threads;
		for (size_t n = 0; n < total_threads; ++n) {
			threads.push_back(std::thread(
				pi, static_cast<unsigned int>(pow(2.0, 24.0 - num_threads))));
		}
		for (auto &t : threads)
			t.join();
		auto end = std::chrono::high_resolution_clock::now();
		auto total = end - start;
		data << "," << std::chrono::duration_cast<std::chrono::microseconds>(total).count();
		//}
		data << endl;
	}
	data.close();

	std::cin >> stop;

	return 0;
}