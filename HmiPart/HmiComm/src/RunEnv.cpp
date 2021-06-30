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
	Cnf.SrvIP = "127.0.0.1";
	Cnf.SrvPort = 8800;
	Cnf.UIPort = 9000;
	if (rc < 0)
	{
		//ÅäÖÃÎÄ¼þ´íÎó
		return false;
	};
	const char* pv;
	int port = 0;
	pv = ini.GetValue("run", "BinPath");
	if (pv != nullptr)
		Cnf.BinPath = System::GetAbsPath(pv);

	pv = ini.GetValue("run", "SrvIP");
	if (pv != nullptr)
		Cnf.SrvIP = pv;

	pv = ini.GetValue("run", "SrvPort");
	if (pv != nullptr && sscanf(pv, "%d", &port) > 0)
		Cnf.SrvPort = port;

	pv = ini.GetValue("run", "UIPort"); 
	if (pv != nullptr && sscanf(pv, "%d", &port) > 0)
		Cnf.UIPort = port;

	pv = ini.GetValue("run", "AlarmPath");
	if (pv != nullptr)
		Cnf.AlarmPath = System::GetAbsPath(pv);
	
	pv = ini.GetValue("run", "SamplePath");
	if (pv != nullptr)
		Cnf.SamplePath = System::GetAbsPath(pv);

	pv = ini.GetValue("run", "OperationPath");
	if (pv != nullptr)
		Cnf.OperationPath = System::GetAbsPath(pv);

	pv = ini.GetValue("run", "RecipePath");
	if (pv != nullptr)
		Cnf.RecipePath = System::GetAbsPath(pv);

	pv = ini.GetValue("run", "PinYinDataPath");
	if (pv != nullptr)
		Cnf.PinYinDataPath = System::GetAbsPath(pv);

	pv = ini.GetValue("log", "CnfPath"); 
	if (pv != nullptr)
		Cnf.LogCnfPath = System::GetAbsPath(pv);
	return true;

}
