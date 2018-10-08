#include <omp.h>
#include <functional>
#include <memory>
#include <iostream>
#include <thread>

void trapezoidal(std::function<double(double)> f, double start, double end, size_t iterations, std::shared_ptr<double> p) {
	//get thread number ID
	auto my_rank = omp_get_thread_num();
	//get number of threads
	auto thread_count = omp_get_num_threads();
	//calculation iteration slice size
	auto slice_size = (end - start) / iterations;
	//calculate number of iterations per thread
	auto iterations_thread = iterations / thread_count;
	//calculate this thread starting point
	auto local_start = start + slice_size * my_rank * iterations_thread;
	//calculate this thread end point
	auto local_end = local_start + slice_size * iterations_thread;
	//calculate itinital result 
	auto my_result = 0.0;

	//declare x before the loop - stops it being allocated and destroyed each iterations
	double x = local_start;
	double x1;
	//sum each iteration
	for (size_t i = 0; i <= iterations_thread - 1; ++i) {
		//calculate next slice to calculate
		x1 = x + slice_size;
		//add to current result
		my_result += (f(x) + f(x1)) / 2 * slice_size;
		x = x1;
	}
	//multiply the result by the slice size
	
	//my_result *= slice_size / 2;
	
	//critical section - add to the shared data
#pragma omp critical
	*p += my_result;

}

int main() {
	//declare shared result 
	auto result = std::make_shared<double>(0.0);
	//define start and end values
	auto start = 0.0;
	auto end = 3.14159265359;
	//defined number of trapeziods to generation
	auto trapezoids = static_cast<size_t>(pow(2, 24));
	//get number of threads
	auto thread_count = std::thread::hardware_concurrency();

	//create fucntion to caliucalte integral of. use cos
	auto f = [](double x) { return sin(x); };

	//run trap in parallel
#pragma omp parallel num_threads(thread_count)
	trapezoidal(f, start, end, trapezoids, result);

	//output result
	std::cout << "using " << trapezoids << " trapezoids. ";
	std::cout << "estimated integral of funcion " << start << " to " << end << " = " << *result << std::endl;
	std::cin.get();
	return 0;
}