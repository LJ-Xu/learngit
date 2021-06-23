/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeyBtnControl.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于键盘按键的控制的头文件，创建键盘按键绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "KeyBtnView.h"
#include "KeyBtnModel.h"
namespace UI
{
	class KeyBtnControl : public BaseControl
	{
	public:
		KeyBtnControl() :KeyBtnControl(nullptr) {}
		KeyBtnControl(HMIPage*  );
		void CreateView()override;
		void HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);
		void OnReady();
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<KeyBtnModel> mode_;
	};
}


