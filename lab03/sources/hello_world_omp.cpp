#include <iostream>
#include <omp.h>

using namespace std;

//Number to threads to run
constexpr int THREADS = 10;

void hello(){
	

	//Get thread number (ID)
	auto my_rank = omp_get_thread_num();
	//get number of threads in operation
	auto thread_count = omp_get_num_threads();
	//display message
	cout << "Hello from thread " << my_rank << "of " << thread_count << endl;

	
}

int main() {
	int stop;
	//run hello THREADS times
#pragma omp parallel num_threads(THREADS)
	hello();

	cin >> stop;
	return 0;
}