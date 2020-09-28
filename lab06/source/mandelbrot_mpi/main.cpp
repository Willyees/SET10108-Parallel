#include <mpi.h>
#include <iostream>
#include <vector>

// Number of iterations to perform to find pixel value
constexpr size_t max_iterations = 1000;
// Mandelbrot dimensions are ([-2.1, 1.0], [-1.3, 1.3])
constexpr double xmin = -2.1;
constexpr double xmax = 1.0;
constexpr double ymin = -1.3;
constexpr double ymax = 1.3;


std::vector<double> mandelbrot(unsigned dim, size_t start_y, size_t end_y)
{
	// The conversion from Mandelbrot coordinate to image coordinate
	double integral_x = (xmax - xmin) / static_cast<double>(dim);
	double integral_y = (ymax - ymin) / static_cast<double>(dim);
	// Declare values we will use
	double x, y, x1, y1, xx = 0.0;
	size_t loop_count = 0;
	// Where to store the results
	std::vector<double> results;

	// Loop through each line
	y = ymin + (start_y * integral_y);
	for (size_t y_coord = start_y; y_coord < end_y; ++y_coord)
	{
		x = xmin;
		// Loop through each pixel on the line
		for (size_t x_coord = 0; x_coord < dim; ++x_coord)
		{
			x1 = 0.0, y1 = 0.0;
			loop_count = 0;
			// Calculate Mandelbrot value
			while (loop_count < max_iterations && sqrt((x1 * x1) + (y1 * y1)) < 2.0)
			{
				++loop_count;
				xx = (x1 * x1) - (y1 * y1) + x;
				y1 = 2 * x1 * y1 + y;
				x1 = xx;
			}
			// Get value where loop completed
			auto val = static_cast<double>(loop_count) / static_cast<double>(max_iterations);
			// Push this value onto the vector
			results.push_back(val);
			// Increase x based on integral
			x += integral_x;
		}
		// Increase y based on integral
		y += integral_y;
	}
	// Return vector
	return results;
}
int main() {
	int my_rank, process_num;

	auto result = MPI_Init(nullptr, nullptr);

	if (result != MPI_SUCCESS) {
		std::cout << "problem initializing MPI" << std::endl;
		MPI_Abort(MPI_COMM_WORLD, result);
		return -1;
	}

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &process_num);

	//broadcast dimension around the workers
	unsigned dim = 0;
	if (my_rank == 0) {
		//broadcast dimension to all the workers 
		dim = 8192;
		MPI_Bcast(&dim, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
	}
	else {
		//get dimension
		MPI_Bcast(&dim, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
	}
	
	//perform mandelbrot
	auto strip_height = dim / process_num;
	auto res = mandelbrot(dim, my_rank * strip_height, (my_rank + 1) * strip_height);
	
	//gather results back
	std::vector<float> data;
	
	//check if main process, if so resize data to gather results
	if (my_rank == 0) {
		data.resize(res.size() * process_num);
	}
	
	//gather results
	MPI_Gather(&res[0], res.size(), MPI_FLOAT, &data[0], res.size(), MPI_FLOAT, 0, MPI_COMM_WORLD);

	std::cout << "finish!" << std::endl;

	//shutdown MPI
	MPI_Finalize();

	return 0;
}