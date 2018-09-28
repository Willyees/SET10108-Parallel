#include <mutex>
#include <condition_variable>

std::mutex mut;

void task1(std::condition_variable& cond) {
	std::lock_guard<std::mutex> lock(mut);

}

void task2(std::condition_variable& cond) {
	std::unique_lock<std::mutex> lock(mut);
	cond.wait(lock);

}
int main(){
	std::condition_variable cond;
	std::thread t1(task1, std::ref(cond));
	std::thread t2(task2, std::ref(cond));
	
	t1.join();
	t2.join();
}