#pragma once
#include <BaseControl.h>
#include "DynamicTextBarModel.h"
#include "StatusRes.h"
namespace UI
{
	class DynamicTextBarControl :public BaseControl
	{
	public:
		DynamicTextBarControl() :DynamicTextBarControl(nullptr) {}
		DynamicTextBarControl(HMIPage*);
		void CreateView()override;
		void StartScoll();
		void Stop();
		void OnReady()override;
		void HandleSysChange(SysChangeEM catogray);
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		//void InitAlarmData(Project::DataVarId& varid);
	public:
		bool IsRuning;
	};
}

