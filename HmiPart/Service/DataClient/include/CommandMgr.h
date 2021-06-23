#pragma once
namespace HMI
{
	class PackageMgr;
	typedef void(*CMDHandler)(PackageMgr* mgr, char* buf, int datalen);
	struct CMDInfo
	{
		short Cmd;
		CMDHandler Handler;
	};
	 
	class CommandMgr
	{
	public:
		static void ProcessCMD(int cmd, PackageMgr* mgr, char* buf, int len);
	};
}


