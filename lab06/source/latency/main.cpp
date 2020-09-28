#include <mpi.h>
#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>

using namespace std::chrono;
int main(int argc, char* args) {
	
	std::ofstream data("times.csv", std::ios_base::out);
	int result = MPI_Init(nullptr, nullptr);
	if (result != MPI_SUCCESS) {
		MPI_Abort(MPI_COMM_WORLD, result);
		std::cout << "problem initializing mpi" << std::endl;
		return EXIT_FAILURE;
	}

	int my_rank, process_num;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &process_num);
	//buffs used to receive and send 1 char
	
	//for (unsigned i = 0; i < 100; ++i) {
		MPI_Barrier(MPI_COMM_WORLD);
		for (unsigned j = 10; j < 20; ++j) {
			
			std::vector<char> receive(pow(2, j));
			std::vector<char> send (pow(2, j));
			
			auto start = system_clock::now();
			
			if (my_rank == 0) {
					//ping
					MPI_Send(&send[0], pow(2,j), MPI_CHAR, 1, 0, MPI_COMM_WORLD);
					//pong 
					MPI_Recv(&receive[0], pow(2, j), MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
			else {
					//pong 
					MPI_Recv(&receive[0], pow(2, j), MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					// Ping
					MPI_Send(&send[0], pow(2, j), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
			}
		
			// Get time
			auto end = system_clock::now();
			auto total = duration_cast<nanoseconds>(end - start).count();
		
			// Divide total by number of iterations to get time per iteration
			auto res = static_cast<double>(total);//time taken to send pow(2,j) data
			// Divide by two to get latency
			res /= 2.0;
			auto ratio_sec = 1000000000 / res;
			auto data_in_mb = pow(2,j) * ((pow(2, 20) / pow(2, j)) / 8;//data sent in mb(megabit)
			auto mb_sec = data_in_mb * ratio_sec;
			
			
			// Output result
			if (my_rank == 0){
				data << res << ",";
				std::cout << res << std::endl;
			}
	}
	//}
	// Close file if main process
	if (my_rank == 0)
	{
		data << std::endl;
		data.close();
	}

	MPI_Finalize();
	return EXIT_SUCCESS;
}