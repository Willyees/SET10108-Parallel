#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include "thread_safe_stack.h"

void pusher(std::shared_ptr<thread_safe_stack<unsigned int>> stack) {
	for (unsigned i = 0; i < 1000000; ++i) {
		stack->push(i);
		// Make the pusher yield.  Will give priority to another thread
		std::this_thread::yield();
	}
}

void popper(std::shared_ptr<thread_safe_stack<unsigned int>> stack) {
	// Popper will pop 1 million values from the stack.
	// We do this using a counter and a while loop
	unsigned int count = 0;
	while (count < 1000000) {
		//try pop a value
		try {
			auto val = stack->pop();
			//item popped , increment count
			++count;
		}
		catch (std::exception e) {
			//item not popped
			std::cout << e.what() << std::endl;
		}
	}
}

int main(int argc, char** argv) {
	int stop;
	
	//create thread safe stack
	auto stack = std::make_shared<thread_safe_stack<unsigned int>>();
	//create two threads
	std::thread t1(popper, stack);
	std::thread t2(pusher, stack);

	//join two threads
	t1.join();
	t2.join();

	//check if stack is empty
	std::cout << "stack empty = " << stack->empty() << std::endl;

	std::cin >> stop;
	return 0;
}