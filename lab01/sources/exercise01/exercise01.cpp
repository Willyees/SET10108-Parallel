#include <random>
#include <iostream>
#include <chrono>
#include <thread>

using std::cout;
using std::endl;

void max(std::vector<int> vi) {
        int max = vi.at(0);
        for (auto &i : vi) {
                if (i > max)
                        max = i;
        }
        //cout << "Max: " << max << endl;
}

void min(std::vector<int> vi) {
        int min = vi.at(0);
        for (auto &i : vi) {
                if (i < min)
                        min = i;
        }
        //cout << "Min: " << min << endl;
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

        for(size_t pow_thread = 0; pow_thread <= 5; ++pow_thread){
                auto total_threads = pow(2, pow_thread);
                cout << "Total threads: " << total_threads * 2 << endl;
                std::vector<std::thread> threads;
                auto start = std::chrono::system_clock::now();
                for (size_t num_thread = 0; num_thread < total_threads; ++num_thread){
                        threads.push_back(std::thread(max, vi));
                        threads.push_back(std::thread(min, vi));
                }
                for (auto &t : threads)
                        t.join();
                auto end = std::chrono::system_clock::now();

                auto total_time = end - start;
                cout << "Total time: " <<
                        std::chrono::duration_cast<std::chrono::milliseconds>(total_time).count() << endl;

        }
        std::cin >> stop;
        return 0;
}
