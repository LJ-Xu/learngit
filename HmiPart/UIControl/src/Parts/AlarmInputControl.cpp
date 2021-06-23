/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : AlarmInputControl.cpp
 * Author   : qiaodan
 * Date     : 2020-10-29
 * Descript : ��������¼����ƣ����ݴ�����Ϣ
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
		//����KeyBtnModel
		mode_ = std::shared_ptr<AlarmInputModel>(new AlarmInputModel());
		InitMVCModel(mode_);
	}

	void AlarmInputControl::CreateView()
	{
		//��ʼ��VIEW
 
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
		case Project::LT:			//С��
			if (src < dst)
				return true;
			else
				return false;
		case Project::GT:			//����
			if (src > dst)
				return true;
			else
				return false;
		case Project::LE:			//С�ڵ���
			if (src <= dst)
				return true;
			else
				return false;
		case Project::GE:			//���ڵ���
			if (src >= dst)
				return true;
			else
				return false;
		case Project::EQ:			//���
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
		if (info.Count != 0)		//��֮ǰ�ָ�����Ϣ��Ϊ����״̬
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
		//�������״̬
		Project::AlarmContent content;
		content.GroupName = info.GroupName;
		content.GroupNo = info.GroupNo;
		content.Category = info.Category;

		timeval tv;
		gettimeofday(&tv, NULL);
		content.TriggerTime.tv_sec = tv.tv_sec;
		content.TriggerTime.tv_usec = tv.tv_usec;		//��ȡ��������ʱ��
		content.Msg = StringUtility::GetDrawString(IResourceService::Ins(), info.Text, 0);//��ȡ������Ϣ
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
					(*alarmContent).RecoverTime.tv_usec = tv.tv_usec;		//��ȡ��������ʱ��
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
			//��ǰReadReg�Ĵ������ݷ����仯
			if ((*tmpInfo).ReadRegVar.Cmp(varId) || (*tmpInfo).ComPareVar.Cmp(varId))
			{
				if ((*tmpInfo).RegType)		//�ּĴ���
				{
					//��ȡ�ּĴ���ֵ
					byteValue = UIDataService::Ins().GetNumber<int>((*tmpInfo).ReadRegVar);
					//��ȡ�Ա���ֵ
					if ((*tmpInfo).ComPareVar ==  Project::DataVarId::NullId)
						compareValue = (*tmpInfo).ComPareValue;
					else
						compareValue = UIDataService::Ins().GetNumber<int>((*tmpInfo).ComPareVar);

					//�ж��Ƿ񱨾���������
					if (MeetByteCondition((*tmpInfo).BtyeCondition, byteValue, compareValue))
					{
						//�洢��Ϣ
						(*tmpInfo).Count++;
						StoreAlarmInfo((*tmpInfo));
						ret = 1;
					}
					else
					{
						//�����ָ�
						if ((*tmpInfo).Count != 0)
							if (RecoverAlarm((*tmpInfo)))
								ret = 2;
					}
				}
				else		//λ�Ĵ���
				{
					//��ȡλ�Ĵ���ֵ
					bitValue = UIDataService::Ins().GetBit((*tmpInfo).ReadRegVar);
					//�ж��Ƿ���������
					if (MeetBitCondition((*tmpInfo).BitCondition, prevbitValue_, bitValue))
					{
						(*tmpInfo).Count++;
						//�洢��Ϣ
						StoreAlarmInfo((*tmpInfo));
						ret = 1;
					}
					else
					{
						//�����ָ�
						if ((*tmpInfo).Count != 0)
							if(RecoverAlarm((*tmpInfo)))
								ret = 2;
					}
				}
			}
			else if((*tmpInfo).ComPareVar != Project::DataVarId::NullId && (*tmpInfo).ComPareVar.Cmp(varId))
			{
				//�ּĴ���
				byteValue = UIDataService::Ins().GetNumber<int>((*tmpInfo).ReadRegVar);
				compareValue = UIDataService::Ins().GetNumber<int>((*tmpInfo).ComPareVar);
				if (MeetByteCondition((*tmpInfo).BtyeCondition, byteValue, compareValue))
				{
					//�洢��Ϣ
					(*tmpInfo).Count++;
					StoreAlarmInfo((*tmpInfo));
					ret = 1;
				}
				else
				{
					//�����ָ�
					if ((*tmpInfo).Count != 0)
						if (RecoverAlarm((*tmpInfo)))
							ret = 2;
				}
			}
			//֪ͨ��Ҫ��ʾ�ı�����ʾ�ؼ�
			if (ret != 0)
			{
				std::vector<Project::AlarmDisPlayInfo>::iterator tmpDisplayInfo;
				for (tmpDisplayInfo = DisplayInfo.begin(); tmpDisplayInfo != DisplayInfo.end(); tmpDisplayInfo++)
				{
					if ((*tmpInfo).GroupName >= (*tmpDisplayInfo).GroupLowerLmt && (*tmpInfo).GroupName <= (*tmpDisplayInfo).GroupUpperLmt)
						(*tmpDisplayInfo).NeedNotify = true;
				}
			}
			//֪ͨ��Ҫ�����ı�������
			if (ret != 0)
			{
				std::vector<Project::AlarmWinInfo>::iterator tmpWinInfo;
				for (tmpWinInfo = WinInfo.begin(); tmpWinInfo != WinInfo.end(); tmpWinInfo++)
				{
					if ((*tmpInfo).GroupName == (*tmpWinInfo).GroupName && (*tmpInfo).GroupNo == (*tmpWinInfo).GroupNo)
					{
						if (ret == 1)
							(*tmpWinInfo).InfoMode = 1;			//����
						else if(ret == 2)
							(*tmpWinInfo).InfoMode = 0;			//�ָ�
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
			//֪ͨ������ʾ
			std::vector<Project::AlarmDisPlayInfo>::iterator tmpDisplayInfo;
			for (tmpDisplayInfo = DisplayInfo.begin(); tmpDisplayInfo != DisplayInfo.end(); tmpDisplayInfo++)
			{
				if ((*tmpDisplayInfo).NeedNotify)
				{
					(*tmpDisplayInfo).NeedNotify = false;
					//֪ͨ��Ӧ�ı�����ʾ���������仯
					Win()->SendUserMessage((void*)(*tmpDisplayInfo).CtrlId, AlARM_EVENT_CHANGE, 0);
				}
			}
			//֪ͨ������ʾ
			std::vector<Project::AlarmWinInfo>::iterator tmpWinInfo;
			for (tmpWinInfo = WinInfo.begin(); tmpWinInfo != WinInfo.end(); tmpWinInfo++)
			{
				if ((*tmpWinInfo).NeedNotify)
				{
					(*tmpWinInfo).NeedNotify = false;
					//֪ͨ��Ӧ�ı�����ʾ���������仯
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
