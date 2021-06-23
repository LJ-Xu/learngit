/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ChineseBtnControl.h
 * Author   : qiaodan
 * Date     : 2020-10-15
 * Descript : 关于中文按键控制的头文件，创建字符显示框，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "ChineseBtnModel.h"
#include "ChineseBtnView.h"
#include "BaseControl.h"
namespace UI
{
	class GButtonView;
	class ChineseBtnControl : public BaseControl
	{
	public:
		ChineseBtnControl() :ChineseBtnControl(nullptr) {}
		ChineseBtnControl(HMIPage*);
		void CreateView()override;
		void SendMsg(int message, int lparam);
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		enum DataVarType { OFFX, OFFY, PERM, NO };
		DataVarType GetDataVarType(Project::DataVarId &varId);
		void HandleDataVar(DataVarType, Project::DataVarId &varId);
	private:
		shared_ptr < ChineseBtnModel> mode_;
	};
}
