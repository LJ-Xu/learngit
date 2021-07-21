/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : CharacterDisplayControl.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于字符显示框的控制的头文件，创建字符显示框，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "CharacterDisplayModel.h"
#include "CharacterDisplayView.h"
namespace UI
{
	class CharacterDisplayControl : public BaseControl
	{
	public:
		CharacterDisplayControl() :CharacterDisplayControl(nullptr) {}
		void SetOffSetPos(int offx, int offy);
		CharacterDisplayControl(HMIPage*);
		void CreateView()override;
		bool HandleSysChange(SysChangeEM catogray);
		void OnReady();
	protected:

		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<CharacterDisplayModel> mode_;
	};
}