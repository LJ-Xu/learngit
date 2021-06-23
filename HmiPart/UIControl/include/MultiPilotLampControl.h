/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : MultiPilotLampControl.h
 * Author   : qiaodan
 * Date     : 2020-10-27
 * Descript : 关于多状态指示灯的控制的头文件，创建多状态指示灯绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "MultiPilotLampModel.h"
#include "MultiPilotLampView.h"

namespace UI
{
	//class MultiPilotLampView;
	struct MultiPilotLampData
	{
		MultiPilotLampView *View;
		MultiPilotLampModel *Model;
	};

	class MultiPilotLampControl : public BaseControl
	{
	public:
		MultiPilotLampControl() :MultiPilotLampControl(nullptr) {}
		MultiPilotLampControl(HMIPage*);
		void CreateView() override;
		void HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);

		void OnReady();		//开始
		MultiPilotLampData MultiLampTimerData;
		bool HasTimer = false;
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);

		/*处理数据寄存器变话*/
		void HandleRegChange();
		void HandleMeetCondition(int i);
		/*******************************************************************************
		 * Name     : MeetCondition
		 * Descript : 判断是否满足状态条件
		 * Input    : cond  - 当前状态条件
		 *			  src   - 对比值1
		 *			  dst   - 对比值1
		 * Output   : true  - 满足条件
		 *			  flase - 不满足条件
		 * Note	    :
		 *******************************************************************************/
		//bool MeetCondition(Project::Conditions cond, double src, double dst);
		bool MeetCondition(Project::StatusCondition cond, XJDataType* tp);

	private:
		std::shared_ptr<MultiPilotLampModel> mode_;
		double readvalue_;			//获取读取数据
		bool readbit_;
	};
}
