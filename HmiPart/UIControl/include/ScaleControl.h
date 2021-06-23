#pragma once
#include "BaseControl.h"
#include "ScaleModel.h"
#include "ScaleView.h"

namespace UI
{
	class ScaleControl :public BaseControl
	{
	public:
		ScaleControl() :ScaleControl(nullptr) {}
		 ScaleControl(HMIPage*);
		void CreateView()override;
		void HandleSysChange(SysChangeEM catogray);
		void OnReady();
	protected:
		//void AddVarData(vector<Project::DataVarId*> vars) override;
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<ScaleModel> mode_;
	};

}