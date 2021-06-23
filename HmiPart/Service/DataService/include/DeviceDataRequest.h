#pragma once
#include <atomic>
#include "DataFrame.h"
namespace Project
{
	struct BaseVar;
}
namespace UI
{
	struct DevCommState
	{
		int Success; // 成功次数
		int Error; //失败次数
		int Req; //请求次数
		int Tick; //时间戳,计算最近2S中状态

	};
	enum DeviceDataState
	{
		Dev_State_Req, Dev_State_Success, Dev_State_Error, Dev_State_Timeout
	};
	class DeviceDataRequest
	{
	public:
		static void Init();
		static void State(int devid,int stano,int state);
	private:
		//
		std::map<unsigned int, DevCommState> DevStates;
	};
}
