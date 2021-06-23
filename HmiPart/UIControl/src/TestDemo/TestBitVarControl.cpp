/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TestBitVarControl.cpp
 * Author   : qiaodan
 * Date     : 2021-01-19
 * Descript : 创建测试按键绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "Message.h"
#include "HMIWindow.h"
#include "TestBitVarControl.h"
#include "TestBitVarView.h"
#include "UIData.h"
#include <time.h> 
namespace UI
{
	TestBitVarControl::TestBitVarControl(HMIPage* w) :BaseControl(w)
	{
		//创建TestBitVarModel
		std::shared_ptr <TestBitVarModel> mode(new TestBitVarModel());
		InitMVCModel(mode);
	}
	string randstr(int len)
	{
		srand((size_t)time(NULL));
		int i;
		string data;
		for (i = 0; i < len; ++i)
		{
			data  += ('A' + rand() % 26);
		}
		//str[++i] = '\0';
		
		//static int cnt = 0;
		//sprintf(str, ">>%d", cnt++);
		return data;
	}

	void TestVarClickCB(Fl_Widget* view, void* param)
	{
		HMIPage* pg = (HMIPage*)param;
		TestBitVarView *v = (TestBitVarView *)view;
		std::shared_ptr <TestBitVarModel> md = v->BaseView.GetModel<TestBitVarModel>();
		std::shared_ptr <TestBitVarControl> ctrl = v->BaseView.GetControl<TestBitVarControl>();
		if (md->BitConfig.Type == Project::VarType::WordVar)	//设置递增数
		{
			md->BitConfig.Value += md->BitConfig.IncreaseValue;
			UI::UIData::Number(md->BitConfig.VarId, md->BitConfig.Value);
		}
		if (md->BitConfig.Type == Project::VarType::SetRandVar)	//设置随机数
		{
			/*产生随机数*/
			srand((unsigned)time(0));
			int value = (rand() % (md->BitConfig.Max - md->BitConfig.Min + 1)) + md->BitConfig.Min;
			UI::UIData::Number(md->BitConfig.VarId, value);
		}
		if (md->BitConfig.Type == Project::VarType::SetFloat)	//设置随机数
		{
			/*产生随机数*/
			srand((unsigned)time(0));
			float value = (float)(rand() % (md->BitConfig.Max - md->BitConfig.Min + 1)) + md->BitConfig.Min;
			UI::UIData::Number(md->BitConfig.VarId, value);
		}
		if (md->BitConfig.Type == Project::VarType::SetString)	//设置字符串
		{
			srand((unsigned)time(0));
			int len = (rand() % (md->BitConfig.Max - md->BitConfig.Min + 1)) + md->BitConfig.Min;
			string data = randstr(len);
			UIDataService::Ins().SetString(md->BitConfig.VarId, data.c_str());
		}
		if (md->BitConfig.Type == Project::VarType::SetChina)	//设置字符串
		{
			srand((unsigned)time(0));
			int Num = (rand() % (md->BitConfig.Max - md->BitConfig.Min + 1)) + md->BitConfig.Min;
			if((size_t)Num < ctrl->ChineseString.size())
				UIDataService::Ins().SetString(md->BitConfig.VarId, ctrl->ChineseString[Num].c_str());
		}

	}

	void TestBitVarControl::CreateView()
	{
		//初始化VIEW
		std::shared_ptr <TestBitVarModel> mode = std::static_pointer_cast<TestBitVarModel>(pModel_);
		TestBitVarView* view = new TestBitVarView(mode->BitConfig.X, mode->BitConfig.Y,
			mode->BitConfig.Width, mode->BitConfig.Height, mode->BitConfig.Name.c_str());
		InitMVCView(view);
		vector<string>().swap(ChineseString);
		ChineseString.push_back("中文");
		ChineseString.push_back("输入测试");
		ChineseString.push_back("下位机");
		ChineseString.push_back("部件");
		ChineseString.push_back("赵钱孙李");

		view->callback(TestVarClickCB, Page());
	}
	
	void TestBitVarControl::HandleDataVar(Project::DataVarId &varId)
	{
		TestBitVarView *pView = static_cast<TestBitVarView *>(pView_);
		std::shared_ptr <TestBitVarModel> mode = std::static_pointer_cast<TestBitVarModel>(pModel_);

		if (mode->BitConfig.VarId.Cmp(varId))
		{
			if (mode->BitConfig.Type == Project::VarType::StateWordVar
				|| mode->BitConfig.Type == Project::VarType::SetRandVar)
			{
				int value = UI::UIData::Number<int>(mode->BitConfig.VarId);
				//pView->label(to_string(value).c_str());
				mode->BitConfig.Name = to_string(value);
				pView->redraw();
			}
			if (mode->BitConfig.Type == Project::VarType::SetFloat)
			{
				int value = (int)UI::UIData::Number<DOUBLE>(mode->BitConfig.VarId);
				//pView->label(to_string(value).c_str());
				mode->BitConfig.Name = to_string(value);
				pView->redraw();
			}
			if (mode->BitConfig.Type == Project::VarType::StateBitVar)
			{
				bool bit = UI::UIData::Bit(mode->BitConfig.VarId);
				if (bit)
					pView->color(FL_BLUE);
				else
					pView->color(FL_RED);
				pView->redraw();
			}
			if (mode->BitConfig.Type == Project::VarType::SetString)
			{
				mode->BitConfig.Name = UIDataService::Ins().GetString(mode->BitConfig.VarId);
			}
			if (mode->BitConfig.Type == Project::VarType::SetChina)
			{
				mode->BitConfig.Name = UIDataService::Ins().GetString(mode->BitConfig.VarId);
				UI::IResourceService::GB2312toUtf8(mode->BitConfig.Name);
			}
		}
	}
	int TestBitVarControl::PeekHMIMessage(Message::Msg* msg)
	{
		std::shared_ptr <TestBitVarModel> mode = std::static_pointer_cast<TestBitVarModel>(pModel_);

		Project::DataVarId varId;
		switch (msg->GetCode())
		{
		case WM_EVENT_DATEUPDATE:
			varId = Project::DataVarId(msg->LParam);
			HandleDataVar(varId);
			break;
		default:
			break;
		}
		return 1;//handled;
	}
}
