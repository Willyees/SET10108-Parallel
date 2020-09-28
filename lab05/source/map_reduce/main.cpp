#include <iostream>
#include <mpi.h>
#include <random>

double montecarlo_pi(size_t iterations)
{
	// Seed with real random number if available
	std::random_device r;
	// Create random number generator
	std::default_random_engine e(r());
	// Create a distribution - we want doubles between 0.0 and 1.0
	std::uniform_real_distribution<double> distribution(0.0, 1.0);

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
	//lock pi and then add the result to others pi calculated in other threads

	return pi;
}


int main(){
	int num_procs, my_rank;

	auto result = MPI_Init(nullptr, nullptr);
	if (result != MPI_SUCCESS) {
		std::cout << "ERROR - initialising MPI" << std::endl;
		MPI_Abort(MPI_COMM_WORLD, result);
		return -1;
	}

	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	double local_sum, global_sum;

	//calcualte local sum - use previously defined function
	local_sum = montecarlo_pi(static_cast<unsigned int>(pow(2, 24)));
	//print local sum
	std::cout.precision(std::numeric_limits<double>::digits10);
	std::cout << my_rank << ":" << local_sum << std::endl;
	//reduce
	MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	//if it is main process, show global reduced sum
	if (my_rank == 0) {
		global_sum /= num_procs;
		std::cout << "pi = " << global_sum << std::endl;
	}

	MPI_Finalize();
	return 0;
}