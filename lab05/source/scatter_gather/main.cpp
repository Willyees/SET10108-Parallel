#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <mpi.h>

using namespace std::chrono;

const int SIZE = 40;

//randomly generate vector values
void generate_data(std::vector<float> &data) {
	//create random engine
	auto millis = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	std::default_random_engine e(static_cast<unsigned int>(millis.count()));
	for (int i = 0; i < data.size(); ++i) {
		data.at(i) = e();
	}
}

//normalize 4d vectors
void normalise_vector(std::vector<float> &data) {
	
	//iterate through each 4 dimensional vector
	for (unsigned int i = 0; i < data.size() / 4; ++i)
	{
		//sum the squares of the 4 components
		float sum = 0.0f;
		for (unsigned int j = 0; j < 4; ++j)
			sum += powf(data[(i * 4) + j], 2.0);
		//get the square root of the result
		sum = sqrtf(sum);
		//divide each component by sum
		for (unsigned int j = 0; j < 4; ++j)
			data[(i * 4) + j] /= sum;
	}
}

int main() {
	int num_procs, my_rank;

	auto result = MPI_Init(nullptr, nullptr);

	if (result != MPI_SUCCESS) {
		std::cout << "ERROR - initialising MPI" << std::endl;
		MPI_Abort(MPI_COMM_WORLD, result);
		return -1;
	}
	
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	//vector containing values to normalise
	std::vector<float> data;
	//local storage. allocate enough space
	std::vector<float> my_data(SIZE / num_procs);

	//check if main process
	if (my_rank == 0) {
		//generate the data
		data.resize(SIZE);
		generate_data(data);
		//scatter the data
		MPI_Scatter(&data[0], SIZE / num_procs, MPI_FLOAT, //SOURCE
					&my_data[0], SIZE / num_procs, MPI_FLOAT, //DESTINATION
					0, MPI_COMM_WORLD);
	}
	else {
		MPI_Scatter(nullptr, SIZE / num_procs, MPI_FLOAT, //SOURCE
					&my_data[0], SIZE / num_procs, MPI_FLOAT, //DESTINATION
					0, MPI_COMM_WORLD);
	}
	//normalise local data
	normalise_vector(my_data);
	
	if (my_rank == 0) {
		//gather results
		MPI_Gather(&my_data[0], SIZE / num_procs, MPI_FLOAT,// Source
			&data[0], SIZE / num_procs, MPI_FLOAT,          // Dest
			0, MPI_COMM_WORLD);
		int count = 0;
		for (auto v : data){
			std::cout << v;
			if (++count % 4 == 0)
				std::cout << std::endl;
		}
			
	}
	else
	{
		MPI_Gather(&my_data[0], SIZE / num_procs, MPI_FLOAT,// Source
			nullptr, SIZE / num_procs, MPI_FLOAT,           // Dest
			0, MPI_COMM_WORLD);
	}

	//mpi shutdown
	MPI_Finalize();
	return 0;
}