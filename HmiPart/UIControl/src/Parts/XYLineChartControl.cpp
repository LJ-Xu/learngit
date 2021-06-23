/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : XYLineChartControl.cpp
 * Author   : qiaodan
 * Date     : 2020-12-09
 * Descript : 创建XY折线图绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "XYLineChartControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "IDataService.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
namespace UI
{
	XYLineChartControl::XYLineChartControl(UI::HMIPage* w) : BaseControl(w)
	{
		//创建TrendChartModel
		mode_ = std::shared_ptr<XYLineChartModel>(new XYLineChartModel());
		InitMVCModel(mode_);
	}

	void XYLineChartControl::CreateView()
	{
		//初始化VIEW
		XYLineChartView *view = new XYLineChartView(
			mode_->ChartXYLineConfig.X + mode_->ChartXYLineConfig.OffX,
			mode_->ChartXYLineConfig.Y + mode_->ChartXYLineConfig.OffY,
			mode_->ChartXYLineConfig.Width,
			mode_->ChartXYLineConfig.Height, nullptr);
		InitMVCView(view);
		view->InitDraw();
		//OnReady();
	}

	void XYChartTimerCb(void *data)
	{
		XYLineChartControl *ctrl = (XYLineChartControl *)data;
		/*采集数据*/
		ctrl->GetSampleData();
	}
	void XYLineChartControl::OnReady()
	{
		XYLineChartView* pView = static_cast<XYLineChartView*>(pView_);
		TimerData.Model = mode_.get();
		TimerData.View = pView;
		if (mode_->ChartXYLineConfig.RefreshMode == 0)	//周期式采样
		{
			if (HasTimer)
			{
				Page()->RemoveTimeout(XYChartTimerCb, (void*)this);
				if(mode_->ChartXYLineConfig.SamplingCycle != 0)
					Page()->AddTimeout((size_t)mode_->ChartXYLineConfig.SamplingCycle, XYChartTimerCb, (void *)this, true);
			}
			else
			{
				if (mode_->ChartXYLineConfig.SamplingCycle != 0)
					Page()->AddTimeout((size_t)mode_->ChartXYLineConfig.SamplingCycle, XYChartTimerCb, (void *)this, true);
				HasTimer = true;
			}
			isSample_ = true;
		}
		else
			preSampleStatus_ = UI::UIData::Bit(mode_->ChartXYLineConfig.TriggerTypeVar);
		preClearStatus_= UI::UIData::Bit(mode_->ChartXYLineConfig.ClearControlVar);
		HandleSysChange(Permission);
	}
	void XYLineChartControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->ChartXYLineConfig.VOffX != Project::DataVarId::NullId)
			mode_->ChartXYLineConfig.OffX = UI::UIData::Number<int>(mode_->ChartXYLineConfig.VOffX) + pageoffx;
		else
			mode_->ChartXYLineConfig.OffX = pageoffx;


		if (mode_->ChartXYLineConfig.VOffY != Project::DataVarId::NullId)
			mode_->ChartXYLineConfig.OffY = UI::UIData::Number<int>(mode_->ChartXYLineConfig.VOffY) + pageoffy;
		else
			mode_->ChartXYLineConfig.OffY = pageoffy;
	}
	void XYLineChartControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->ChartXYLineConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->ChartXYLineConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->ChartXYLineConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->ChartXYLineConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->ChartXYLineConfig.Perm, mode_->ChartXYLineConfig.X + mode_->ChartXYLineConfig.OffX, mode_->ChartXYLineConfig.Y + mode_->ChartXYLineConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}
	/*获取采样数据*/
	void XYLineChartControl::GetSampleData()
	{
		XYLineChartView* pView = static_cast<XYLineChartView*>(pView_);
		vector<vector<DDWORD>>().swap(pView->XData);
		vector<vector<DDWORD>>().swap(pView->YData);
		for (unsigned i = 0; i < mode_->ChartXYLineConfig.DataChanel.size(); i++)
		{
			if (mode_->ChartXYLineConfig.DataChanel[i].IsXYDataFromSameSource)	//XY相同数据区
			{
				if (mode_->ChartXYLineConfig.DataChanel[i].StartXAddr != Project::DataVarId::NullId)
				{
					XJDataType tp = UIDataService::Ins().GetDataType(mode_->ChartXYLineConfig.DataChanel[i].StartXAddr);
					int num = Utility::DataFormate::GetByteNumByInfo(tp);
					DataApi::AppReqVarDatas(mode_->ChartXYLineConfig.DataChanel[i].StartXAddr, pView->SampleNum * 2 * num);
					//UIDataService::Ins().GetVarDatas(mode_->ChartXYLineConfig.DataChanel[i].StartXAddr, pView->SampleNum * 2 * num);
				}
			}
			else
			{
				if (mode_->ChartXYLineConfig.DataChanel[i].StartXAddr != Project::DataVarId::NullId)
				{
					XJDataType tpx = UIDataService::Ins().GetDataType(mode_->ChartXYLineConfig.DataChanel[i].StartXAddr);
					int numx = Utility::DataFormate::GetByteNumByInfo(tpx);
					DataApi::AppReqVarDatas(mode_->ChartXYLineConfig.DataChanel[i].StartXAddr, pView->SampleNum * numx);
					//UIDataService::Ins().GetVarDatas(mode_->ChartXYLineConfig.DataChanel[i].StartXAddr, pView->SampleNum * numx);
				}
				if (mode_->ChartXYLineConfig.DataChanel[i].StartYAddr != Project::DataVarId::NullId)
				{
					XJDataType tpy = UIDataService::Ins().GetDataType(mode_->ChartXYLineConfig.DataChanel[i].StartYAddr);
					int numy = Utility::DataFormate::GetByteNumByInfo(tpy);
					DataApi::AppReqVarDatas(mode_->ChartXYLineConfig.DataChanel[i].StartYAddr, pView->SampleNum * numy);
					//UIDataService::Ins().GetVarDatas(mode_->ChartXYLineConfig.DataChanel[i].StartYAddr, pView->SampleNum * numy);
				}
			}
		}
	}
	void XYLineChartControl::HandleSampleData(BYTE* str, int len, int channelno,bool Isxdata)
	{
		XYLineChartView* pView = static_cast<XYLineChartView*>(pView_);
		if (!str)
			return;
		if (mode_->ChartXYLineConfig.DataChanel[channelno].IsXYDataFromSameSource)
		{
			XJDataType tp;
			if (mode_->ChartXYLineConfig.DataChanel[channelno].StartXAddr != Project::DataVarId::NullId)
				tp = UIDataService::Ins().GetDataType(mode_->ChartXYLineConfig.DataChanel[channelno].StartXAddr);
			vector<DDWORD> xdatatmp;
			vector<DDWORD> ydatatmp;
			for (size_t i = 0; i < (size_t)len; i ++)
			{
				DDWORD da = Utility::DataFormate::BytesParse(i,str,tp);
				DOUBLE a = Utility::DataFormate::GetNumber<DOUBLE>(da, tp);
				if(Isxdata)
					xdatatmp.push_back(da);
				else
					ydatatmp.push_back(da);
				Isxdata = !Isxdata;
			}
			pView->XData.push_back(xdatatmp);
			pView->YData.push_back(ydatatmp);
		}
		else
		{
			XJDataType tpx;
			if (mode_->ChartXYLineConfig.DataChanel[channelno].StartXAddr != Project::DataVarId::NullId)
				tpx = UIDataService::Ins().GetDataType(mode_->ChartXYLineConfig.DataChanel[channelno].StartXAddr);
			XJDataType tpy;
			if (mode_->ChartXYLineConfig.DataChanel[channelno].StartYAddr != Project::DataVarId::NullId)
				tpy = UIDataService::Ins().GetDataType(mode_->ChartXYLineConfig.DataChanel[channelno].StartYAddr);

			vector<DDWORD> datatmp;

			for (size_t i = 0; i < (size_t)len; i ++)
			{
				if (Isxdata)
				{
					DDWORD da = Utility::DataFormate::BytesParse(i, str, tpx);
					datatmp.push_back(da);
				}
				else
				{
					DDWORD da = Utility::DataFormate::BytesParse(i, str, tpy);
					datatmp.push_back(da);
				}
			}
			if (Isxdata)
				pView->XData.push_back(datatmp);
			else
				pView->YData.push_back(datatmp);
		}
	}

	void XYLineChartControl::HandleDataVar(Project::DataVarId &varId)
	{
		XYLineChartView* pView = static_cast<XYLineChartView*>(pView_);
		if (mode_->ChartXYLineConfig.VOffX.Cmp(varId))
		{
			mode_->ChartXYLineConfig.OffX = UI::UIData::Number<int>(mode_->ChartXYLineConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->ChartXYLineConfig.Perm, mode_->ChartXYLineConfig.X + mode_->ChartXYLineConfig.OffX, mode_->ChartXYLineConfig.Y + mode_->ChartXYLineConfig.OffY);
		}
		if (mode_->ChartXYLineConfig.VOffY.Cmp(varId))
		{
			mode_->ChartXYLineConfig.OffY = UI::UIData::Number<int>(mode_->ChartXYLineConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->ChartXYLineConfig.Perm, mode_->ChartXYLineConfig.X + mode_->ChartXYLineConfig.OffX, mode_->ChartXYLineConfig.Y + mode_->ChartXYLineConfig.OffY);
		}
		if (mode_->ChartXYLineConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->ChartXYLineConfig.Perm, pView_);
		if (mode_->ChartXYLineConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->ChartXYLineConfig.Perm, pView_);
		/*触发采样寄存器*/
		if (mode_->ChartXYLineConfig.TriggerTypeVar.Cmp(varId))
		{
			//触发式采样并且没有暂停采样
			if (mode_->ChartXYLineConfig.RefreshMode && isStop_ == false)		
			{
				bool status = UIDataService::Ins().GetBit(mode_->ChartXYLineConfig.TriggerTypeVar);
				if (mode_->ChartXYLineConfig.TriggerCond == 1)	//下降沿触发
				{
					if (preSampleStatus_ == true && status == false)
						GetSampleData();		//采样数据
				}
				else				//上升沿触发
				{
					if (preSampleStatus_ == false && status == true)
						GetSampleData();
				}
				preSampleStatus_ = status;
			}
		}
		/*暂停采样*/
		if (mode_->ChartXYLineConfig.PauseControlVar.Cmp(varId)) 
		{
			isStop_ = UI::UIData::Bit(mode_->ChartXYLineConfig.PauseControlVar);
			if ((isStop_ == true && mode_->ChartXYLineConfig.PauseControlCondition == 0) ||
				(isStop_ == false && mode_->ChartXYLineConfig.PauseControlCondition == 1))
			{
				if (mode_->ChartXYLineConfig.RefreshMode == 0)				//周期采样
				{
					if (HasTimer)
						Page()->RemoveTimeout(XYChartTimerCb, (void *)this);	//删除定时器
					isSample_ = false;
				}
			}
			else			//开始采样
			{
				if (mode_->ChartXYLineConfig.RefreshMode == 0)
				{
					if (!isSample_)		//没有开始采样
					{
						if (HasTimer)
						{
							Page()->RemoveTimeout(XYChartTimerCb, (void*)this);
							if (mode_->ChartXYLineConfig.SamplingCycle != 0)
								Page()->AddTimeout(mode_->ChartXYLineConfig.SamplingCycle, XYChartTimerCb, (void *)this, true);
						}
						else
						{
							if (mode_->ChartXYLineConfig.SamplingCycle != 0)
								Page()->AddTimeout(mode_->ChartXYLineConfig.SamplingCycle, XYChartTimerCb, (void *)this, true);
							HasTimer = true;
						}
						isSample_ = true;
					}
				}
			}
		}
		/*采样个数*/
		if (mode_->ChartXYLineConfig.SamplePointsVar.Cmp(varId))
		{
			pView->SampleNum = UI::UIData::Number<int>(mode_->ChartXYLineConfig.SamplePointsVar);
			GetSampleData();
		}
		bool needredraw = false;
		/*翻页*/
		if (mode_->ChartXYLineConfig.TurnControlVar.Cmp(varId))
		{
			pView->PageNo = UI::UIData::Number<int>(mode_->ChartXYLineConfig.TurnControlVar);
			if (pView->PageNo <= pView->SampleNum / mode_->ChartXYLineConfig.PointsPerScreen)
				needredraw = true;
		}
		/*清除采样*/
		if (mode_->ChartXYLineConfig.ClearControlVar.Cmp(varId))
		{
			bool status = UI::UIData::Bit(mode_->ChartXYLineConfig.ClearControlVar);
			if ((status == true && preClearStatus_ == false && mode_->ChartXYLineConfig.ClearControlCondition == 0) ||
				(status == false && preClearStatus_ == true && mode_->ChartXYLineConfig.ClearControlCondition == 1))
			{
				vector<vector<DDWORD>>().swap(pView->XData);
				vector<vector<DDWORD>>().swap(pView->YData);
				//UI::UIData::Bit(mode_->ChartXYLineConfig.ClearControlVar, false);
				preClearStatus_ = status;
				needredraw = true;
			}
		}
		/*上下限*/
		if (mode_->ChartXYLineConfig.XScaleSet.UpperLimitVar.Cmp(varId))
		{
			needredraw = true;
			mode_->ChartXYLineConfig.XScaleSet.UpperLimit = UI::UIData::Number<DOUBLE>(mode_->ChartXYLineConfig.XScaleSet.UpperLimitVar);
		}
		if (mode_->ChartXYLineConfig.XScaleSet.LowerLimitVar.Cmp(varId))
		{
			needredraw = true;
			mode_->ChartXYLineConfig.XScaleSet.LowerLimit = UI::UIData::Number<DOUBLE>(mode_->ChartXYLineConfig.XScaleSet.LowerLimitVar);
		}
		if (mode_->ChartXYLineConfig.YScaleSet.UpperLimitVar.Cmp(varId))
		{
			needredraw = true;
			mode_->ChartXYLineConfig.YScaleSet.UpperLimit = UI::UIData::Number<DOUBLE>(mode_->ChartXYLineConfig.YScaleSet.UpperLimitVar);
		}
		if (mode_->ChartXYLineConfig.YScaleSet.LowerLimitVar.Cmp(varId))
		{
			needredraw = true;
			mode_->ChartXYLineConfig.YScaleSet.LowerLimit = UI::UIData::Number<DOUBLE>(mode_->ChartXYLineConfig.YScaleSet.LowerLimitVar);
		}
		/*参考线*/
		for (unsigned i = 0; i < mode_->ChartXYLineConfig.ReferenceLines.size(); i++)
		{
			if (mode_->ChartXYLineConfig.ReferenceLines[i].LineValueVar.Cmp(varId))
			{
				needredraw = true;
				mode_->ChartXYLineConfig.ReferenceLines[i].LineValue = UI::UIData::Number<DOUBLE>(mode_->ChartXYLineConfig.ReferenceLines[i].LineValueVar);
			}
		}
		for (unsigned i = 0; i < mode_->ChartXYLineConfig.DataChanel.size(); i++)
		{
			if (mode_->ChartXYLineConfig.DataChanel[i].StartXAddr.Cmp(varId))
			{
				XJDataType tp = UIDataService::Ins().GetDataType(mode_->ChartXYLineConfig.DataChanel[i].StartXAddr);
				int num = Utility::DataFormate::GetByteNumByInfo(tp);
				//BYTE *ddata = new BYTE[num * pView->SampleNum * 2 + 2];
				if (mode_->ChartXYLineConfig.DataChanel[i].IsXYDataFromSameSource)	//XY相同数据区
				{
					vector<BYTE> ddata = DataApi::AppVarDatas(mode_->ChartXYLineConfig.DataChanel[i].StartXAddr);
					//UIDataService::Ins().GetDataBytes(mode_->ChartXYLineConfig.DataChanel[i].StartXAddr, ddata, pView->SampleNum * 2 * num + 2);
					//float a1 = UI::UIData::Number<float>(mode_->ChartXYLineConfig.DataChanel[i].StartXAddr);
					HandleSampleData(ddata.data(), pView->SampleNum * 2, i, true);
					//HandleSampleData(ddata, 1, i, true);
					if (i == mode_->ChartXYLineConfig.DataChanel.size() - 1)
						needredraw = true;
				}
				else
				{
					vector<BYTE> ddata = DataApi::AppVarDatas(mode_->ChartXYLineConfig.DataChanel[i].StartXAddr);
					//UIDataService::Ins().GetDataBytes(mode_->ChartXYLineConfig.DataChanel[i].StartXAddr, ddata, pView->SampleNum * num + 2);
					HandleSampleData(ddata.data(), pView->SampleNum, i, true);
				}
				//delete []ddata;
			}
			if (mode_->ChartXYLineConfig.DataChanel[i].StartYAddr.Cmp(varId))
			{
				XJDataType tp = UIDataService::Ins().GetDataType(mode_->ChartXYLineConfig.DataChanel[i].StartYAddr);
				int num = Utility::DataFormate::GetByteNumByInfo(tp);
				vector<BYTE> ddata = DataApi::AppVarDatas(mode_->ChartXYLineConfig.DataChanel[i].StartYAddr);
				//BYTE *ddata = new BYTE[num * pView->SampleNum + 2];
				//UIDataService::Ins().GetDataBytes(mode_->ChartXYLineConfig.DataChanel[i].StartYAddr, ddata, pView->SampleNum * num + 2);
				HandleSampleData(ddata.data(), pView->SampleNum, i, false);
				if (i == mode_->ChartXYLineConfig.DataChanel.size() - 1)
					needredraw = true;
				//delete[]ddata;
			}
		}
		if (needredraw)
			UI::ViewShowUtility::ShowView(pView, mode_->ChartXYLineConfig.Perm,
				mode_->ChartXYLineConfig.X + mode_->ChartXYLineConfig.OffX,
				mode_->ChartXYLineConfig.Y + mode_->ChartXYLineConfig.OffY);	
	}

	int XYLineChartControl::PeekHMIMessage(Message::Msg* msg)
	{
		XYLineChartView* pView = static_cast<XYLineChartView*>(pView_);
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
