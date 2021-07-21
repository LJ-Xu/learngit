/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ChineseDisplayControl.h
 * Author   : qiaodan
 * Date     : 2020-10-15
 * Descript : 关于中文显示控制的头文件，创建字符显示框，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "ChineseDisplayModel.h"
#include "ChineseDisplayView.h"
namespace UI
{
	class ChineseDisplayControl : public BaseControl
	{
	public:
		ChineseDisplayControl() :ChineseDisplayControl(nullptr) {}
		ChineseDisplayControl(HMIPage*);
		void CreateView()override;
		bool HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);
		void OnReady();

	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<ChineseDisplayModel> mode_;
	};
}
