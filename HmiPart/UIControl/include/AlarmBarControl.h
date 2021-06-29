#pragma once
#include <BaseControl.h>
#include "AlarmBarModel.h"
#include "AlarmInfoRes.h"
#include "StatusRes.h"
namespace UI
{
	class AlarmBarControl :public BaseControl
	{
	public:
		AlarmBarControl() :AlarmBarControl(nullptr) {}
		AlarmBarControl(HMIPage*);
		void CreateView()override;
		void StartScoll();
		void Stop();
		void OnReady()override;
		void HandleSysChange(SysChangeEM catogray);
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		void InitAlarmData(Project::DataVarId& varid);
	public:
		bool IsNewAlarm;
		bool IsRuning;
	};
}

