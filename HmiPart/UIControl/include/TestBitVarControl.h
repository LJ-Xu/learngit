#pragma once
#include "BaseControl.h"
#include "TestBitVarModel.h"

namespace UI
{
	class TestBitVarControl :public BaseControl
	{
	public:
		TestBitVarControl() :TestBitVarControl(nullptr) {}
		TestBitVarControl(HMIPage*);
		void CreateView()override;
		void HandleDataVar(Project::DataVarId &varId);
		vector<string> ChineseString;
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
	};
}

