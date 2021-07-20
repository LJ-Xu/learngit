/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GPolyLineControl.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于折线的控制的头文件，创建折线绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "GPolyLineView.h"
#include "BaseControl.h"
#include "GPolyLineModel.h"

namespace UI
{
	class GPolyLineControl :public BaseControl
	{
	public:
		GPolyLineControl() :GPolyLineControl(nullptr) {}
		GPolyLineControl(HMIPage*);
		void CreateView()override;
		bool HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);
		void OnReady();

	protected:
		void HandleDataVar(Project::DataVarId &varId);
		int PeekHMIMessage(Message::Msg* msg) override;
	private:
		std::shared_ptr <GPolyLineModel> mode_;

	};
}
