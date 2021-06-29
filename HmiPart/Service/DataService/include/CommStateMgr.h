#pragma once
#include <vector>
#include <map>
namespace Project
{
	struct HMIProject;
}
namespace UI
{
	struct DevCommStateInfo
	{
		long long RespTick;//ms
		long long ReqTick;//ms
		int ReqIdx;
		std::map<int, DevCommStateInfo> SubDevs;
		DevCommStateInfo():ReqIdx(0), RespTick(0), ReqTick(0){}
	};
	enum DeviceDataStateEM
	{
		Dev_State_Req, Dev_State_Resp,Dev_State_Success, Dev_State_Error, Dev_State_Timeout
	};
	enum RespStateEM
	{
		Resp_State_OK, Resp_State_Timeout, Resp_State_Wait, Resp_State_ERR
	};
	class CommStateMgr
	{
	public:
		static void Init(Project::HMIProject* prj);
		static void State(short devid, short stano, short state);
		//static bool IsRespTimeout();
		//是否可以继续请求
		static RespStateEM RespState(short devid, short stano);
	private:
		static std::vector<DevCommStateInfo> devstate_;
	};
}

