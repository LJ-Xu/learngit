#include "System.h"
#include <SysDateTime.h>
#include <SysFile.h>
#ifndef _MSC_VER
#include <unistd.h> //for readlink
#include <string.h>
#include <linux/limits.h>
#endif
#include <iostream>

unsigned long long System::appStartTick_;

void System::Init()
{
	appStartTick_ = GetCurrentTimeStampMs();
}

unsigned long long System::GetCurrentTimeStamp()
{
	return base::datetime().now().gettimestamp();
}

unsigned long long System::GetCurrentTimeStampMs()
{
	return base::datetime().now().gettimestamp_ms();
}

unsigned long long System::GetElapseTickMs()
{
	return GetCurrentTimeStampMs() - appStartTick_;
}

void System::GetCurrentDate(int& year, int& month, int& day, int& week)
{
	base::date d;
	d.now();
	year = d.year;
	month = d.month;
	day = d.day;
	week = d.wday;
}

void System::GetCurrentClock(int& hour, int& minute, int& sec, int& milsec)
{
	base::time t;
	t.now();
	hour = t.hour;
	minute = t.minute;
	sec = t.second;
	milsec = t.millisecond;
}

std::string System::GetCurrentDateToString()
{
	return base::date().now().str(1);
}

std::string System::GetCurrentTimeToString()
{
	return base::time().now().str(1);
}

std::string System::GetCurrentDateTimeToString()
{
	return base::datetime().now().str(1);
}

std::string System::GetDateToString(time_t time)
{
	return base::datetime(time).m_date.str(1);
}

std::string System::GetTimeToString(time_t time)
{
	return base::datetime(time).m_time.str(1);
}

std::string System::GetDateTimeToString(time_t time)
{
	return base::datetime(time).str(1);
}

time_t System::GetTimeStampFromDateString(const std::string& str)
{
	return base::date().setvalue(str).gettimestamp();
}

time_t System::GetTimeStampFromTimeString(const std::string& str)
{
	return base::time().setvalue(str).gettimestamp();
}

time_t System::GetTimeStampFromDateTimeString(const std::string& str)
{
	return base::datetime().setvalue(str).gettimestamp();
}

unsigned long long System::GetNextDayNodeMs()
{
	base::time t;
	t.now();
	return (23 - t.hour) * 3600000 + (59 - t.minute) * 60000 + (60 - t.second) * 1000 - t.millisecond;
}

unsigned long long System::GetNextHourNodeMs()
{
	base::time t;
	t.now();
	return (59 - t.minute) * 60000 + (60 - t.second) * 1000 - t.millisecond;
}

unsigned long long System::GetNextMinuteNodeMs()
{
	base::time t;
	t.now();
	return (60 - t.second) * 1000 - t.millisecond;
}

unsigned long long System::GetNextSecondNodeMs()
{
	base::time t;
	t.now();
	return 1000 - t.millisecond;
}

tm System::GetFormatTimeBySec(time_t time)
{
	tm p;
#ifdef _MSC_VER
	localtime_s(&p, &time);
#else
	localtime_r(&time, &p);
#endif
	return p;
}

#ifdef _MSC_VER
/******************************************************************************************
Function:        ConvertLPWSTRToLPSTR
Description:     LPWSTR转char*
Input:           lpwszStrIn:待转化的LPWSTR类型
Return:          转化后的char*类型
*******************************************************************************************/
char* ConvertLPWSTRToLPSTR(LPWSTR lpwszStrIn,char* buf)
{
	LPSTR pszOut = NULL;
	try
	{
		if (lpwszStrIn != NULL)
		{
			int nInputStrLen = wcslen(lpwszStrIn);

			// Double NULL Termination  
			int nOutputStrLen = WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, NULL, 0, 0, 0) + 2;
			//pszOut = new char[nOutputStrLen];

			if (buf)
			{
				memset(buf, 0x00, nOutputStrLen);
				WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, buf, nOutputStrLen, 0, 0);
			}
		}
	}
	catch (std::exception e)
	{
	}

	return buf;
}
#endif
char* System::GetFilePath(const char* realpath, char* abspath)
{
	char *p = NULL;
#ifdef _WIN32
	//TCHAR path[MAX_PATH] = { 0 };
	//GetModuleFileName(NULL, path, MAX_PATH);
	//ConvertLPWSTRToLPSTR(path, abspath);
	if (abspath) GetModuleFileName(NULL, abspath, MAX_PATH);
	if (NULL != (p = strrchr(abspath, '\\')))
#else
	readlink("/proc/self/exe",abspath,256);
	if (NULL != (p = strrchr(abspath, '/')))
#endif
	{
		*p = '\0';
	}
	else
	{
		std::cerr << "process path is wrong" << std::endl;
	}
	strcat(abspath, realpath);
	return abspath;
}

bool System::IsFileExist(const char* file_path)
{
	return base::file::is_file_exist(file_path);
}

bool System::IsDirectoryExist(const char* file_path)
{
	return base::file::is_directory_exist(file_path);
}

bool System::RenameDirector(const char* src_path, const char* des_path)
{
	return base::file::rename_director(src_path, des_path);
}

bool System::RenameFile(const char* src_file_path, const char* des_file_path)
{
	return base::file::rename_file(src_file_path, des_file_path);
}

bool System::RemoveFile(const char* file_path)
{
	return base::file::remove_file(file_path);
}

bool System::MakeSureDirectoryExist(const char* dir_path)
{
	return base::file::make_sure_directory_exist(dir_path);
}

std::string System::GetFileDirectory(const char* file_path)
{
	return base::file::file_directory(file_path);
}

int System::ScreenHeight()
{
#ifdef _MSC_VER
	return 	GetSystemMetrics(SM_CYSCREEN);//获取屏幕高度
#else

#endif
}
int System::ScreenWidth()
{
#ifdef _MSC_VER
	return GetSystemMetrics(SM_CXSCREEN); //获取屏幕宽度
#else
#endif
}
std::string System::GetCurDir()
{
#ifdef _MSC_VER
	std::string strCurPath;
	char szBuffer[1024];
	memset(szBuffer, 0x00, sizeof(szBuffer));
	DWORD dwSize = GetModuleFileName(NULL, szBuffer, 1024);
	szBuffer[dwSize] = 0;
	while (szBuffer[dwSize] != '\\' && dwSize != 0) {
		szBuffer[dwSize] = 0; dwSize--;
	}
	//去掉最后一个"\\"
	if (0 <= dwSize) {
		szBuffer[dwSize] = 0;
	}
	strCurPath = szBuffer;
	return strCurPath;
#else
    std::string strDir;
	char szBuffer[PATH_MAX];
	int nLen = readlink("/proc/self/exe", (char*)szBuffer, PATH_MAX);
	if (nLen < 0 || nLen >= PATH_MAX) {
		return NULL;
	}
	while (szBuffer[nLen] != '/')   nLen--;
	szBuffer[nLen] = '\0';
	strDir = szBuffer;
	return strDir;
#endif
}
bool System::IsAbsPath(const char* path)
{
	if (!path)return false;
	if (path[0] == '\\' || path[0] == '/')
		return true;
	return false;
}
std::string System::GetAbsPath(const char* path)
{
	if (!IsAbsPath(path))
	{
		if (path[0] == '.')
		{
			return  System::GetCurDir() + (path + 1);
		}
		else
			return  System::GetCurDir() + PATH_SEPARATOR + path;

	}

	return path;
}

std::string System::CombinPath(std::string dir, std::string name)
{
	return  dir+ PATH_SEPARATOR + name;
}

std::string System::GetLibExt()
{
#ifdef _MSC_VER
	return ".dll";
#else
	return ".so";
#endif

}
