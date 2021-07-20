/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GCircleControl.h
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : 关于圆的控制的头文件，创建圆绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "GCircleView.h"
#include "BaseControl.h"
#include "GCircleModel.h"
namespace UI
{
	class GCircleControl : public BaseControl
	{
	public:
		GCircleControl() :GCircleControl(nullptr) {}
		GCircleControl(HMIPage*  );
		void CreateView()override;
		bool HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);
		void OnReady();
	protected:

		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<GCircleModel> mode_;
	};
}
