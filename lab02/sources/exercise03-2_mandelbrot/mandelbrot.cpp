#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <limits>

#include "FreeImage.h"

using namespace std;

// Number of iterations to perform to find pixel value
constexpr size_t max_iterations = 1000;

// Dimension of the image (in pixels) to generate
constexpr size_t dim = 8192;

// Mandelbrot dimensions are ([-2.1, 1.0], [-1.3, 1.3])
constexpr double xmin = -2.1;
constexpr double xmax = 1.0;
constexpr double ymin = -1.3;
constexpr double ymax = 1.3;

// The conversion from Mandelbrot coordinate to image coordinate
constexpr double integral_x = (xmax - xmin) / static_cast<double>(dim);
constexpr double integral_y = (ymax - ymin) / static_cast<double>(dim);
vector<double> mandelbrot(size_t start_y, size_t end_y)
{
	// Declare values we will use
	double x, y, x1, y1, xx = 0.0;
	size_t loop_count = 0;
	// Where to store the results
	vector<double> results;

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
int main(int agrc, char **argv)
{
	int stop;
	// Get the number of supported threads
	auto num_threads = thread::hardware_concurrency();
	FreeImageIO asd;
	// Determine strip height
	size_t strip_height = dim / num_threads;

	// Create futures
	vector<future<vector<double>>> futures;
	for (unsigned int i = 0; i < num_threads; ++i)
		// Range is used to determine number of values to process
		futures.push_back(async(mandelbrot, i * strip_height, (i + 1) * strip_height));

	// Vector to store results
	vector<vector<double>> results;
	// Get results
	for (auto &f : futures)
		results.push_back(f.get());

	FIBITMAP* Bitmap = FreeImage_Allocate(8192, 8192, 24, 0, 0, 0);
	
	double max = -DBL_MAX;
	double min = DBL_MAX;
	for (int i = 0; i < results.size(); ++i) {
		for (int j = 0; j < results[0].size(); ++j) {
			if (results[i][j] > max)
				max = results[i][j];
			if (results[i][j] < min)
				min = results[i][j];
		}
	}

	BYTE *bits = (BYTE*)FreeImage_GetBits(Bitmap);
	unsigned width = FreeImage_GetWidth(Bitmap);
	unsigned height = FreeImage_GetHeight(Bitmap);
	unsigned pitch = FreeImage_GetPitch(Bitmap);

	for (int y = 0; y < height; y++) {
		BYTE *pixel = (BYTE*)bits;
		for (int x = 0; x < width; x++) {
			int y1 = x * y / 8388608;
			int x1 = x * y % 8388608;
			//map a range to an other range: in this case mapping the smallest value found it the result vector to smallest value hex colour (same for biggest)
			//(x - input_start) / (input_end - input_start) * (output_end - output_start) + output_start
			//using max val hex colour 16777215 (#FFFFFF)
			int color = (results[y1][x1] - min) / (max-min) * (16777215 - 0) + 0;
			//convert decimal to rgb
			int r = color / 65536;
			int g = (color - r * 65536) / 256;
			int b = color - r * 65536 - g * 256;
			pixel[FI_RGBA_RED] = r;
			pixel[FI_RGBA_GREEN] = g;
			pixel[FI_RGBA_BLUE] = b;
			pixel += 3;
		}
		bits += pitch;
		
		
	}
	
	if (FreeImage_Save(FIF_BMP, Bitmap, "mybitmap.bmp", 0))
		cout << "saved" << endl;
	cout << "done" << endl;
	
	
	std::cin >> stop;
	

	return 0;
}