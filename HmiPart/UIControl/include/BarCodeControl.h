#pragma once
#include "BaseControl.h"
#include "BarCodeModel.h"
namespace UI
{
	class BarCodeControl :public BaseControl
	{
	public:
		BarCodeControl() :BarCodeControl(nullptr) {}
		BarCodeControl(HMIPage*);
		void CreateView()override;
		bool HandleSysChange(SysChangeEM catogray);
		void OnReady() override;

	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<BarCodeModel> mode_;
	};

}
