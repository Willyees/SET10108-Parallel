#include <iostream>
#include <mpi.h>

int main() {
	//intialize MPI
	auto result = MPI_Init(nullptr, nullptr);
	//check that we initialised correctly
	if (result != MPI_SUCCESS) {
		//display error and abort
		std::cout << "ERROR - initalising MPI! " << std::endl;
		MPI_Abort(MPI_COMM_WORLD, result);
		return -1;
	}
	//get MPI information
	int num_procs, rank, length;
	char host_name[MPI_MAX_PROCESSOR_NAME];
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(host_name, &length);

	//display information
	std::cout << "number of processors = " << num_procs << std::endl;
	std::cout << "My rank = " << rank << std::endl;
	std::cout << "Running on = " << host_name << std::endl;
	// Shutdown MPI
	MPI_Finalize();

	return 0;
}