#include <mpi.h>
#include <vector>
#include <algorithm>

//merges the largest n values in local_data and temp_b into temp_c
//temp_c is then copied back into local_data
void merge_high(std::vector<unsigned> &local_data, std::vector<unsigned> &temp_b, std::vector<unsigned> &temp_c) {
	int ai, bi, ci;
	//get starting size
	ai = bi = ci = local_data.size() - 1;
	//loop through each value and store relevant largest value in temp_c
	for (; ci >= 0; --ci) {
		//find largest from local data and temp_b
		if (local_data[ai] >= temp_b[bi]) {
			temp_c[ci] = local_data[ai];
			--ai;
		}
		else {
			temp_c[ci] = temp_b[bi];
			--bi;
		}
	}
	//copy temp_c into local_data
	std::copy(temp_c.begin(), temp_c.end(), local_data.begin());
}

//merges smallest n values in local_data and temp_b in to temp_c
//then temp_c is copied into local_data
void merge_low(std::vector<unsigned> &local_data, std::vector<unsigned> &temp_b, std::vector<unsigned> &temp_c) {
	int ai, bi, ci;
	//start at 0
	ai = bi = ci = 0;
	//loop through each value and store relevant smalled value in temp_c
	for (; ci < local_data.size(); ++ci) {
		//find smallest from local data and temp_b
		if (local_data[ai] < temp_b[bi]) {
			temp_c[ci] = local_data[ai];
			++ai;
		}
		else {
			temp_c[ci] = temp_b[bi];
			++bi;
		}
	}
	//copy temp_c into local data
	std::copy(temp_c.begin(), temp_c.end(), local_data.begin());
}

void odd_even_iter(std::vector<unsigned> &local_data, std::vector<unsigned> &temp_b, std::vector<unsigned> &temp_c,
				   unsigned phase, int even_partner, int odd_partner, unsigned my_rank, unsigned num_process) {
	//operate based on phase
	if (phase % 2 == 0) {
		//check if even partner is valid
		if (even_partner >= 0) {
			//exchange data with even partner
			MPI_Sendrecv(&local_data[0], local_data.size(), MPI_UNSIGNED, even_partner, 0, &temp_b[0], temp_b.size(), MPI_UNSIGNED, even_partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//merge results accourdingly
			if (my_rank % 2 == 0)
				merge_low(local_data, temp_b, temp_c);
			else
				merge_high(local_data, temp_b, temp_c);
		}
	}
	else {
		//check if odd partner is valid
		if (odd_partner >= 0) {
			// Exchange data with odd partner
			MPI_Sendrecv(&local_data[0], local_data.size(), MPI_UNSIGNED, odd_partner, 0, &temp_b[0], temp_b.size(), MPI_UNSIGNED, odd_partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			// Merge results accordingly
			if (my_rank % 2 == 0)
				merge_high(local_data, temp_b, temp_c);
			else
				merge_low(local_data, temp_b, temp_c);
		}
	}

}
//odd even sort
void odd_even_sort(std::vector<unsigned> &local_data, unsigned my_rank, unsigned num_process) {
	//temporary storage
	std::vector<unsigned> temp_b(local_data);
	std::vector<unsigned> temp_c(local_data);
	//partners. even phase look left. odd phase look right
	int even_partner, int odd_partner;

	//find partners
	if (my_rank % 2 == 0) {
		even_partner = static_cast<int>(my_rank) + 1;
		odd_partner = static_cast<int>(my_rank) - 1;
		//check that even_partner is valid
		if (even_partner == num_process)
			even_partner = MPI_PROC_NULL;
	}
	else {
		even_partner = static_cast<int>(my_rank) - 1;
		odd_partner = static_cast<int>(my_rank) + 1;
		//check that odd_partner is valid
		if (odd_partner == num_process)
			odd_partner = MPI_PROC_NULL;
	}
	//sort this processes share of the data
	//std::sort is in the algorithm header
	std::sort(local_data.begin(), local_data.end());

	// Phased odd-even transposition sort
	for (unsigned int phase = 0; phase < num_process; ++phase)
		odd_even_iter(local_data, temp_b, temp_c, phase, even_partner, odd_partner, my_rank, num_process);
}

int main() {

	/*
	TO BE FINISHED
	*/
	return 0;
}