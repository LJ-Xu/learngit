/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : FluidUnitControl.h
 * Author   : qiaodan
 * Date     : 2020-12-29
 * Descript : 关于流体控件的控制的头文件，创建流体绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "FluidUnitView.h"
#include "FluidUnitModel.h"

namespace UI
{
	void FlowTimerCb(void *data);
	struct FluidData
	{
		FluidUnitView *View;
		FluidUnitModel *Model;
	};
	class FluidUnitControl : public BaseControl
	{
	public:
		FluidUnitControl() :FluidUnitControl(nullptr) {}
		FluidUnitControl(HMIPage*  );
		bool HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);

		void OnReady();		//开始
		FluidData FluidTimerData;
	protected:
		void CreateView()override;
		void HandleDataVar(Project::DataVarId &varId);
		int PeekHMIMessage(Message::Msg* msg) override;
	private:
		bool isflow_ = false;
		std::shared_ptr<FluidUnitModel> mode_;
	};
}

