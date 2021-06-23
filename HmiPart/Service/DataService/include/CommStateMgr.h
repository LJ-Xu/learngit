#pragma once
namespace UI
{
	struct DevCommState
	{
		int Success; // �ɹ�����
		int Error; //ʧ�ܴ���
		int Req; //�������
		unsigned int Tick; //ʱ���,�������2S��״̬
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

