#include "SimpleIni/SimpleIni.h"
#include "System.h"
#include "Comm.h"
#include "RunEnv.h"
RunConfig  RunEnv::Cnf;
SystemInfo RunEnv::SysInfo;
HMIInfo  RunEnv::HMIApp;
#define INI_NAME "HMIMain.ini"

bool RunEnv::Init()
{
	CSimpleIniA ini;
	std::string path = System::GetCurDir() + PATH_SEPARATOR + INI_NAME;
	SI_Error rc = ini.LoadFile(path.c_str());
	if (rc < 0)
	{
		//ÅäÖÃÎÄ¼þ´íÎó
		return false;
	};
	const char* pv;
	pv = ini.GetValue("run", "BinPath"); if (pv == nullptr)return false;
	Cnf.BinPath = System::GetAbsPath(pv);
	pv = ini.GetValue("run", "SrvIP"); if (pv == nullptr)return false;
	Cnf.SrvIP = pv;
	pv = ini.GetValue("run", "SrvPort"); if (pv == nullptr)return false;
	int port = 0;
	if(sscanf(pv, "%d", &port)>0)
		Cnf.SrvPort = port;
	pv = ini.GetValue("log", "CnfPath"); if (pv == nullptr)return false;
	Cnf.LogCnfPath = System::GetAbsPath(pv);


}