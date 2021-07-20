#pragma once
#include "BaseControl.h"
#include "DashboardModel.h"
#include "DashboardView.h"
namespace UI
{
	class DashboardControl :public BaseControl
	{
	public:
		DashboardControl() :DashboardControl(nullptr) {}
		DashboardControl(HMIPage*);
		void CreateView()override;
		void OnReady();
		bool HandleSysChange(SysChangeEM catogray);
	protected:
		//void AddVarData(vector<Project::DataVarId*> vars) override;
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<DashboardModel> mode_;
	};
}


