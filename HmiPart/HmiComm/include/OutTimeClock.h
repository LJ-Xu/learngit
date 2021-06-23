#pragma once
#include <mutex>
#include <condition_variable>
#pragma once
class OutTimeClock
{
public:
	OutTimeClock() {}
	~OutTimeClock() {}
public:
	inline void wait()
	{
		std::unique_lock<std::mutex> unique_lck(mutex_);
		//func();
		condition_.wait(unique_lck);
	}
	template<typename Fn>
	inline void wait(unsigned int second, Fn& func)
	{
		std::unique_lock<std::mutex> unique_lck(mutex_);
		//auto now = std::chrono::system_clock::now();
		if (condition_.wait_for(unique_lck, std::chrono::seconds(second)) == std::cv_status::timeout)
			func();
		//std::cv_status::timeout
		return;
	}
	inline void wait(unsigned int second)
	{
		std::unique_lock<std::mutex> unique_lck(mutex_);
		//auto now = std::chrono::system_clock::now();
		condition_.wait_for(unique_lck, std::chrono::seconds(second));
	}
	void signal()
	{
		std::lock_guard<std::mutex> lck(mutex_);
		condition_.notify_all();
	}
private:
		std::mutex mutex_;
		std::condition_variable condition_;
};

