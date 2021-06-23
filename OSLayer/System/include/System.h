#pragma once
#include <string>
#ifdef _MSC_VER
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif
class System
{
public:
	static void Init();

	/******************时间相关***************************/
	//获取当前时间戳，单位秒
	static unsigned long long GetCurrentTimeStamp();
	//获取当前时间戳，单位毫秒
	static unsigned long long GetCurrentTimeStampMs();
	//获取从Init()调用到当前花费的时间，单位毫秒
	static unsigned long long GetElapseTickMs();
	//获取当前年月日，直接将当前的日期写入参数
	static void GetCurrentDate(int& year, int& month, int& day, int& week);
	//获取当前时分秒，直接将当前的时间写入参数
	static void GetCurrentClock(int& hour, int& minute, int& sec, int& milsec);

	//获取当前日期的格式化字符串，样式：YYYY-MM-DD
	static std::string GetCurrentDateToString();
	//获取当前时间的格式化字符串，样式：hh:mm:ss
	static std::string GetCurrentTimeToString();
	//获取当前日期时间的格式化字符串，样式YYYY-MM-DD hh:mm:ss
	static std::string GetCurrentDateTimeToString();

	//获取指定时间戳的日期的格式化字符串,样式：YYYY-MM-DD
	static std::string GetDateToString(time_t time);
	//获取指定时间戳的时间的格式化字符串，样式：hh:mm:ss
	static std::string GetTimeToString(time_t time);
	//获取指定时间戳的日期时间的格式化字符串，样式YYYY-MM-DD hh:mm:ss
	static std::string GetDateTimeToString(time_t time);
	//获取指定日期的格式化字符串的时间戳,样式：YYYY-MM-DD
	static time_t GetTimeStampFromDateString(const std::string& str);
	//获取指定时间的格式化字符串的时间戳，样式：hh:mm:ss
	static time_t GetTimeStampFromTimeString(const std::string& str);
	//获取指定日期时间的格式化字符串的时间戳，样式YYYY-MM-DD hh:mm:ss
	static time_t GetTimeStampFromDateTimeString(const std::string& str);

	//获取距离下一天节点的毫秒数
	static unsigned long long GetNextDayNodeMs();
	//获取距离下一小时节点的毫秒数
	static unsigned long long GetNextHourNodeMs();
	//获取距离下一分钟节点的毫秒数
	static unsigned long long GetNextMinuteNodeMs();
	//获取距离下一秒节点的毫秒数
	static unsigned long long GetNextSecondNodeMs();

	//根据秒数获取对应时间日期
	static tm GetFormatTimeBySec(time_t time);



	/******************文件相关***************************/
	//获取当前执行程序所在目录+realpath的绝对路径。
	static char* GetFilePath(const char* realpath,char* abspath);
	//判断文件是否存在
	static bool IsFileExist(const char* file_path);
	//判断目录是否存在
	static bool IsDirectoryExist(const char* file_path);
	//重命名路径上最后节点目录
	static bool RenameDirector(const char* src_path, const char* des_path);
	//重命名文件
	static bool RenameFile(const char* src_file_path, const char* des_file_path);
	//删除文件
	static bool RemoveFile(const char* file_path);
	//目录存在直接返回TURE,否则创建
	static bool MakeSureDirectoryExist(const char* dir_path);
	//获取文件路径的目录
	static std::string GetFileDirectory(const char* file_path);

	static std::string GetCurDir();
	static bool IsAbsPath(const char* path);
	static std::string GetAbsPath(const char* path);
	static std::string CombinPath(std::string dir, std::string name);
	static std::string GetLibExt();

	/******************UI相关***************************/
	static int ScreenHeight();
	static int ScreenWidth();
	



private:
	static unsigned long long appStartTick_;
};

