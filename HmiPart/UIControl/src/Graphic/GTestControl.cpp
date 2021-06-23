#include "stdafx.h"
#include "Message.h"
#include "HMIWindow.h"
#include "GTestControl.h"
#include "GTestView.h"

namespace UI
{
	static shared_ptr<GTestModel> Mode;
	GTestControl::GTestControl(UI::HMIWindow* w) :BaseControl(w)
	{
		//创建GTestModel
		Mode = std::shared_ptr < GTestModel>(new GTestModel());
		InitMVCModel(Mode);
	}
	 void GTestControl::CreateView()
	{
		//初始化VIEW
		 shared_ptr < GTestView> view(new GTestView(Mode->m_testConfig.X, Mode->m_testConfig.Y,
			Mode->m_testConfig.Width, Mode->m_testConfig.Height,"bt"));
		InitMVCView(view);
	}
	int GTestControl::PeekHMIMessage(Message::Msg* msg)
	{
		 
		return 1;//handled;
	}
	void GTestControl::AddVarData(vector<Project::DataVarId*> vars)
	{
	}
}
