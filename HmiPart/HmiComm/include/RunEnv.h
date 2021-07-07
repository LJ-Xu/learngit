#pragma once
#include <functional>
#include "SystemInfo.h"
#include "HMIVersion.h"

struct RunConfig
{
	RunConfig():SrvIP("127.0.0.1"), SrvPort(8800), UIPort(9000),IsDebug(false){}
	std::string BinPath;
	std::string SrvIP ;
	unsigned short   SrvPort;
	unsigned short   UIPort;
	std::string LogCnfPath;
	bool IsDebug; //�Ƿ�Ϊdebug��ʽ����
	bool IsOffLine;		//�Ƿ�����ģʽ
	std::string AlarmPath;
	std::string SamplePath;
	std::string OperationPath;
	std::string RecipePath;
	std::string PinYinDataPath;
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

