#include <iostream>
#include <condition_variable>
#include <chrono>
#include <thread>

using std::cout;
using std::endl;
std::mutex mut;

void task1(std::condition_variable& condition) {
	//Task 1 initailly sleep for 3 secs
	std::cout << "Task 1 sleeping for 3 secs" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(3));
	// Create lock - thread 2 should manged to grab it first
	auto lock = std::unique_lock<std::mutex>(mut);
	
	//Notify waiting thread
	std::cout << "Task 1 notifying wating thread" << std::endl;
	condition.notify_one();
	cout << "after notify in task1" << endl;
	//Wait for notification
	std::cout << "Task 1 waitign for notification" << std::endl;
	//Wait, freeing lock as we do
	condition.wait(lock);
	//We are free to continue
	std::cout << "Task 1 notified" << std::endl;
	//sleep for 3 seconds
	cout << "Task 1 sleeping for 3 seconds" << endl;
	std::this_thread::sleep_for(std::chrono::seconds(3));
	//Notify any waitign thread
	cout << "Task 1 notifying waitign thread" << endl;
	condition.notify_one();
	//Wait 3 seconds for notification
	cout << "Task 1 waiting 1 seconds for notification" << endl;
	if (condition.wait_for(lock, std::chrono::seconds(1)) == std::cv_status::no_timeout)
		cout << "Task 1 notified before 1 seconds" << endl;
	else
		cout << "Task 1 got tired of waiting" << endl;
	cout << "Task 1 finished" << endl;
} 

void task2(std::condition_variable& condition) {
	//Create lock
	auto lock = std::unique_lock<std::mutex>(mut);//not lock guard because need to lock and unlock many times
	//Task 2 will initially wait for notification
	cout << "Task 2 waiting for notification" << endl;
	//Wait, releasing lock as we do
	condition.wait(lock);//passing lock, because it will unlock the lock (other threads can access it)
	//Free to continue, after being awakened it will lock again the locker
	cout << "Task 2 notified" << endl;
	//sleep for 5 secs
	cout << "Task 2 sleeping for 5 seconds" << endl;
	std::this_thread::sleep_for(std::chrono::seconds(5));
	//Notify waiting thread
	cout << "Thread 2 notifying waiting thread" << endl;
	condition.notify_one();
	//Now wait 5 secs for notification
	cout << "Task 2 watiign 5 seconds for notification" << endl;
	if (condition.wait_for(lock, std::chrono::seconds(5)) == std::cv_status::no_timeout)
		cout << "Task 2 norified before 5 seconds" << endl;
	else
		cout << "Task 2 got tired of waiting" << endl;
	//Sleep 5 secs
	std::this_thread::sleep_for(std::chrono::seconds(5));
	//notify waiting threads
	cout << "Task 2 notifying waiting thread" << endl;
	condition.notify_one();
	cout << "Task 2 finished" << endl;
}


int main() {
	int stop;

	std::condition_variable condition;
	std::thread t1 = std::thread(task1, std::ref(condition));
	std::thread t2 = std::thread(task2, std::ref(condition));
	
	t1.join();
	t2.join();


	std::cin >> stop;

	return 0;
}