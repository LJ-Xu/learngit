#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : AlarmInputControl.h
 * Author   : qiaodan
 * Date     : 2020-11-3
 * Descript : 关于报警录入的控制的头文件，创建键盘按键绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "AlarmInputView.h"
#include "AlarmInputModel.h"
namespace UI
{
	class AlarmInputControl : public BaseControl
	{
	public:
		AlarmInputControl() :AlarmInputControl(nullptr) {}
		AlarmInputControl(HMIPage*);
		void CreateView() override;
		vector<Project::AlarmContent> Content;				//报警内容列表
		vector<Project::AlarmDisPlayInfo> DisplayInfo;		//报警显示控件列表
		vector<Project::AlarmWinInfo> WinInfo;		//报警窗口控件列表

		/*******************************************************************************
		 * Name     : MeetByteCondition
		 * Descript : 判断是否满足位数据报警触发条件
		 * Input    : cond  - 判断条件
		 *			  src   - 原数据
		 *			  dst   - 对比数据
		 * Output   : true  - 满足报警条件
		 *			  false - 不满足报警条件
		 * Note	    :
		 *******************************************************************************/
		bool MeetByteCondition(Project::Conditions cond, int src, int dst);
		bool MeetBitCondition(Project::BitConditions cond, int src, int dst);


		/*******************************************************************************
		 * Name     : MeetAlarmCondtion
		 * Descript : 测试报警信息是否需要更新
		 * Input    : varId - 发生变化的寄存器地址
		 * Output   : 0 - 不需要更新 
		 *			  1 - 需要更新
		 * Note	    :
		 *******************************************************************************/
		int MeetAlarmCondtion(Project::DataVarId &varId);


		/*******************************************************************************
		 * Name     : RecoverAlarm
		 * Descript : 判断并更新报警信息是否恢复
		 * Input    : info - 当前报警信息条件
		 * Output   : 0 - 报警信息未更新
		 *			  1 - 报警信息更新
		 * Note	    :
		 *******************************************************************************/
		int RecoverAlarm(Project::AlarmInfoRes &info);

		/*******************************************************************************
		 * Name     : StoreAlarmInfo
		 * Descript : 存储报警信息
		 * Input    : info - 触发报警的信息
		 * Output   :
		 * Note	    :
		 *******************************************************************************/
		void StoreAlarmInfo(Project::AlarmInfoRes &info);
	protected:
		enum DataVarType { ALARM, RECOVER, NAME, NO };

		int PeekHMIMessage(Message::Msg* msg) override;
		DataVarType GetDataVarType(Project::DataVarId &varId);
		void HandleDataVar(DataVarType, Project::DataVarId &varId);
	private:
		std::shared_ptr<AlarmInputModel> mode_;
		int prevbitValue_;		//记录上一个位寄存器值
	};
}