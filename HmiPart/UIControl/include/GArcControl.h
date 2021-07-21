/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GArcControl.h
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : 关于圆弧的控制的头文件，创建圆弧绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "GArcView.h"
#include "BaseControl.h"
#include "GArcModel.h"
#include "DataVar.h"

namespace UI
{
	class GArcControl : public BaseControl
	{
	public:
		GArcControl() :GArcControl(nullptr) {}
		GArcControl(HMIPage*);
		void CreateView() override;
		bool HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);
		void OnReady();

	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<GArcModel> mode_;
	};
}