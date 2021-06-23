#pragma once
#ifdef _MSC_VER
#include <windows.h>
#else
#include <unistd.h>
#include <X11/XKBlib.h>
#endif

#include <mutex>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <functional>
class SysCtrlApi
{
public:
	static void Beep();
	static void Beep(int time, int hz = 0);
	//0[¹Ø]-100[¿ª]  
	static void Backlight(int percent);
	static void SetTime(int h, int m, int s);
	static void SetData(int y, int m, int d);
	static const char* SN();		
	static unsigned int Tick();
	static bool IsKeyboardCapslock();
private :
	
	
};
class BeepHelper
{
public:
	static BeepHelper& Ins();
	BeepHelper():td_(nullptr), runflag_(false), beepflag_(false)
	{
	
	}
	void Stop();
	void Start();
	void Beep();
private:
	void Run();
	std::thread* td_;
	bool runflag_;
	bool beepflag_;
	std::mutex mutex_;
	std::condition_variable cvar_;
};

