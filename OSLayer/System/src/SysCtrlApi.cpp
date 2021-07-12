#include "SysCtrlApi.h"

#ifdef _WIN32
#include<windows.h>
#else
#include <stdlib.h>
#include <fcntl.h>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/input.h>
//#include <X11/XKBlib.h>
#endif
#include <iostream>
#include <thread>

#define event_name "/dev/input/event1"
#define brightness_name  "/sys/class/backlight/backlight_gpio/brightness"
void BeepHelper::Start()
{
	runflag_ = true;
	td_ = new std::thread(std::bind(&BeepHelper::Run,this));
	
}
BeepHelper& BeepHelper::Ins()
{
	static BeepHelper bh;
	return bh;
}
void BeepHelper::Run()
{
	while (runflag_)
	{
		
		if (beepflag_)
		{
#ifdef WIN32
			SysCtrlApi::Beep(0, 0);
#else
			SysCtrlApi::Beep(100, 800);
#endif // WIN32
			//std::lock_guard<std::mutex> lck(mutex_);
			beepflag_ = false;
		}
		else
		{
			std::unique_lock<std::mutex> lck(mutex_);
			cvar_.wait(lck);	
		}
	}
}
void BeepHelper::Stop()
{
	runflag_ = false;
}
void BeepHelper::Beep()
{
	//std::lock_guard<std::mutex> lck(mutex_);
	beepflag_ = true;
	cvar_.notify_all();
}
void SysCtrlApi::Beep()
{
	BeepHelper::Ins().Beep();
}

void SysCtrlApi::Beep(int ms, int hz)
{
#ifdef _WIN32
	for (int i = 0; i < ms; ++i)
	{
		::Beep(450, 400);
	}
#else
	int fd, ret;
	struct input_event event;

	if ((fd = open(event_name, O_RDWR)) < 0)
		return;

	event.type = EV_SND;
	event.code = SND_TONE;
	event.value = hz;
	ret = write(fd, &event, sizeof(struct input_event));

	usleep(ms * 1000);
	event.value = 0;
	ret = write(fd, &event, sizeof(struct input_event));

	close(fd);
#endif
}
void SysCtrlApi::OpenBacklight()
{
#ifndef WIN32
	int fd, ret;
	if ((fd = open(brightness_name, O_RDWR)) < 0)
		return;
	int open = 1;
	ret = write(fd, &open, sizeof(open));
	close(fd);
#endif
}
void SysCtrlApi::CloseBacklight()
{
#ifndef WIN32
	int fd, ret;
	if ((fd = open(brightness_name, O_RDWR)) < 0)
		return;
	int open = 0;
	ret = write(fd, &open, sizeof(open));
	close(fd);
#endif
}
void SysCtrlApi::SwitchSysTime(int src, int mode, std::vector<int>* time)
{
}
//0[关]-100[开]  
void SysCtrlApi::Backlight(int percent)
{
	
}
void SysCtrlApi::SetTime(int h, int m, int s)
{
	
}
void SysCtrlApi::SetData(int y, int m, int d)
{
	
}
const char* SysCtrlApi::SN()
{
	return "124";
}
unsigned int SysCtrlApi::Tick()
{
	return 1;
}

bool SysCtrlApi::IsKeyboardCapslock()
{
	bool iscapslock = false;
#ifdef _MSC_VER
	iscapslock = (GetKeyState(VK_CAPITAL) != 0);
#else
	//Display * d = XOpenDisplay((char*)0);
	//if (d)
	//{
	//	unsigned n;
	//	XkbGetIndicatorState(d, XkbUseCoreKbd, &n);
	//	iscapslock = ((n & 1) != 0);
	//}
#endif
	return iscapslock;
}

