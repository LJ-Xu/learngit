#include <iostream>
#include <string.h>
#include "System.h"
#include <chrono>
#include <thread>
#include "SysFile.h"
#include "SysCtrlApi.h"

void TestDateTime()
{
	auto t = System::GetCurrentTimeStamp();
	std::cout << System::GetDateTimeToString(t) << std::endl;

	System::Init();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << System::GetElapseTickMs() << std::endl;

	int year, month, day, wday;
	System::GetCurrentDate(year, month, day, wday);
	int hour, minute, second, ms;
	System::GetCurrentClock(hour, minute, second, ms);
	char buf[64] = { 0 };
	sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d.%03d", year, month, day, hour, minute, second, ms);
	std::cout << buf << std::endl;

	std::cout << System::GetCurrentDateToString() << std::endl;
	std::cout << System::GetCurrentTimeToString() << std::endl;
	std::cout << System::GetCurrentDateTimeToString() << std::endl;

	std::cout << System::GetDateToString(t) << std::endl;
	std::cout << System::GetTimeToString(t) << std::endl;
	std::cout << System::GetDateTimeToString(t) << std::endl;

	std::cout << System::GetTimeStampFromDateString("2021-04-08") << std::endl;
	std::cout << System::GetTimeStampFromTimeString("12:53:30") << std::endl;
	std::cout << System::GetTimeStampFromDateTimeString("2021-04-08 12:53:30") << std::endl;

	std::cout << System::GetNextDayNodeMs() << std::endl;
	std::cout << System::GetNextHourNodeMs() << std::endl;
	std::cout << System::GetNextMinuteNodeMs() << std::endl;
	std::cout << System::GetNextSecondNodeMs() << std::endl;

	tm tt = System::GetFormatTimeBySec(t);
	char buftt[64] = { 0 };
	sprintf(buftt, "%04d-%02d-%02d %02d:%02d:%02d", tt.tm_year + 1900, tt.tm_mon + 1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec);
	std::cout << buftt << std::endl;
}

void TestFile()
{
	char buf[256] = { 0 };
#ifdef _MSC_VER
	System::GetFilePath("\\config\\xxx.xml", buf);
#else
	System::GetFilePath("/config/xxx.xml", buf);
#endif
	std::cout << buf << std::endl;

	std::string filename("ddd.txt");
	std::string filename_des("bbb.txt");
#ifdef _MSC_VER
	std::string pathname("config\\xxxx\\yyyy\\");
	std::string pathname_des("config\\xxxx\\y\\");
#else
	std::string pathname("config/xxxx/yyyy/");
	std::string pathname_des("config/xxxx/y/");
#endif
	std::cout << System::IsFileExist(filename.c_str()) << std::endl;
	std::cout << System::IsDirectoryExist(pathname.c_str()) << std::endl;
	std::cout << System::MakeSureDirectoryExist(pathname.c_str()) << std::endl;
	std::cout << System::IsDirectoryExist(pathname.c_str()) << std::endl;
	std::cout << System::RenameDirector(pathname.c_str(), pathname_des.c_str()) << std::endl;
	std::cout << System::IsDirectoryExist(pathname.c_str()) << std::endl;
	std::cout << System::IsDirectoryExist(pathname_des.c_str()) << std::endl;

	base::file f;
	f.open(filename.c_str(), "ab+");
	if (!f.null())
	{
		f.close();
		std::cout << System::IsFileExist(filename.c_str()) << std::endl;
		std::cout << System::RenameFile(filename.c_str(), filename_des.c_str()) << std::endl;
		std::cout << System::IsFileExist(filename.c_str()) << std::endl;
		std::cout << System::IsFileExist(filename_des.c_str()) << std::endl;
		std::cout << System::RemoveFile(filename_des.c_str()) << std::endl;
		std::cout << System::IsFileExist(filename_des.c_str()) << std::endl;
	}
#ifdef _MSC_VER
	std::cout << System::GetFileDirectory("C:\\code\\oslayer\\aaaa.txt") << std::endl;
#else
	std::cout << System::GetFileDirectory("/mnt/c/code/oslayer/aaaa.txt") << std::endl;
#endif
}

int main()
{
	SysCtrlApi::Beep(5);

	TestDateTime();
	TestFile();


	getchar();
	return 0;
}