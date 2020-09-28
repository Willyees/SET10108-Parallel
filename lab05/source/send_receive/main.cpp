#include <iostream>
#include <mpi.h>
#include <sstream>

const unsigned int MAX_STRING = 100;
int main() {
	int num_procs, my_rank;

	//Initialise MPI
	auto result = MPI_Init(nullptr, nullptr);
	if (result != MPI_SUCCESS) {
		std::cout << "ERROR - initialising MPI" << std::endl;
		MPI_Abort(MPI_COMM_WORLD, result);
		return -1;
	}
	//Get MPI Information
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	//check if we are the main process
	if (my_rank != 0) {
		//not main process - send message
		//genereate messae
		std::stringstream buffer;
		buffer << "Greeting from process " << my_rank << " of " << num_procs << "!";
		//get caracther array from string
		auto data = buffer.str().c_str();
		//send to main node
		MPI_Send((void*)data, buffer.str().length() + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	}
	else {
		//main process, print message
		std::cout << "Greetings from process " << my_rank << " of " << num_procs << "!" << std::endl;
		//read data from each worker process
		char message[MAX_STRING];
		for (int i = 1; i < num_procs; ++i) {
			//receive message into the buffer
			MPI_Recv(message, MAX_STRING, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//display message
			/*for (int j = 0; j < MAX_STRING; ++j) {
				std::cout << message[j] << " ";
				if (message[j] == '\0')
					std::cout << "NULLTERM!";
			}
			std::cout << std::endl;*/
			std::cout << message << std::endl;
			
		}
	}
	//shutdown MPI
	MPI_Finalize();

	
	return 0;
}