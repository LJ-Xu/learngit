#pragma once
#include <mutex>
#include <condition_variable>
class Semaphore
{
public:
	explicit Semaphore(unsigned int count); 
	~Semaphore();
public:
	void wait();
	void signal();
	//³¬Ê±·µ»Øtrue
	inline bool wait(unsigned int second)
	{
		std::unique_lock<std::mutex> unique_lck(mutex_);
		//auto now = std::chrono::system_clock::now();
		if (--count_ < 0)
		{
			if (condition_.wait_for(unique_lck, std::chrono::seconds(second)) == std::cv_status::timeout)
			{
				++count_;
				return false;
			}
		}
		
		return true;
	}
private:
	int count_;  
	std::mutex mutex_;
	std::condition_variable condition_;
};