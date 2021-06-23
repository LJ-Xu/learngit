#pragma once
#include "BaseControl.h"
#include "MotorModel.h"
#include "MotorView.h"

namespace UI
{
	class MotorControl :public BaseControl
	{
	public:
		MotorControl() :MotorControl(nullptr) {}
		MotorControl(HMIPage*);
		void CreateView()override;
		void BlinkMoment();
		void OnReady() override;
		void HandleSysChange(SysChangeEM catogray);
	public:
		bool IsBlinking;
		double BlinkSpeed;
	protected:
		//void AddVarData(vector<Project::DataVarId*> vars) override;
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		void BlinkTick(MotorModel* pMode);
	private:
		std::shared_ptr<MotorModel> mode_;
	};
}