#pragma once
#include <functional>
#include "SystemInfo.h"
#include "HMIVersion.h"

struct RunConfig
{
	RunConfig():SrvIP("127.0.0.1"), SrvPort(8800),IsDebug(false){}
	std::string BinPath;
	std::string SrvIP ;
	int   SrvPort;
	std::string LogCnfPath;
	bool IsDebug; //是否为debug方式运行
	std::string AlarmPath;
	std::string SamplePath;
	std::string OperationPath;
};
struct HMIEvent
{
	std::function<void()> OnAppStart;
	std::function<void()> OnAppClose;
};
struct HMIInfo
{
	struct HMIEvent   Event;
	struct HMIVersion Ver;
};
class RunEnv
{
public:
	static RunConfig  Cnf;
	static SystemInfo SysInfo;
	static HMIInfo  HMIApp;
	static bool Init();
}; 

