#include <random>
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
using namespace chrono;


// Generates a vector of random values
vector<unsigned int> generate_values(size_t size)
{
	// Create random engine
	random_device r;
	default_random_engine e(r());
	// Generate random numbers
	vector<unsigned int> data;
	for (size_t i = 0; i < size; ++i)
		data.push_back(e());
	return data;
}

void parallel_sort(vector<unsigned int>& values)
{
	// Get the number of threads
	auto num_threads = thread::hardware_concurrency();
	// Get the number of elements in the vector
	auto n = values.size();
	// Declare the variables used in the loop
	int i, tmp, phase;
	// Declare parallel section
#pragma omp parallel num_threads(num_threads) default(none) shared(values, n) private(i, tmp, phase)
	for (phase = 0; phase < n; ++phase)
	{
		// Determine which phase of the sort we are in
		if (phase % 2 == 0)
		{
			// Parallel for loop.  Each thread jumps forward 2 so no conflict
#pragma omp for
			for (i = 1; i < n - 1; i += 2)
			{
				// Check if we should swap values
				if (values[i - 1] > values[i])
				{
					// Swap values
					tmp = values[i - 1];
					values[i - 1] = values[i];
					values[i] = tmp;
				}
			}
		}
		else
		{
			// Parallel for loop.  Each thread jumps forward 2 so no conflict
#pragma omp for
			for (i = 1; i < n - 1; i += 2)
			{
				// Check is we should swap values
				if (values[i] > values[i + 1])
				{
					// Swap values
					tmp = values[i + 1];
					values[i + 1] = values[i];
					values[i] = tmp;
				}
			}
		}
	}
}
int main() {
	//create file
	ofstream results("bubble_parallel.csv", ofstream::out);
	//gather resutls for 2^8 to 2^16 results
	for (size_t size = 8; size < 16; ++size) {
		//output data size
		results << pow(2, size) << " , ";
		//gather 100 results
		for (size_t i = 0; i < 1; ++i) {
			//generate vector of random variables
			auto data = generate_values(static_cast<size_t>(pow(2,size)));
			//sort the vector
			cout << "Sorting" << endl;
			auto start = chrono::system_clock::now();
			parallel_sort(data);
			auto end = chrono::system_clock::now();
			auto total = duration_cast<milliseconds>(end - start).count();
			//output total 
			results << total << ",";
		}
		results << endl;
		
	}
	results.close();
}