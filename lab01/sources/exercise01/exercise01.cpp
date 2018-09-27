#include <random>
#include <iostream>
#include <chrono>
#include <thread>

using std::cout;
using std::endl;

typedef std::vector<int>::iterator vec_it;

//find max and min in same function
void max_min(vec_it start, vec_it end, int& result_max, int& result_min) {
		
	int max = *start;
	int min = *start;

        for (vec_it it = start; it != end; ++it) {
			if ( *it> max)
				max = *it;
			if (*it < min)
				min = *it;
		}
		result_max = max;
		result_min = min;
        //cout << "Max: " << max << endl;
		
}
//only find min in this function
void min(vec_it start, vec_it end, int &result) {

	int min = *start;

	for (vec_it it = start; it != end; ++it) {
		if (*it < min)
			min = *it;
	}
	result = min;
	//cout << "Max: " << max << endl;

}
void max(vec_it start, vec_it end, int &result) {

	int max = *start;

	for (vec_it it = start; it != end; ++it) {
		if (*it > max)
			max = *it;
	}
	result = max;
	//cout << "Max: " << max << endl;

}
int main()
{
        int stop;

        std::random_device r;
        std::default_random_engine e(r());
        std::vector<int> vi;
		
		for (int i = 0; i < 10000000; ++i) {
                vi.push_back(e());
        }

		vec_it start_it = vi.begin();
        for(size_t pow_thread = 0; pow_thread <= 5; ++pow_thread){
                auto total_threads = pow(2, pow_thread);
                cout << "Total threads: " << total_threads * 2 << endl;//using 2 functions to find max/min 
                std::vector<std::thread> threads;
				
				unsigned size_to_process = 10000000 / total_threads;
				std::vector<int> results_max(total_threads);
				std::vector<int> results_min(total_threads);
				
                auto start = std::chrono::system_clock::now();
                for (size_t num_thread = 0; num_thread < total_threads; ++num_thread){
					vec_it end_it = start_it + size_to_process;
                        //threads.push_back(std::thread(max_min, start_it, end_it, std::ref(results_max[num_thread]), std::ref(results_min[num_thread])));
						threads.push_back(std::thread(max, start_it, end_it, std::ref(results_max[num_thread])));
						threads.push_back(std::thread(min, start_it, end_it, std::ref(results_min[num_thread])));
						start_it += size_to_process;
                }
                for (auto &t : threads){
                        t.join();
						
				}
				int max, min;
				max = *std::max_element(results_max.begin(), results_max.end());
				min = *std::min_element(results_min.begin(), results_min.end());
				auto end = std::chrono::system_clock::now();
				cout << "max: " << max << endl;
				cout << "max: " << min << endl;
                
				auto total_time = end - start;
                cout << "Total time: " <<
                        std::chrono::duration_cast<std::chrono::milliseconds>(total_time).count() << endl;
				start_it = vi.begin();
        }
        std::cin >> stop;
        return 0;
}
