#include "VersionMgr.h"
#include <string>
//Version hmiVer = {0,1,1}; //HMI����汾
HMIVersion hmiVer = { 0,1,15 }; //HMI����汾
std::string GetVersionInfo()
{
	std::string versionNo = 'V' + std::to_string(hmiVer.Main) + '.' + std::to_string(hmiVer.Sub) + '.' + std::to_string(hmiVer.Order)+'.'+std::string(__DATE__);
	return versionNo;
}