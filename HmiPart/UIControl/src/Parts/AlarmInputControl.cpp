/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : AlarmInputControl.cpp
 * Author   : qiaodan
 * Date     : 2020-10-29
 * Descript : 创建报警录入绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "AlarmInputControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "StringUtility.h"
#include <time.h>
namespace UI
{
	AlarmInputControl::AlarmInputControl(HMIPage* w) :BaseControl(w)
	{
		//创建KeyBtnModel
		mode_ = std::shared_ptr<AlarmInputModel>(new AlarmInputModel());
		InitMVCModel(mode_);
	}

	void AlarmInputControl::CreateView()
	{
		//初始化VIEW
 
	}

	bool AlarmInputControl::MeetBitCondition(Project::BitConditions cond, int src, int dst)
	{
		switch (cond)	
		{
		case Project::ON:
			if (src == 1 && dst == 1)
				return true;
			else
				return false;
			break;
		case Project::OFF:
			if (src == 0 && dst == 0)
				return true;
			else
				return false;
			break;
		case Project::FALL:
			if (src == 1 && dst == 0)
				return true;
			else
				return false;
			break;
		case Project::RISE:
			if (src == 0 && dst == 1)
				return true;
			else
				return false;
			break;
		default:
			break;
		}
		return false;
	}

	bool AlarmInputControl::MeetByteCondition(Project::Conditions cond, int src, int dst)
	{
		switch (cond)
		{
		case Project::LT:			//小于
			if (src < dst)
				return true;
			else
				return false;
		case Project::GT:			//大于
			if (src > dst)
				return true;
			else
				return false;
		case Project::LE:			//小于等于
			if (src <= dst)
				return true;
			else
				return false;
		case Project::GE:			//大于等于
			if (src >= dst)
				return true;
			else
				return false;
		case Project::EQ:			//相等
			if (src == dst)
				return true;
			else
				return false;
		case Project::NE:
			if (src != dst)
				return true;
			else
				return false;
		default:
			break;
		}
		return false;
	}
	void AlarmInputControl::StoreAlarmInfo(Project::AlarmInfoRes &info)
	{
		if (info.Count != 0)		//将之前恢复的信息置为报警状态
		{
			vector<Project::AlarmContent>::iterator alarmContent;
			for (alarmContent = Content.begin();alarmContent != Content.end();alarmContent++)
			{
				if ((*alarmContent).GroupName == info.GroupName && (*alarmContent).GroupNo == info.GroupNo)
				{
					if ((*alarmContent).Recovered)
					{
						(*alarmContent).Recovered = false;
					}
				}
			}
		}
		//添加现在状态
		Project::AlarmContent content;
		content.GroupName = info.GroupName;
		content.GroupNo = info.GroupNo;
		content.Category = info.Category;

		timeval tv;
		gettimeofday(&tv, NULL);
		content.TriggerTime.tv_sec = tv.tv_sec;
		content.TriggerTime.tv_usec = tv.tv_usec;		//获取报警触发时间
		content.Msg = StringUtility::GetDrawString(IResourceService::Ins(), info.Text, 0);//获取报警信息
		content.Count = info.Count;
		content.Recovered = false;
		content.Confirmed = false;
		Content.push_back(content);
	}

	int AlarmInputControl::RecoverAlarm(Project::AlarmInfoRes &info)
	{
		int ret = 0;
		vector<Project::AlarmContent>::iterator alarmContent;
		for (alarmContent = Content.begin();alarmContent != Content.end();alarmContent++)
		{
			if ((*alarmContent).GroupName == info.GroupName && (*alarmContent).GroupNo == info.GroupNo)
			{
				if (!(*alarmContent).Recovered)
				{
					(*alarmContent).Recovered = true;
					timeval tv;
					gettimeofday(&tv, NULL);
					(*alarmContent).RecoverTime.tv_sec = tv.tv_sec;
					(*alarmContent).RecoverTime.tv_usec = tv.tv_usec;		//获取报警触发时间
					ret = 1;
				}
			}
		}
		return ret;
	}
	int AlarmInputControl::MeetAlarmCondtion(Project::DataVarId &varId)
	{
		std::vector<Project::AlarmInfoRes>::iterator tmpInfo;
		unsigned i, ret = 0;
		int byteValue, compareValue;
		bool bitValue;
		for (i = 0, tmpInfo = mode_->InputAlarmConfig.InfoLst.begin(); tmpInfo != mode_->InputAlarmConfig.InfoLst.end(); i++, tmpInfo++)
		{
			//当前ReadReg寄存器数据发生变化
			if ((*tmpInfo).ReadRegVar.Cmp(varId) || (*tmpInfo).ComPareVar.Cmp(varId))
			{
				if ((*tmpInfo).RegType)		//字寄存器
				{
					//获取字寄存器值
					byteValue = UIDataService::Ins().GetNumber<int>((*tmpInfo).ReadRegVar);
					//获取对比数值
					if ((*tmpInfo).ComPareVar ==  Project::DataVarId::NullId)
						compareValue = (*tmpInfo).ComPareValue;
					else
						compareValue = UIDataService::Ins().GetNumber<int>((*tmpInfo).ComPareVar);

					//判断是否报警满足条件
					if (MeetByteCondition((*tmpInfo).BtyeCondition, byteValue, compareValue))
					{
						//存储信息
						(*tmpInfo).Count++;
						StoreAlarmInfo((*tmpInfo));
						ret = 1;
					}
					else
					{
						//报警恢复
						if ((*tmpInfo).Count != 0)
							if (RecoverAlarm((*tmpInfo)))
								ret = 2;
					}
				}
				else		//位寄存器
				{
					//获取位寄存器值
					bitValue = UIDataService::Ins().GetBit((*tmpInfo).ReadRegVar);
					//判断是否满足条件
					if (MeetBitCondition((*tmpInfo).BitCondition, prevbitValue_, bitValue))
					{
						(*tmpInfo).Count++;
						//存储信息
						StoreAlarmInfo((*tmpInfo));
						ret = 1;
					}
					else
					{
						//报警恢复
						if ((*tmpInfo).Count != 0)
							if(RecoverAlarm((*tmpInfo)))
								ret = 2;
					}
				}
			}
			else if((*tmpInfo).ComPareVar != Project::DataVarId::NullId && (*tmpInfo).ComPareVar.Cmp(varId))
			{
				//字寄存器
				byteValue = UIDataService::Ins().GetNumber<int>((*tmpInfo).ReadRegVar);
				compareValue = UIDataService::Ins().GetNumber<int>((*tmpInfo).ComPareVar);
				if (MeetByteCondition((*tmpInfo).BtyeCondition, byteValue, compareValue))
				{
					//存储信息
					(*tmpInfo).Count++;
					StoreAlarmInfo((*tmpInfo));
					ret = 1;
				}
				else
				{
					//报警恢复
					if ((*tmpInfo).Count != 0)
						if (RecoverAlarm((*tmpInfo)))
							ret = 2;
				}
			}
			//通知需要显示的报警显示控件
			if (ret != 0)
			{
				std::vector<Project::AlarmDisPlayInfo>::iterator tmpDisplayInfo;
				for (tmpDisplayInfo = DisplayInfo.begin(); tmpDisplayInfo != DisplayInfo.end(); tmpDisplayInfo++)
				{
					if ((*tmpInfo).GroupName >= (*tmpDisplayInfo).GroupLowerLmt && (*tmpInfo).GroupName <= (*tmpDisplayInfo).GroupUpperLmt)
						(*tmpDisplayInfo).NeedNotify = true;
				}
			}
			//通知需要弹出的报警窗口
			if (ret != 0)
			{
				std::vector<Project::AlarmWinInfo>::iterator tmpWinInfo;
				for (tmpWinInfo = WinInfo.begin(); tmpWinInfo != WinInfo.end(); tmpWinInfo++)
				{
					if ((*tmpInfo).GroupName == (*tmpWinInfo).GroupName && (*tmpInfo).GroupNo == (*tmpWinInfo).GroupNo)
					{
						if (ret == 1)
							(*tmpWinInfo).InfoMode = 1;			//报警
						else if(ret == 2)
							(*tmpWinInfo).InfoMode = 0;			//恢复
						(*tmpWinInfo).NeedNotify = true;
					}

				}
			}
		}
		return ret;
	}

	AlarmInputControl::DataVarType AlarmInputControl::GetDataVarType(Project::DataVarId &varId)
	{
		if (MeetAlarmCondtion(varId))
			return (DataVarType)ALARM;
		else if (mode_->InputAlarmConfig.SaveLst.FileNameDataVar.Cmp(varId))
			return (DataVarType)NAME;
		else
			return (DataVarType)NO;
	}
	void AlarmInputControl::HandleDataVar(DataVarType type, Project::DataVarId &varId)
	{

		switch (type)
		{
		case UI::AlarmInputControl::ALARM:
		{
			//通知报警显示
			std::vector<Project::AlarmDisPlayInfo>::iterator tmpDisplayInfo;
			for (tmpDisplayInfo = DisplayInfo.begin(); tmpDisplayInfo != DisplayInfo.end(); tmpDisplayInfo++)
			{
				if ((*tmpDisplayInfo).NeedNotify)
				{
					(*tmpDisplayInfo).NeedNotify = false;
					//通知对应的报警显示报警发生变化
					Win()->SendUserMessage((void*)(*tmpDisplayInfo).CtrlId, AlARM_EVENT_CHANGE, 0);
				}
			}
			//通知报警显示
			std::vector<Project::AlarmWinInfo>::iterator tmpWinInfo;
			for (tmpWinInfo = WinInfo.begin(); tmpWinInfo != WinInfo.end(); tmpWinInfo++)
			{
				if ((*tmpWinInfo).NeedNotify)
				{
					(*tmpWinInfo).NeedNotify = false;
					//通知对应的报警显示报警发生变化
					Win()->SendUserMessage((void*)(*tmpWinInfo).CtrlId, AlARM_EVENT_CHANGE, (*tmpWinInfo).InfoMode);
				}
			}
			break;
		}
		case UI::AlarmInputControl::NAME:
			break;
		case UI::AlarmInputControl::NO:
			break;
		default:
			break;
		}
	}

	int AlarmInputControl::PeekHMIMessage(Message::Msg* msg)
	{
		Project::DataVarId varId;
		DataVarType type;
		switch (msg->GetCode())
		{
		case WM_EVENT_DATEUPDATE:
			varId = *(Project::DataVarId *)msg->LParam;
			type = GetDataVarType(varId);
			HandleDataVar(type, varId);
			break;
		case WM_EVENT_TIMEUP:
			Win()->SendUserMessage((void*)this->CtrlId(), WM_EVENT_TIMETICK, 1000);//1s
			break;
		case WM_EVENT_READY:
			Win()->SendUserMessage((void*)this->CtrlId(), WM_EVENT_TIMETICK, 1000);//1s
			break;
		case AlARM_EVENT_DISPLAY:
		{
			Project::AlarmDisPlayInfo *info = (Project::AlarmDisPlayInfo *)msg->LParam;
			Project::AlarmDisPlayInfo displayInfo = *info;
			DisplayInfo.push_back(displayInfo);
			break;
		}
		default:
			break;
		}
		return 1;//handled;
	}
}
