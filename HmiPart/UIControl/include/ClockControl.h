#pragma once
#include "BaseControl.h"
#include "ClockModel.h"
#include "ClockView.h"
namespace UI
{
	class ClockControl :
		public BaseControl
	{
	public:
		ClockControl() :ClockControl(nullptr){}
		ClockControl(HMIPage*);
		void CreateView()override;
		void StartTick(bool firstStart=false);
		void StopTick();
		void OnReady() override;
		bool HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int pageoffx, int pageoffy);
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
	private:
		shared_ptr <ClockModel> mode_;
	};
}
