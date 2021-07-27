#pragma once
#include <vector>
#include <map>
namespace Project
{
	struct HMIProject;
}
class Semaphore;
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
	enum WaitReqEM:int
	{
		WR_State_OK,WR_State_Continue_Next,WR_State_Timeout, WR_State_Continue_Wait, WR_State_Param_Err
	};
	struct ComCond
	{
		int CanCnt;//可以的个数
		unsigned int Tick;//上次请求时间
	};
	class CommStateMgr
	{
	public:
		static bool Wait(int ms);
		static void Signal();

		static void Init(Project::HMIProject* prj,int canreqcnt=2);

		static int Req(short devid, short stano=0);
		static void Resp(short devid, short stano=0);
		static ComCond Get(short devid, short stano=0);
		static void SetCanCount(int count, short devid, short stano=0);
		//static bool IsRespTimeout();
	private:
		//static std::vector<DevCommStateInfo> devstate_;
		static Semaphore* sem_;
		static std::vector<ComCond> devcondition_;
	};
}

