#include "Semaphore.h"
Semaphore::Semaphore(unsigned int count) :count_(count) {
}

Semaphore::~Semaphore() {
}

void Semaphore::wait() {
	std::unique_lock<std::mutex> unique_lck(mutex_);
	if (--count_ < 0) {
		condition_.wait(unique_lck);
	}
}

//template<typename Fn>
//inline void Semaphore::wait(unsigned int second, Fn func)
//{
//	std::unique_lock<std::mutex> unique_lck(mutex_);
//	if (--count_ < 0) {
//		xtime xtm;
//		auto now = std::chrono::system_clock::now();
//		condition_.wait_until(unique_lck, now.time_since_epoch+second, func);
//	}
//}

void Semaphore::signal() {
	std::lock_guard<std::mutex> lck(mutex_);
	if (++count_ < 1) {
		condition_.notify_one();
	}
}