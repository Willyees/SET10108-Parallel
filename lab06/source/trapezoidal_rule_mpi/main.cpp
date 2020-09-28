#define _USE_MATH_DEFINES

#include <mpi.h>
#include <iostream>
#include <functional>
#include <memory>
#include <math.h>
#include <thread>



constexpr double PI = M_PI;

double trapezoidal(std::function<double(double)> f, double start, double end, unsigned iterations ,size_t my_rank, size_t num_process)
{
	
	// Calculation iteration slice size
	auto slice_size = (end - start) / iterations;
	// Calculate number of iterations per thread
	auto iterations_thread = iterations / num_process;
	// Calculate this thread's start point
	auto local_start = start + ((my_rank * iterations_thread) * slice_size);
	// Calculate this thread's end point
	auto local_end = local_start + iterations_thread * slice_size;
	// Calculate initial result
	auto my_result = (f(local_start) + f(local_end)) / 2.0;

	// Declare x before the loop - stops it being allocated and destroyed each iteration
	double x;
	// Sum each iteration
	for (size_t i = 0; i <= iterations_thread - 1; ++i)
	{
		// Calculate next slice to calculate
		x = local_start + i * slice_size;
		// Add to current result
		my_result += f(x);
	}
	// Multiply the result by the slice size
	my_result *= slice_size;

	return my_result;
}

int main() {
	
	auto result = MPI_Init(nullptr, nullptr);
	if (result != MPI_SUCCESS) {
		MPI_Abort(MPI_COMM_WORLD, result);
		std::cout << "problem initializing mpi" << std::endl;
		return -1;
	}
	int num_process, my_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_process);
	//std::cout << "total processes:" << num_process <<  std::endl;
	//sync
	MPI_Barrier(MPI_COMM_WORLD);
	//function to use
	auto f = [](double x) { return cos(x); };
	
	//perfom calculation
	unsigned iterations = static_cast<unsigned>(pow(2, 24));
	auto local_result = trapezoidal(f, 0.0, PI, iterations, my_rank, num_process);

	//reduce result
	double global_result = 0.0;
	MPI_Reduce(&local_result, &global_result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	//if main process, display result
	if (my_rank == 0)
		std::cout << "area under the curve: " << global_result << std::endl;
	
	MPI_Finalize();
	//std::cin.get();
	return 0;
}