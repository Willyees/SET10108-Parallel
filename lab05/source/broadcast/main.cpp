#include <iostream>
#include <mpi.h>

int main() {
	auto result = MPI_Init(nullptr, nullptr);

	if (result != MPI_SUCCESS) {
		std::cout << "ERROR - initialising MPI" << std::endl;
		MPI_Abort(MPI_COMM_WORLD, result);
		return -1;
	}
	int my_rank, num_process;

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_process);

	if (my_rank == 0) {
		//broadcast message to workers
		std::string str = "Whassup?";
		MPI_Bcast((void*)&str.c_str()[0], str.length() + 1, MPI_CHAR, 0, MPI_COMM_WORLD);
	}
	else {
		//receive messafge rom main process
		//buffer to hold message
		char data[100];
		MPI_Bcast(data, 100, MPI_CHAR, 0, MPI_COMM_WORLD);
		std::cout << my_rank << ": " << data << std::endl;
	}
	//mpi shutdown
	MPI_Finalize();
	return 0;
}