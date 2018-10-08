#include <random>
#include <fstream>
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;


// Generates a vector of random values
vector<unsigned int> generate_values(size_t size)
{
	// Create random engine
	random_device r;
	default_random_engine e(r());
	// Generate random numbers
	vector<unsigned int> data;
	for (size_t i = 0; i < size; ++i)
		data.push_back(e());
	return data;
}

void bubble_sort(vector<unsigned int> data) {
	int countj = 0;
	int counti = 0;
	for(size_t j = 0; j < data.size(); ++j){
		
		for (size_t i = 0; i != data.size() - 2; ++i) {
			
			if (data.at(i) > data.at(i + 1)) {
				unsigned temp = data.at(i + 1);
				data.at(i + 1) = data.at(i);
				data.at(i) = temp;
			}
			++counti;

		}
		++countj;
	}
	cout << countj << endl << counti << endl;
}
int main() {
	//create file
	ofstream results("bubble.csv", ofstream::out);
	//gather resutls for 2^8 to 2^16 results
	for (size_t size = 8; size < 16; ++size) {
		//output data size
		results << pow(2, size) << " , ";
		//gather 100 results
		for (size_t i = 0; i < 1; ++i) {
			//generate vector of random variables
			auto data = generate_values(static_cast<size_t>(pow(2,size)));
			//sort the vector
			cout << "Sorting" << endl;
			auto start = chrono::system_clock::now();
			bubble_sort(data);
			auto end = chrono::system_clock::now();
			auto total = duration_cast<milliseconds>(end - start).count();
			//output total 
			results << total << ",";
		}
		results << endl;
		
	}
	results.close();
}