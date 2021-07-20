/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GLineControl.h
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : 关于直线的控制的头文件，创建直线绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "GLineView.h"
#include "BaseControl.h"
#include "GLineModel.h"

namespace UI
{
	class GLineControl :public BaseControl
	{
	public:
		GLineControl() :GLineControl(nullptr) {}
		GLineControl(HMIPage*  );
		void CreateView()override;
		void SetOffSetPos(int offx, int offy);
		bool HandleSysChange(SysChangeEM catogray);
		void OnReady();

	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<GLineModel> mode_;
	};

}