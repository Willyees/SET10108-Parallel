#include <thread>
#include <iostream>

using namespace std;

constexpr int num_threads = 10;

int main() {
	int stop;
	// Get number of supported threads
	auto num_threads = thread::hardware_concurrency();

	// Number of iteration values to perform
	const int n = static_cast<int>(pow(2, 30));
	// Factor value
	double factor = 0.0;
	// Calculated pi
	double pi = 0.0;

	// Parallelised for loop that does the work
#pragma omp parallel for num_threads(num_threads) reduction(+:pi) private(factor)
	for (int k = 0; k < n; ++k) {
		//determine sign of factor
		if (k % 2 == 0)
			factor = 1.0;
		else
			factor = -1.0;
		//add this iteration value to pi sum
		pi += factor / (2.0 * k + 1);
	}

	//get final value of pi
	pi *= 4.0;
	//show more precision of pi
	cout.precision(numeric_limits<double>::digits10);
	cout << "pi = " << pi << endl;

	cin >> stop;
	return 0;
}