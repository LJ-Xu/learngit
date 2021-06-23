#pragma once
namespace UI
{
	struct DevCommState
	{
		int Success; // 成功次数
		int Error; //失败次数
		int Req; //请求次数
		unsigned int Tick; //时间戳,计算最近2S中状态
		DevCommState() {}
		DevCommState(unsigned int tc):Tick(tc)
		{
		
		}
	};
	enum DeviceDataState
	{
		Dev_State_Req, Dev_State_Success, Dev_State_Error, Dev_State_Timeout
	};

	class CommStateMgr
	{
	public:
		static void State(short devid, short stano, short state);
		static bool IsOK(short devid, short stano);
	private:
		static std::map<unsigned int, DevCommState> devstate_;
	};
}

