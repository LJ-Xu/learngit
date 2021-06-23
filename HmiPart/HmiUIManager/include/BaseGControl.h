#pragma once
#include "DataVarInfo.h"
#include "BaseControl.h"
namespace Message
{
	struct Msg;
}
namespace UI
{
	enum GCtrlEnum
	{
		SAMPLE = 0,//���ݲ���
		ALARM,//����
		PRINTER,//��ӡ
		GCTRL_SIZE,
	};
	class HMIWindow;
	class BaseGControl : public BaseControl
	{
	public:
		BaseGControl(){}
		BaseGControl(HMIPage * pg) { pPage_ = pg; };
		void SetHMIWindow(HMIPage * pg) { pPage_ = pg; };
		virtual ~BaseGControl() {};
		virtual int PeekHMIMessage(Message::Msg* msg) = 0;
		virtual void HandleDataNotify(Project::DataVarId id) = 0;
	private:
		HMIWindow* pWindow_ = nullptr;
	};

}

