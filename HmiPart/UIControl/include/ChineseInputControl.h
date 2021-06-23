/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : CharacterInputControl.h
 * Author   : qiaodan
 * Date     : 2020-10-19
 * Descript : 关于中文输入框的控制的头文件，创建字符输入框，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "ChineseInputModel.h"
namespace UI
{
	class ChineseInputControl : public BaseControl
	{
	public:
		ChineseInputControl() :ChineseInputControl(nullptr) {}
		ChineseInputControl(HMIPage*);
		void CreateView()override;
		void HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);
		void OnReady();

	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);

	private:
		std::shared_ptr<ChineseInputModel> mode_;
	};
}


