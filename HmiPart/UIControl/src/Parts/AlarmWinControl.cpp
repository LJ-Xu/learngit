/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : AlarmDisplayControl.cpp
 * Author   : qiaodan
 * Date     : 2020-11-05
 * Descript : ����������ʾ���ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "AlarmWinControl.h"
#include "AlarmWinView.h"
#include "AlarmWinModel.h"
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include "HMIWindow.h"
#include "Message.h"
#include "AlarmInfoRes.h"
#include "UIComm.h"
#include "AlarmInputControl.h"
namespace UI
{
	AlarmWinControl::AlarmWinControl(HMIPage* w) :BaseControl(w)
	{
		//����KeyBtnModel
		mode_ = std::shared_ptr<AlarmWinModel>(new AlarmWinModel());
		InitMVCModel(mode_);
	}

	void AlarmWinControl::CreateView()
	{
		//��ʼ��VIEW
		AlarmWinView* view = new AlarmWinView(
				mode_->WinAlarmConfig.X + mode_->WinAlarmConfig.OffX,
				mode_->WinAlarmConfig.Y + mode_->WinAlarmConfig.OffY,
				mode_->WinAlarmConfig.Width,
				mode_->WinAlarmConfig.Height,nullptr);
		InitMVCView(view);
		//������Ϣ������¼����еǼ�ע��
		Project::AlarmWinInfo *info = new Project::AlarmWinInfo();
		info->CtrlId = CtrlId();
		info->GroupName = mode_->WinAlarmConfig.GroupName;
		info->GroupNo = mode_->WinAlarmConfig.GroupNo;
		//Win()->SendAutoFreeMessage<Project::AlarmWinInfo>((void*)Win()->AlarmEventId, AlARM_EVENT_WIN,info);
		delete info;
		//Win()->SendUserMessage((void*)Win()->AlarmEventId, AlARM_EVENT_MSG,(int)info);
 
	}

	void AlarmWinControl::AddVarData(vector<Project::DataVarId*> vars)
	{
	}
	AlarmWinControl::DataVarType AlarmWinControl::GetDataVarType(Project::DataVarId &varId)
	{
		if (mode_->WinAlarmConfig.VOffX.Cmp(varId))
			return (DataVarType)OFFX;
		else if (mode_->WinAlarmConfig.VOffY.Cmp(varId))
			return (DataVarType)OFFY;
		else if (mode_->WinAlarmConfig.Perm.Permission.PermData.Cmp(varId))
			return (DataVarType)PERM;
		else
			return (DataVarType)NO;
	}
	void AlarmWinControl::HandleDataVar(DataVarType type, Project::DataVarId &varId)
	{
		AlarmWinView* pView = static_cast<AlarmWinView*>(pView_);
		switch (type)
		{
		case UI::AlarmWinControl::OFFX:
			mode_->WinAlarmConfig.OffX = UIDataService::Ins().GetNumber<int>(mode_->WinAlarmConfig.VOffX);
			RedrawMove(mode_->WinAlarmConfig.X + mode_->WinAlarmConfig.OffX, mode_->WinAlarmConfig.Y + mode_->WinAlarmConfig.OffY);
			break;
		case UI::AlarmWinControl::OFFY:
			mode_->WinAlarmConfig.OffY = UIDataService::Ins().GetNumber<int>(mode_->WinAlarmConfig.VOffY);
			RedrawMove(mode_->WinAlarmConfig.X + mode_->WinAlarmConfig.OffX, mode_->WinAlarmConfig.Y + mode_->WinAlarmConfig.OffY);
			break;
		case UI::AlarmWinControl::PERM:
			if (mode_->WinAlarmConfig.Perm.Permission.GetDisplayPms())	//������ʾ
			{
				pView_->show();
				pView_->redraw();
			}
			else {
				pView_->hide();
				pView_->redraw();
			}
			if (mode_->WinAlarmConfig.Perm.Permission.GetUsePms())		//����ʹ��
			{
				pView_->activate();
				pView_->redraw();
			}
			else
			{
				pView_->deactivate();
				pView_->redraw();
			}
			break;
		case UI::AlarmWinControl::NO:
			break;
		default:
			break;
		}
	}

	int AlarmWinControl::PeekHMIMessage(Message::Msg* msg)
	{
		AlarmWinView* pView = static_cast<AlarmWinView*>(pView_);
		Project::DataVarId varId;
		DataVarType type;
		bool permission;
		switch (msg->GetCode())
		{
		case WM_EVENT_DATEUPDATE:
			varId = *(Project::DataVarId *)msg->LParam;
			type = GetDataVarType(varId);
			HandleDataVar(type, varId);
			break;
		case WM_CHANGE_PERM:
			permission = IPermission::Ins()->HasPerm(mode_->WinAlarmConfig.Perm.RequireRegion);
			HandlePermChange(mode_->WinAlarmConfig.Perm.ActionMode, permission);
			break;
		case WM_EVENT_TIMEUP:
			Win()->SendUserMessage((void*)this->CtrlId(), WM_EVENT_TIMETICK, 1000);//1s
			break;
		case WM_EVENT_READY:
			Win()->SendUserMessage((void*)this->CtrlId(), WM_EVENT_TIMETICK, 1000);//1s
			break;
		case AlARM_EVENT_CHANGE:
		{

			//��ȡ����
			//std::shared_ptr<BaseControl> ctrl = Win()->GetCtrlById(/*Win()->AlarmEventId*/0);
			//std::shared_ptr <UI::AlarmInputControl> pctrl = std::static_pointer_cast<UI::AlarmInputControl>(ctrl);
			//unsigned i;
			//for (i = 0; i < pctrl->Content.size(); i++)
			//{
			//	if (pctrl->Content[i].GroupName == mode_->WinAlarmConfig.GroupName
			//		&& pctrl->Content[i].GroupNo == mode_->WinAlarmConfig.GroupNo)
			//		AlarmInfo = pctrl->Content[i];
			//}
			////�ж������ݻָ��������ݱ���
			//if (msg->LParam == 0 && mode_->WinAlarmConfig.AutoClose)	//�ָ�
			//	pView_->hide();		//�رմ���
			//else 					//����	
			//	pView_->show();
			break;
		}
		default:
			break;
		}
		return 1;//handled;
	}
}