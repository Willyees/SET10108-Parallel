#pragma once
#include <mutex>
#include <exception>
#include <stack>
#include <memory>


template<typename T>
class thread_safe_stack {
private:
	//Stack obj using
	std::stack<T> data;
	//mutex to control access
	mutable std::mutex mut;
public:
	thread_safe_stack() {}
	thread_safe_stack(const thread_safe_stack& other) {
		//copy data from other stack -> lock other
		std::lock_guard<std::mutex> lock(other.mut);
		data = other.data;
	}
	void push(T value) {
		//Lock access to object
		std::lock_guard<std::mutex> lock(mut);
		//push value into internal stack
		data.push(value);
	}

	T pop() {
		//lock access to object
		std::lock_guard<std::mutex> lock(mut);
		//check if stack is empty
		if (data.empty()) throw std::exception();
		//access value at top of stack
		auto res = data.top();
		//remove top item from stack
		data.pop();

		return res;
	}
	bool empty() const {
		std::lock_guard<std::mutex> lock(mut);

		return data.empty();
	}

};

