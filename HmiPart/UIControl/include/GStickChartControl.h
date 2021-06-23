#pragma once
#include "BaseControl.h"
#include "GStickChartModel.h"

namespace UI
{
	class GStickChartControl :public BaseControl
	{
	public:
		GStickChartControl() :GStickChartControl(nullptr) {}
		GStickChartControl(HMIPage*  );
		void CreateView()override;
		void OnReady();
		void HandleSysChange(SysChangeEM catogray);
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<GStickChartModel> mode_;
	};

}