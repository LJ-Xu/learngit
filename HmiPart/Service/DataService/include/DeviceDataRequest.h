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
		int Success; // �ɹ�����
		int Error; //ʧ�ܴ���
		int Req; //�������
		int Tick; //ʱ���,�������2S��״̬

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
