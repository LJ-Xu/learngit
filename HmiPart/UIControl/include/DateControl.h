#pragma once
#include "BaseControl.h"
#include "DateView.h"
#include "DateModel.h"
namespace UI
{
	class DateControl : public BaseControl
	{
	public:
		DateControl() :DateControl(nullptr) {}
		DateControl(HMIPage*  );
		void CreateView()override;
		void StartTick(bool firstStart = false);
		void StopTick();
		bool HandleSysChange(SysChangeEM catogray);
		void OnReady() override;
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
	private:
		shared_ptr <DateModel> mode_;
	};

}
