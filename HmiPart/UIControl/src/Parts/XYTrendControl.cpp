/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : XYTrendControl.cpp
 * Author   : qiaodan
 * Date     : 2021-03-12
 * Descript : ����XY����ͼ���ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "XYTrendControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "IDataService.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
namespace UI
{
	XYTrendControl::XYTrendControl(UI::HMIPage* w) : BaseControl(w)
	{
		//����TrendChartModel
		mode_ = std::shared_ptr<XYTrendModel>(new XYTrendModel());
		InitMVCModel(mode_);
	}

	void XYTrendControl::CreateView()
	{
		//��ʼ��VIEW
		XYTrendView *view = new XYTrendView(
			mode_->TrendXYConfig.X + mode_->TrendXYConfig.OffX,
			mode_->TrendXYConfig.Y + mode_->TrendXYConfig.OffY,
			mode_->TrendXYConfig.Width,
			mode_->TrendXYConfig.Height, nullptr);
		InitMVCView(view);
		view->InitDraw();
	}

	void XYTrendTimerCb(void *data)
	{
		XYTrendControl *ctrl = (XYTrendControl *)data;
		/*�ɼ�����*/
		ctrl->GetSampleData();
	}
	void XYTrendControl::OnReady()
	{
		XYTrendView* pView = static_cast<XYTrendView*>(pView_);
		if (mode_->TrendXYConfig.RefreshMode == 0)	//����ʽ����
		{
			if (HasTimer)
			{
				Page()->RemoveTimeout(XYTrendTimerCb, (void*)this);
				if (mode_->TrendXYConfig.SamplingCycle != 0)
					Page()->AddTimeout((size_t)mode_->TrendXYConfig.SamplingCycle, XYTrendTimerCb, (void *)this, true);
			}
			else
			{
				if (mode_->TrendXYConfig.SamplingCycle != 0)
					Page()->AddTimeout((size_t)mode_->TrendXYConfig.SamplingCycle, XYTrendTimerCb, (void *)this, true);
				HasTimer = true;
			}
			isSample_ = true;
		}
		else
		{
			if(mode_->TrendXYConfig.TriggerType == 0)
				preSampleStatus_ = UI::UIData::Bit(mode_->TrendXYConfig.TriggerTypeVar);
		}
		if (mode_->TrendXYConfig.SamplePointsVar != Project::DataVarId::NullId)
			mode_->TrendXYConfig.SamplePoints = UI::UIData::Number<int>(mode_->TrendXYConfig.SamplePointsVar);
		if (mode_->TrendXYConfig.TriggerDataVar != Project::DataVarId::NullId)
			mode_->TrendXYConfig.TriggerData = UI::UIData::Number<DOUBLE>(mode_->TrendXYConfig.TriggerDataVar);
		HandleSysChange(Permission);
	}
	void XYTrendControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->TrendXYConfig.VOffX != Project::DataVarId::NullId)
			mode_->TrendXYConfig.OffX = UI::UIData::Number<int>(mode_->TrendXYConfig.VOffX) + pageoffx;
		else
			mode_->TrendXYConfig.OffX = pageoffx;


		if (mode_->TrendXYConfig.VOffY != Project::DataVarId::NullId)
			mode_->TrendXYConfig.OffY = UI::UIData::Number<int>(mode_->TrendXYConfig.VOffY) + pageoffy;
		else
			mode_->TrendXYConfig.OffY = pageoffy;
	}

	void XYTrendControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*�ж��Ƿ���Ȩ��*/
			if (!UI::PermUtility::HasUserPerm(mode_->TrendXYConfig.Perm.RequireRegion))	//û��Ȩ�޲�������
			{
				if (mode_->TrendXYConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->TrendXYConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->TrendXYConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->TrendXYConfig.Perm, mode_->TrendXYConfig.X + mode_->TrendXYConfig.OffX, mode_->TrendXYConfig.Y + mode_->TrendXYConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}

	void XYTrendControl::GetSampleData(void)
	{
		XYTrendView* pView = static_cast<XYTrendView*>(pView_);
		if (pView->XData.size() != mode_->TrendXYConfig.DataChanel.size())
		{
			vector<vector<DDWORD>>().swap(pView->XData);
			for (unsigned i = 0; i < mode_->TrendXYConfig.DataChanel.size(); i++)
			{
				vector<DDWORD> tmpvector;
				pView->XData.push_back(tmpvector);
			}
		}
		if (pView->YData.size() != mode_->TrendXYConfig.DataChanel.size())
		{
			vector<vector<DDWORD>>().swap(pView->YData);
			for (unsigned i = 0; i < mode_->TrendXYConfig.DataChanel.size(); i++)
			{
				vector<DDWORD> tmpvector;
				pView->YData.push_back(tmpvector);
			}
		}
		for (unsigned i = 0; i < mode_->TrendXYConfig.DataChanel.size(); i++)
		{
			bool status = UI::UIData::Bit(mode_->TrendXYConfig.DataChanel[i].SampleControlVarId);
			//�������������������δ��������
			if (((status && mode_->TrendXYConfig.DataChanel[i].ControlCond == 0) ||
				(!status && mode_->TrendXYConfig.DataChanel[i].ControlCond == 1)))
				//&& pView->XData.size() < mode_->TrendXYConfig.SamplePoints)
			{
				if (pView->XData[i].size() >= (size_t)mode_->TrendXYConfig.SamplePoints)	//���ݲ���
				{
					if (mode_->TrendXYConfig.HandleSampleMode == Project::FullSampleAct::CleanSample)		//���
					{
						vector<DDWORD>().swap(pView->XData[i]);
						vector<DDWORD>().swap(pView->YData[i]);
					}
					if (mode_->TrendXYConfig.HandleSampleMode == Project::FullSampleAct::StopSample)		//ֹͣ
						return;			//���ɼ�����
					if (mode_->TrendXYConfig.HandleSampleMode == Project::FullSampleAct::CoverSample)		//����
					{
						if (!pView->XData[i].empty() && !pView->YData[i].empty())
						{
							pView->XData[i].erase(pView->XData[i].begin());
							pView->YData[i].erase(pView->YData[i].begin());
						}
					}
				}
				DDWORD xda = UIDataService::Ins().GetNumber(mode_->TrendXYConfig.DataChanel[i].StartXAddr);
				pView->XData[i].push_back(xda);
				DDWORD yda = UIDataService::Ins().GetNumber(mode_->TrendXYConfig.DataChanel[i].StartYAddr);
				pView->YData[i].push_back(yda);
				UI::ViewShowUtility::ShowView(pView, mode_->TrendXYConfig.Perm,
					mode_->TrendXYConfig.X + mode_->TrendXYConfig.OffX,
					mode_->TrendXYConfig.Y + mode_->TrendXYConfig.OffY);
			}
		}
	}

	void XYTrendControl::HandleDataVar(Project::DataVarId &varId)
	{
		XYTrendView* pView = static_cast<XYTrendView*>(pView_);
		if (mode_->TrendXYConfig.VOffX.Cmp(varId))
		{
			mode_->TrendXYConfig.OffX = UI::UIData::Number<int>(mode_->TrendXYConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->TrendXYConfig.Perm, mode_->TrendXYConfig.X + mode_->TrendXYConfig.OffX, mode_->TrendXYConfig.Y + mode_->TrendXYConfig.OffY);
		}
		if (mode_->TrendXYConfig.VOffY.Cmp(varId))
		{
			mode_->TrendXYConfig.OffY = UI::UIData::Number<int>(mode_->TrendXYConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->TrendXYConfig.Perm, mode_->TrendXYConfig.X + mode_->TrendXYConfig.OffX, mode_->TrendXYConfig.Y + mode_->TrendXYConfig.OffY);
		}
		if (mode_->TrendXYConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->TrendXYConfig.Perm, pView_);
		if (mode_->TrendXYConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->TrendXYConfig.Perm, pView_);
		bool needredraw = false;
		/*�Ա���ֵ*/
		if (mode_->TrendXYConfig.TriggerDataVar.Cmp(varId))
			mode_->TrendXYConfig.TriggerData = UI::UIData::Number<DOUBLE>(mode_->TrendXYConfig.TriggerDataVar);
		/*���������Ĵ���*/
		if (mode_->TrendXYConfig.TriggerTypeVar.Cmp(varId))
		{
			//����ʽ��������û����ͣ����
			if (mode_->TrendXYConfig.RefreshMode )
			{
				if (mode_->TrendXYConfig.TriggerType == 0)	//λ�Ĵ���
				{
					bool status = UIDataService::Ins().GetBit(mode_->TrendXYConfig.TriggerTypeVar);
					if (mode_->TrendXYConfig.ConditionBit == 1)	//�½��ش���
					{
						if (preSampleStatus_ == true && status == false)
							GetSampleData();		//��������
					}
					else				//�����ش���
					{
						if (preSampleStatus_ == false && status == true)
							GetSampleData();
					}
					preSampleStatus_ = status;
				}
				else	//�ּĴ���
				{
					DOUBLE data = UI::UIData::Number<DOUBLE>(mode_->TrendXYConfig.TriggerTypeVar);
					XJDataType tp = UIDataService::Ins().GetDataType(mode_->TrendXYConfig.TriggerTypeVar);
					if(JudgeCondition::MeetCondition(mode_->TrendXYConfig.ConditionWord,
						data, mode_->TrendXYConfig.TriggerData, tp))
						GetSampleData();
				}
			}
		}
		/*��������*/
		if (mode_->TrendXYConfig.SamplePointsVar.Cmp(varId))
		{
			mode_->TrendXYConfig.SamplePoints = UI::UIData::Number<int>(mode_->TrendXYConfig.SamplePointsVar);
			needredraw = true;
		}
		/*������*/
		if (mode_->TrendXYConfig.XScaleSet.UpperLimitVar.Cmp(varId))
		{
			needredraw = true;
			mode_->TrendXYConfig.XScaleSet.UpperLimit = UI::UIData::Number<DOUBLE>(mode_->TrendXYConfig.XScaleSet.UpperLimitVar);
		}
		if (mode_->TrendXYConfig.XScaleSet.LowerLimitVar.Cmp(varId))
		{
			needredraw = true;
			mode_->TrendXYConfig.XScaleSet.LowerLimit = UI::UIData::Number<DOUBLE>(mode_->TrendXYConfig.XScaleSet.LowerLimitVar);
		}
		if (mode_->TrendXYConfig.YScaleSet.UpperLimitVar.Cmp(varId))
		{
			needredraw = true;
			mode_->TrendXYConfig.YScaleSet.UpperLimit = UI::UIData::Number<DOUBLE>(mode_->TrendXYConfig.YScaleSet.UpperLimitVar);
		}
		if (mode_->TrendXYConfig.YScaleSet.LowerLimitVar.Cmp(varId))
		{
			needredraw = true;
			mode_->TrendXYConfig.YScaleSet.LowerLimit = UI::UIData::Number<DOUBLE>(mode_->TrendXYConfig.YScaleSet.LowerLimitVar);
		}
		if (needredraw)
			UI::ViewShowUtility::ShowView(pView, mode_->TrendXYConfig.Perm,
				mode_->TrendXYConfig.X + mode_->TrendXYConfig.OffX,
				mode_->TrendXYConfig.Y + mode_->TrendXYConfig.OffY);
	}

	int XYTrendControl::PeekHMIMessage(Message::Msg* msg)
	{
		XYTrendView* pView = static_cast<XYTrendView*>(pView_);
		Project::DataVarId varId;
		switch (msg->GetCode())
		{
		case WM_EVENT_DATEUPDATE:
			varId = Project::DataVarId(msg->LParam);
			HandleDataVar(varId);
			break;
		default:
			break;
		}
		return 1;//handled;
	}
}
