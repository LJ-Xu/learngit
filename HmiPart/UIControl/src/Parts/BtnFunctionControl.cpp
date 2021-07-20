/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BtnFunctionControl.cpp
 * Author   : qiaodan
 * Date     : 2020-10-29
 * Descript : 创建功能按键绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "BtnFunctionControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
#include "ProjectDataTables.h"
#include "System.h"
#include "XJMacro.h"
#include "PrintSetControl.h"
#include <iostream>
#include <fstream> 
#include <sstream> 
#include "RecipeStorage.h"
#include "OperatorGControl.h"
#include "Logger.h"
#include "RecipeUtility.h"
namespace UI
{
	BtnFunctionControl::BtnFunctionControl(HMIPage* w) :BaseControl(w)
	{
		//创建KeyBtnModel
		mode_ = std::shared_ptr<BtnFunctionModel>(new BtnFunctionModel());
		InitMVCModel(mode_);
		vector<Project::BtnFunctionRes>().swap(TimerTask);
	}

	void BtnFunctionControl::CreateView()
	{
		//初始化VIEW
		BtnFunctionView* view = new BtnFunctionView(
			mode_->FuncBtnConfig.X + mode_->FuncBtnConfig.OffX,
			mode_->FuncBtnConfig.Y + mode_->FuncBtnConfig.OffY,
			mode_->FuncBtnConfig.Width,
			mode_->FuncBtnConfig.Height, nullptr);
		InitMVCView(view);
	}

	void BtnFunctionControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->FuncBtnConfig.VOffX != Project::DataVarId::NullId)
			mode_->FuncBtnConfig.OffX = UI::UIData::Number<int>(mode_->FuncBtnConfig.VOffX) + pageoffx;
		else
			mode_->FuncBtnConfig.OffX = pageoffx;


		if (mode_->FuncBtnConfig.VOffY != Project::DataVarId::NullId)
			mode_->FuncBtnConfig.OffY = UI::UIData::Number<int>(mode_->FuncBtnConfig.VOffY) + pageoffy;
		else
			mode_->FuncBtnConfig.OffY = pageoffy;
	}

	bool BtnFunctionControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->FuncBtnConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->FuncBtnConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->FuncBtnConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->FuncBtnConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->FuncBtnConfig.Perm, mode_->FuncBtnConfig.X + mode_->FuncBtnConfig.OffX, mode_->FuncBtnConfig.Y + mode_->FuncBtnConfig.OffY);
				if (needswitchwin_)
				{
					needswitchwin_ = false;
					if (switchwinno_ > 5000)
						Win()->OpenDialogPage(switchwinno_);
					if (switchwinno_ > 0 && switchwinno_ <= 5000)
					{
						Win()->SwitchPage(switchwinno_);
						switchwinno_ = 0;
						return false;
					}
					switchwinno_ = 0;
				}
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}

	void SetDataTimerCb(void *data)
	{
		static DDWORD timercount = 1;
		BtnFunctionControl *ctrl = (BtnFunctionControl *)data;
		if (ctrl->StartTimer)
		{
			ctrl->StartTimer = false;
			timercount = 1;
		}
		for (size_t i = 0; i < ctrl->TimerTask.size(); i++)
		{
			if (ctrl->TimerTask[i].FunctionName == "SetData" &&
				(timercount * 100) % (int)ctrl->TimerTask[i].FunctionParam.SetData.Rate == 0)			//是否是设置数据
				ctrl->HandleSetData(ctrl->TimerTask[i].FunctionParam);
		}
		if (timercount == 0xFFFFFFFF)
			timercount = 1;
		else
			timercount++;
	} 
	void BtnFunctionControl::OnReady()
	{
		HandleSysChange(Permission);
	}
	bool BtnFunctionControl::HandlePermLimit(int region, bool ispopwin)
	{
		if ((region != 0) && (!UI::PermUtility::HasUserPerm(region)))		//用户设置了安全权限并且没有权限
		{
			if (ispopwin)				//弹出提示窗口
			{
				Win()->OpenDialogPage(SafeTipsWinNum);
				return false;
			}
		}
		return true;
	}

	bool BtnFunctionControl::HandleBtnFunc(int action)
	{
		//vector<Project::BtnFunctionRes> reses;
		vector<string> funcname;
		switch (action)
		{
		case FunctionAct::PRESS:
		{
			vector<Project::BtnFunctionRes>().swap(TimerTask);
			switchwin_ = false;				//不需要切换画面
			bool needaddtimer = false;		//是否需要添加定时器
			LOG_INFO_("Btn Function Handle Press Func\n");
			for (size_t i = 0; i < mode_->FuncBtnConfig.Press.size(); i++)
			{
				if (!HandlePermLimit(mode_->FuncBtnConfig.Press[i].RequireRegion, mode_->FuncBtnConfig.Press[i].PopTipWin))
					return false;
				if (switchwin_)		//已经切换画面，不执行动作
					return false;
				funcname.push_back(mode_->FuncBtnConfig.Press[i].FunctionName);
				if (mode_->FuncBtnConfig.Press[i].FunctionName == "SetCoil")
					HandleSetCoil(mode_->FuncBtnConfig.Press[i].FunctionParam);
				if (mode_->FuncBtnConfig.Press[i].FunctionName == "SetData")
				{
					//按下状态且递增递减
					HandleSetData(mode_->FuncBtnConfig.Press[i].FunctionParam);		//先处理获取数值
					if (mode_->FuncBtnConfig.Press[i].FunctionParam.SetData.ActionData == Project::DataAction::INCREASE ||
							mode_->FuncBtnConfig.Press[i].FunctionParam.SetData.ActionData == Project::DataAction::DECREASE)
					{
						TimerTask.push_back(mode_->FuncBtnConfig.Press[i]);			//添加定时器任务
						needaddtimer = true;					//需要添加定时器
					}
				}
				if (mode_->FuncBtnConfig.Press[i].FunctionName == "Arithmetic")
					HandleArithmetic(mode_->FuncBtnConfig.Press[i].FunctionParam);
				if (mode_->FuncBtnConfig.Press[i].FunctionName == "SendData")
					HandleSendData(mode_->FuncBtnConfig.Press[i].FunctionParam);
				if (mode_->FuncBtnConfig.Press[i].FunctionName == "SwitchScreen")
					HandleSwitchScreen(mode_->FuncBtnConfig.Press[i].FunctionParam);
				if (mode_->FuncBtnConfig.Press[i].FunctionName == "OpenWin")
					HandleOpenWin(mode_->FuncBtnConfig.Press[i].FunctionParam);
				if (mode_->FuncBtnConfig.Press[i].FunctionName == "CloseWin")
					HandleCloseWin(mode_->FuncBtnConfig.Press[i].FunctionParam);
				if (mode_->FuncBtnConfig.Press[i].FunctionName == "ImportCSVdata")
					HandleImportCSVdata(mode_->FuncBtnConfig.Press[i]);
				if (mode_->FuncBtnConfig.Press[i].FunctionName == "ExportCSVdata")
					HandleExportCSVdata(mode_->FuncBtnConfig.Press[i]);
				if (mode_->FuncBtnConfig.Press[i].FunctionName == "DownloadRecipe")
					HandleDownloadRecipe(mode_->FuncBtnConfig.Press[i]);
				if (mode_->FuncBtnConfig.Press[i].FunctionName == "UploadRecipe")
					HandleUploadRecipe(mode_->FuncBtnConfig.Press[i]);
				if (mode_->FuncBtnConfig.Press[i].FunctionName == "CallbackFunc")
					HandleCallbackFunc(mode_->FuncBtnConfig.Press[i]);
				if (mode_->FuncBtnConfig.Press[i].FunctionName == "PrintScreen")
					HandlePrintScreen(mode_->FuncBtnConfig.Press[i].FunctionParam);
				if (mode_->FuncBtnConfig.Press[i].FunctionName == "EditRecipe")
					HandleEditRecipe(mode_->FuncBtnConfig.Press[i].FunctionParam);
			}
			if (mode_->FuncBtnConfig.IsRecord)
				OperatorGControl::Ins()->AddOperatorRecord(Page()->Winno(), mode_->FuncBtnConfig.CtrlName,
					Storage::OperatorAction::OA_PRESS, funcname);
			if (needaddtimer)				//添加定时器，执行递增递减动作
			{
				StartTimer = true;
				Page()->AddTimeout(100, SetDataTimerCb, (void*)this, true);
			}
			if (switchwin_)					//切换页面，取消执行释放动作，
			{
				//Page()->RemoveTimeout(SetDataTimerCb, (void*)this);
				StartTimer = false;			//用于清除定时器计数
				return false;
			}

			break;
		}
		case FunctionAct::RELEASE:
		{
			/*删除按下时添加的定时器*/
			if (switchwin_)		//已经切换画面，不执行动作
				return false;
			Page()->RemoveTimeout(SetDataTimerCb, (void*)this);
			LOG_INFO_("Btn Function Handle Release Func\n");
			for (size_t i = 0; i < mode_->FuncBtnConfig.Release.size(); i++)
			{
				if (switchwin_)		//已经切换画面，不执行动作
					return false;
				funcname.push_back(mode_->FuncBtnConfig.Release[i].FunctionName);
				if (mode_->FuncBtnConfig.Release[i].FunctionName == "SetCoil")
					HandleSetCoil(mode_->FuncBtnConfig.Release[i].FunctionParam);
				if (mode_->FuncBtnConfig.Release[i].FunctionName == "SetData")
					HandleSetData(mode_->FuncBtnConfig.Release[i].FunctionParam);		//先处理获取数值
				if (mode_->FuncBtnConfig.Release[i].FunctionName == "Arithmetic")
					HandleArithmetic(mode_->FuncBtnConfig.Release[i].FunctionParam);
				if (mode_->FuncBtnConfig.Release[i].FunctionName == "SendData")
					HandleSendData(mode_->FuncBtnConfig.Release[i].FunctionParam);
				if (mode_->FuncBtnConfig.Release[i].FunctionName == "SwitchScreen")
					HandleSwitchScreen(mode_->FuncBtnConfig.Release[i].FunctionParam);
				if (mode_->FuncBtnConfig.Release[i].FunctionName == "OpenWin")
					HandleOpenWin(mode_->FuncBtnConfig.Release[i].FunctionParam);
				if (mode_->FuncBtnConfig.Release[i].FunctionName == "CloseWin")
					HandleCloseWin(mode_->FuncBtnConfig.Release[i].FunctionParam);
				if (mode_->FuncBtnConfig.Release[i].FunctionName == "ImportCSVdata")
					HandleImportCSVdata(mode_->FuncBtnConfig.Release[i]);
				if (mode_->FuncBtnConfig.Release[i].FunctionName == "ExportCSVdata")
					HandleExportCSVdata(mode_->FuncBtnConfig.Release[i]);
				if (mode_->FuncBtnConfig.Release[i].FunctionName == "DownloadRecipe")
					HandleDownloadRecipe(mode_->FuncBtnConfig.Release[i]);
				if (mode_->FuncBtnConfig.Release[i].FunctionName == "UploadRecipe")
					HandleUploadRecipe(mode_->FuncBtnConfig.Release[i]);
				if (mode_->FuncBtnConfig.Release[i].FunctionName == "CallbackFunc")
					HandleCallbackFunc(mode_->FuncBtnConfig.Release[i]);
				if (mode_->FuncBtnConfig.Release[i].FunctionName == "PrintScreen")
					HandlePrintScreen(mode_->FuncBtnConfig.Release[i].FunctionParam);
				if (mode_->FuncBtnConfig.Release[i].FunctionName == "EditRecipe")
					HandleEditRecipe(mode_->FuncBtnConfig.Release[i].FunctionParam);
			}
			if (mode_->FuncBtnConfig.IsRecord)
				OperatorGControl::Ins()->AddOperatorRecord(Page()->Winno(), mode_->FuncBtnConfig.CtrlName,
					Storage::OperatorAction::OA_RELEASE, funcname);
			if (switchwin_)		//已经切换画面，不执行动作
				return false;
			break;
		}
		default:
			break;
		}
		/*清空timerTask*/
		return true;

	}

	void BtnFunctionControl::HandleSetCoil(Project::FunctionSetup param)
	{
		switch (param.SetCoil.ActionCoil)
		{
		case Project::CoilAction::SETON:
			UI::UIData::Bit(param.SetCoil.RegVar, true);
			break;
		case Project::CoilAction::SETOFF:
			UI::UIData::Bit(param.SetCoil.RegVar, false);
			break;
		case Project::CoilAction::OPPOSITE:
		{
			bool status = UI::UIData::Bit(param.SetCoil.RegVar);
			status = !status;
			UI::UIData::Bit(param.SetCoil.RegVar, status);
			break;
		}
		default:
			break;
		}
	}
	void BtnFunctionControl::HandleSetData(Project::FunctionSetup &param)
	{
		switch (param.SetData.ActionData)
		{
		case Project::DataAction::CONSTANT:
		{
			if (param.SetData.ValueVar != Project::DataVarId::NullId)
				param.SetData.Value = UI::UIData::Number<DOUBLE>(param.SetData.ValueVar);
			UI::UIData::Number(param.SetData.WriteVarIdRef, param.SetData.Value);
			break;
		}
		case Project::DataAction::ADD:
		{
			if (param.SetData.IncreaseValueVar != Project::DataVarId::NullId)
				param.SetData.IncreaseValue = UI::UIData::Number<DOUBLE>(param.SetData.IncreaseValueVar);
			if (UI::DataApi::GetDataFlag(param.SetData.WriteVarIdRef))
			{
				DOUBLE writedata = UI::UIData::Number<DOUBLE>(param.SetData.WriteVarIdRef);
				writedata += param.SetData.IncreaseValue;
				if (param.SetData.UpperLmtValueVar != Project::DataVarId::NullId)
					param.SetData.UpperLmtValue = UI::UIData::Number<DOUBLE>(param.SetData.UpperLmtValueVar);
				if (param.SetData.LowerLmtValueVar != Project::DataVarId::NullId)
					param.SetData.LowerLmtValue = UI::UIData::Number<DOUBLE>(param.SetData.LowerLmtValueVar);
				if (writedata > param.SetData.UpperLmtValue)
				{
					if (param.SetData.Loop)		//使用周期循环
						UI::UIData::Number(param.SetData.WriteVarIdRef, param.SetData.LowerLmtValue);
					else
						UI::UIData::Number(param.SetData.WriteVarIdRef, param.SetData.UpperLmtValue);
				}
				else
					UI::UIData::Number(param.SetData.WriteVarIdRef, writedata);
			}
			break;

		}
		case Project::DataAction::MINUS:
		{
			if (param.SetData.IncreaseValueVar != Project::DataVarId::NullId)
				param.SetData.IncreaseValue = UI::UIData::Number<DOUBLE>(param.SetData.IncreaseValueVar);
			if (UI::DataApi::GetDataFlag(param.SetData.WriteVarIdRef))
			{
				DOUBLE writedata = UI::UIData::Number<DOUBLE>(param.SetData.WriteVarIdRef);
				writedata -= param.SetData.IncreaseValue;
				if (param.SetData.UpperLmtValueVar != Project::DataVarId::NullId)
					param.SetData.UpperLmtValue = UI::UIData::Number<DOUBLE>(param.SetData.UpperLmtValueVar);
				if (param.SetData.LowerLmtValueVar != Project::DataVarId::NullId)
					param.SetData.LowerLmtValue = UI::UIData::Number<DOUBLE>(param.SetData.LowerLmtValueVar);
				if (writedata < param.SetData.LowerLmtValue)
				{
					if (param.SetData.Loop)		//使用周期循环
						UI::UIData::Number(param.SetData.WriteVarIdRef, param.SetData.UpperLmtValue);
					else
						UI::UIData::Number(param.SetData.WriteVarIdRef, param.SetData.LowerLmtValue);
				}
				else
					UI::UIData::Number(param.SetData.WriteVarIdRef, writedata);
			}
			break;

		}
		case Project::DataAction::INCREASE:
		{
			if (param.SetData.IncreaseValueVar != Project::DataVarId::NullId)
				param.SetData.IncreaseValue = UI::UIData::Number<DOUBLE>(param.SetData.IncreaseValueVar);
			DOUBLE writedata = UI::UIData::Number<DOUBLE>(param.SetData.WriteVarIdRef);
			writedata += param.SetData.IncreaseValue;
			if (param.SetData.UpperLmtValueVar != Project::DataVarId::NullId)
				param.SetData.UpperLmtValue = UI::UIData::Number<DOUBLE>(param.SetData.UpperLmtValueVar);
			if (param.SetData.LowerLmtValueVar != Project::DataVarId::NullId)
				param.SetData.LowerLmtValue = UI::UIData::Number<DOUBLE>(param.SetData.LowerLmtValueVar);
			if (writedata > param.SetData.UpperLmtValue)
			{
				if (param.SetData.Loop)		//使用周期循环
					UI::UIData::Number(param.SetData.WriteVarIdRef, param.SetData.LowerLmtValue);
				else
					UI::UIData::Number(param.SetData.WriteVarIdRef, param.SetData.UpperLmtValue);
			}
			else
				UI::UIData::Number(param.SetData.WriteVarIdRef, writedata);
			break;
		}
		case Project::DataAction::DECREASE:
		{
			if (param.SetData.IncreaseValueVar != Project::DataVarId::NullId)
				param.SetData.IncreaseValue = UI::UIData::Number<DOUBLE>(param.SetData.IncreaseValueVar);
			DOUBLE writedata = UI::UIData::Number<DOUBLE>(param.SetData.WriteVarIdRef);
			writedata -= param.SetData.IncreaseValue;
			if (param.SetData.LowerLmtValueVar != Project::DataVarId::NullId)
				param.SetData.LowerLmtValue = UI::UIData::Number<DOUBLE>(param.SetData.LowerLmtValueVar);
			if (param.SetData.UpperLmtValueVar != Project::DataVarId::NullId)
				param.SetData.UpperLmtValue = UI::UIData::Number<DOUBLE>(param.SetData.UpperLmtValueVar);
			if (writedata < param.SetData.LowerLmtValue)
			{
				if (param.SetData.Loop)		//使用周期循环
					UI::UIData::Number(param.SetData.WriteVarIdRef, param.SetData.UpperLmtValue);
				else
					UI::UIData::Number(param.SetData.WriteVarIdRef, param.SetData.LowerLmtValue);
			}
			else
				UI::UIData::Number(param.SetData.WriteVarIdRef, writedata);
			break;
		}
		default:
			break;
		}
	}
	void BtnFunctionControl::HandleArithmetic(Project::FunctionSetup param)
	{
		DDWORD leftvalue, rightvalue;
		XJDataType lefttp, righttp;
		/*获取左值*/
		if (param.Arithmetic.Left.ValueVar != Project::DataVarId::NullId)
			param.Arithmetic.Left.Value = UI::UIData::Number<double>(param.Arithmetic.Left.ValueVar);
			memcpy(&leftvalue, &param.Arithmetic.Left.Value, sizeof(DDWORD));
			lefttp.Cls = VarDataType::DT_DDWORD;
			lefttp.Type = Project::VarNumberType::NT_Float;
			
		/*获取右值*/
		if (param.Arithmetic.Right.ValueVar != Project::DataVarId::NullId)
			param.Arithmetic.Right.Value = UI::UIData::Number<double>(param.Arithmetic.Right.ValueVar);
			memcpy(&rightvalue, &param.Arithmetic.Right.Value, sizeof(DDWORD));
			righttp.Cls = VarDataType::DT_DDWORD;
			righttp.Type = Project::VarNumberType::NT_Float;

		DDWORD value;
		XJDataType valuetp = UI::UIDataService::Ins().GetDataType(param.Arithmetic.WriteVarRef);
		switch (param.Arithmetic.ActionMath)
		{
		case Project::MathAction::ADDTION:
			value = Utility::DataFormate::Add(leftvalue, lefttp, rightvalue, righttp);
			break;
		case Project::MathAction::SUB:
			value = Utility::DataFormate::Sub(leftvalue, lefttp, rightvalue, righttp);
			break;
		case Project::MathAction::MULTIPLY:
			value = Utility::DataFormate::Mul(leftvalue, lefttp, rightvalue, righttp);
			break;
		case Project::MathAction::DIVISION:
			value = Utility::DataFormate::Div(leftvalue, lefttp, rightvalue, righttp);
			break;
		default:
			break;
		}
		if (param.Arithmetic.UpperUsed)		//使用上限
		{
			DDWORD max;
			XJDataType maxtp;
			if (param.Arithmetic.UpperLmtValueVar != Project::DataVarId::NullId)
			{
				max = UI::UIDataService::Ins().GetNumber(param.Arithmetic.UpperLmtValueVar);
				maxtp = UI::UIDataService::Ins().GetDataType(param.Arithmetic.UpperLmtValueVar);
			}
			else
			{
				memcpy(&max, &param.Arithmetic.UpperLmtValue, sizeof(float));
				maxtp.Cls = VarDataType::DT_DWORD;
				maxtp.Type = Project::VarNumberType::NT_Float;
			}
			if (Utility::DataFormate::Sub(value, valuetp, max, maxtp) > 0)
				value = max;
		}
		if (param.Arithmetic.LowerUsed)		//使用下限
		{
			DDWORD min;
			XJDataType mintp;
			if (param.Arithmetic.UpperLmtValueVar != Project::DataVarId::NullId)
			{
				min = UI::UIDataService::Ins().GetNumber(param.Arithmetic.UpperLmtValueVar);
				mintp = UI::UIDataService::Ins().GetDataType(param.Arithmetic.UpperLmtValueVar);
			}
			else
			{
				memcpy(&min, &param.Arithmetic.UpperLmtValue, sizeof(float));
				mintp.Cls = VarDataType::DT_DWORD;
				mintp.Type = Project::VarNumberType::NT_Float;
			}
			if (Utility::DataFormate::Sub(value, valuetp, min, mintp) < 0)
				value = min;
		}
		value = Utility::DataFormate::NumParse(value, valuetp);
		//UI::UIData::Number(param.Arithmetic.WriteVarRef, value);
		UI::UIDataService::Ins().SetNumber(param.Arithmetic.WriteVarRef, value);
	}
	void BtnFunctionControl::HandleSendData(Project::FunctionSetup param)
	{
		UI::UIDataService::Ins().MoveDataVars(param.SendData.SrcVar, param.SendData.DstVar, param.SendData.RegNum);
	}
	void BtnFunctionControl::HandleSwitchScreen(Project::FunctionSetup param)
	{
		int curwinno = Page()->Winno();			//获取当前页面
		switch (param.SwitchScreen.Frame)
		{
		case Project::FrameAction::START:
		{
			if(curwinno != Win()->SwitchStartPage())
				switchwin_ = true;
			break;
		}
		case Project::FrameAction::FRONT:
		{
			int frontwinno = Page()->LastWinno();
			if (!Win()->PageHasPrem(frontwinno, param.SwitchScreen.UsePasswd))		//没有权限
			{
				needswitchwin_ = true;
				switchwinno_ = frontwinno;
				return;
			}
			if(curwinno != frontwinno)
				switchwin_ = true;
			Win()->GobackPage();
			break;
		}
		case Project::FrameAction::NUM:
		{
			if (curwinno != param.SwitchScreen.FrameNum)
			{
				switchwin_ = true;
				if (!Win()->PageHasPrem(param.SwitchScreen.FrameNum, param.SwitchScreen.UsePasswd))		//没有权限
				{
					needswitchwin_ = true;
					switchwinno_ = param.SwitchScreen.FrameNum;
					return;
				}
			}
			Win()->SwitchPage(param.SwitchScreen.FrameNum);
			break;
		}
		default:
			break;
		}
	}
	void BtnFunctionControl::HandleOpenWin(Project::FunctionSetup param)
	{
		int curwinno = Page()->Winno();			//获取当前页面
		switch (param.OpenWin.ActionWin)
		{
		case Project::WinAction::SWITCH:
		{
			if (!Win()->PageHasPrem(param.OpenWin.ExchangeWinNo, param.OpenWin.UsePasswd))		//没有权限
			{
				needswitchwin_ = true;
				switchwinno_ = param.OpenWin.ExchangeWinNo;
				return;
			}
			if (curwinno > 5000)		//窗口
			{
				Win()->SwitchDialogPage(param.OpenWin.ExchangeWinNo);
				Win()->ClosePage(curwinno);
				switchwin_ = true;
			}
			else
			{
				Win()->OpenDialogPage(param.OpenWin.ExchangeWinNo);
			}
			//if (curwinno != param.OpenWin.ExchangeWinNo)
			//Win()->SwitchDialogPage(param.OpenWin.ExchangeWinNo);
			//Win()->ClosePage(curwinno);
			break;
		}
		case Project::WinAction::OPEN:
			if (!Win()->PageHasPrem(param.OpenWin.PopUpWinNo, param.OpenWin.UsePasswd))		//没有权限
			{
				needswitchwin_ = true;
				switchwinno_ = param.OpenWin.PopUpWinNo;
				return;
			}
			Win()->OpenDialogPage(param.OpenWin.PopUpWinNo);
			break;
		default:
			break;
		}
		//if (param.OpenWin.UsePasswd)		//打开密码页面
		//	Win()->OpenDialogPage(2);
	}
	void BtnFunctionControl::HandleCloseWin(Project::FunctionSetup param)
	{
		int curwinno = Page()->Winno();			//获取当前页面
		if (param.CloseWin.CloseAction)
		{
			if (curwinno == param.CloseWin.CloseNum)
				switchwin_ = true;
			Win()->ClosePage(param.CloseWin.CloseNum);
		}
		else
		{
			HMIPage* pg = Page()->Next;
			while (pg != Page())
			{
				if (pg->Winno() == curwinno)
				{
					switchwin_ = true;
					break;
				}
				HMIPage* tp = pg->Next;
				pg = tp;
			}
			Win()->CloseAllDialog();
		}
	}
	void BtnFunctionControl::HandleImportCSVdata(Project::BtnFunctionRes res)
	{
		//导入CSVtoPSW
		Project::FunctionSetup param = res.FunctionParam;
		string filename;
		switch (param.ImportCSVdata.NameType)
		{
		case Project::FlieNameType::FIX:
			filename = res.FileName;
			break;
		case Project::FlieNameType::DATA:
		{
			filename = System::GetCurrentDateToString() + ".csv";
			break;
		}
		case Project::FlieNameType::DYNAMIC:
		{
			if (param.ImportCSVdata.NameVar != Project::DataVarId::NullId)
				filename = UIDataService::Ins().GetString(param.ImportCSVdata.NameVar) + ".csv";
			break;
		}
		default:
			break;
		}
		if (param.ImportCSVdata.LocationVar != Project::DataVarId::NullId)
			param.ImportCSVdata.FileAddr = (Project::FlieLocation)UI::DataApi::AppNumber<int>(param.ImportCSVdata.LocationVar);
		vector<Project::ColDataTypeInfo> typeinfos;
		int regnum = 0;
		for (size_t i = 0; i < (size_t)param.ImportCSVdata.DataSize; i++)
		{
			ColDataTypeInfo info;
			info.DataFmt = (Project::VarNumberType)res.Data[i].DataFmt;
			info.DataType = (Project::VarDataType)res.Data[i].DataType;
			info.RegCount = res.Data[i].Number;
			info.ColTitle = res.Data[i].Title;
			typeinfos.push_back(info);
			regnum += info.RegCount;
		}
		LOG_INFO("Import CSV %s From PLC ,count = %d\n", filename.c_str(), param.ImportCSVdata.Count * regnum);
		DataApi::DataImportCSV(param.ImportCSVdata.FileAddr, filename, param.ImportCSVdata.Count * regnum,
			param.ImportCSVdata.OrgVarIdRef, typeinfos, param.ImportCSVdata.StatusVarRef,
			param.ImportCSVdata.ResultVarRef, param.ImportCSVdata.ProgressVarRef);
	}
	void BtnFunctionControl::HandleExportCSVdata(Project::BtnFunctionRes res)
	{
		//csv导出from PSW
		//获取文件名称
		Project::FunctionSetup param = res.FunctionParam;
		string filename;
		switch (param.ExportCSVdata.NameType)
		{
		case Project::FlieNameType::FIX:
			filename = res.FileName;
			break;
		case Project::FlieNameType::DATA:
		{
			filename = System::GetCurrentDateToString() + ".csv";
			//filename = to_string(year) + '-' + to_string(year) + '-' + to_string(year) + ".csv";
			break;
		}
		case Project::FlieNameType::DYNAMIC:
		{
			if (param.ExportCSVdata.NameVar != Project::DataVarId::NullId)
				filename = UIDataService::Ins().GetString(param.ExportCSVdata.NameVar) + ".csv";
			break;
		}
		default:
			break;
		}
		if (param.ExportCSVdata.LocationVar != Project::DataVarId::NullId)
			param.ExportCSVdata.FileAddr = (Project::FlieLocation)UI::DataApi::AppNumber<int>(param.ExportCSVdata.LocationVar);

		int regnum = 0;
		vector<Project::ColDataTypeInfo> typeinfos;
		for (size_t i = 0; i < (size_t)param.ExportCSVdata.DataSize; i++)
		{
			ColDataTypeInfo info;
			info.DataFmt = (Project::VarNumberType)res.Data[i].DataFmt;
			info.DataType = (Project::VarDataType)res.Data[i].DataType;
			info.RegCount = res.Data[i].Number;
			info.ColTitle = res.Data[i].Title;
			typeinfos.push_back(info);
			regnum += info.RegCount;
		}
		LOG_INFO("Expor CSV %s To PLC ,count = %d\n", filename.c_str(), param.ExportCSVdata.Count * regnum);
		DataApi::DataExportCSV(param.ExportCSVdata.FileAddr, filename, param.ExportCSVdata.Count * regnum,
			param.ExportCSVdata.OrgVarIdRef, typeinfos, param.ExportCSVdata.StatusVarRef,
			param.ExportCSVdata.ResultVarRef, param.ExportCSVdata.ProgressVarRef);
	}
	void BtnFunctionControl::HandleDownloadRecipe(Project::BtnFunctionRes res)
	{
		Project::FunctionSetup param = res.FunctionParam;
		vector<Project::ColDataTypeInfo> colinfo;
		RecipeDT::Ins()->GetDataTypes(res.FileName, colinfo);
		int row = Storage::RecipeStorage::Ins()->GetCountByRepiceName(res.FileName);
		LOG_INFO("Upload Recipe %s Form PLC ,count = %d\n", res.FileName, param.DownloadRecipe.Size);
		short index;
		DataVarId indexvar = RecipeDT::Ins()->GetIndexVar(res.FileName);
		if (indexvar == DataVarId::NullId)
			index = LocalData::GetNumberData<short>(SYS_PSW_RecipeIndex);
		else
			index = UI::UIData::Number<short>(indexvar);
		
		DataApi::RecipeToPLC(res.FileName, param.DownloadRecipe.Size, index,
			param.DownloadRecipe.RegVar, colinfo, param.DownloadRecipe.TransferVarIdRef);
	}
	void BtnFunctionControl::HandleUploadRecipe(Project::BtnFunctionRes res)
	{
		Project::FunctionSetup param = res.FunctionParam;
		vector<Project::ColDataTypeInfo> colinfo;
		RecipeDT::Ins()->GetDataTypes(res.FileName, colinfo);
		int row = Storage::RecipeStorage::Ins()->GetCountByRepiceName(res.FileName);
		LOG_INFO("Download Recipe %s To PLC ,count = %d\n", res.FileName, param.UploadRecipe.Size);
		short index;
		DataVarId indexvar = RecipeDT::Ins()->GetIndexVar(res.FileName);
		if (indexvar == DataVarId::NullId)
			index = LocalData::GetNumberData<short>(SYS_PSW_RecipeIndex);
		else
			index = UI::UIData::Number<short>(indexvar);

		DataApi::RecipeFromPLC(res.FileName, param.UploadRecipe.Size, index,
			param.UploadRecipe.RegVar, colinfo, param.UploadRecipe.TransferVarIdRef);
	}
	void BtnFunctionControl::HandleCallbackFunc(Project::BtnFunctionRes res)
	{
		//string name = param.CallbackFunc.CbFuncName;
		Project::FunctionSetup param = res.FunctionParam;
		char * name = new char[res.FileName.size() + 1];
		memset(name, '\0', res.FileName.size() + 1);
		strcpy(name, res.FileName.c_str());
		UI::XJMacro::Trig_Macro(name);
		LOG_INFO("CallbackFunc %s\n", name);
		delete[] name;
		//UI::XJMacro::Trig_Macro(name);
	}
	void BtnFunctionControl::HandlePrintScreen(Project::FunctionSetup param)
	{
		int winno;
		if (param.PrintScreen.ScreenSrc == Project::WinSrc::REGVAR &&
			param.PrintScreen.ScreenVarIdRef != Project::DataVarId::NullId)
			winno = UI::UIData::Number<int>(param.PrintScreen.ScreenVarIdRef);
		else if (param.PrintScreen.ScreenSrc == Project::WinSrc::WINNO)
			winno = param.PrintScreen.WinNo;
		else
			winno = Page()->Winno();
		Fl_Widget *target = Win()->FindPage(winno);
		if (!target)
			target = Win()->ProducePage(winno);
		PrintSetControl::PrintWinPic(target);
	}

	void BtnFunctionControl::HandleEditRecipe(Project::FunctionSetup param)
	{
		switch (param.EditRecipe.Action)
		{
		case Project::AddRecipe:
			RecipeUtility::Ins().Add();
			break;
		case Project::InsertRecipe:
			RecipeUtility::Ins().Insert();
			break;
		case Project::DeleteRecipe:
			RecipeUtility::Ins().Delete();
			break;
		case Project::CopyRecipe:
			RecipeUtility::Ins().Copy();
			break;
		case Project::MoveUpRecipe:
			RecipeUtility::Ins().MoveUp();
			break;
		case Project::MoveDownRecipe:
			RecipeUtility::Ins().MoveDown();
			break;
		default:
			break;
		}
	}

	
	void BtnFunctionControl::HandleDataVar(Project::DataVarId &varId)
	{
		BtnFunctionView* pView = static_cast<BtnFunctionView*>(pView_);
		if (mode_->FuncBtnConfig.VOffX.Cmp(varId))
		{
			mode_->FuncBtnConfig.OffX = UI::UIData::Number<int>(mode_->FuncBtnConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->FuncBtnConfig.Perm, mode_->FuncBtnConfig.X + mode_->FuncBtnConfig.OffX, mode_->FuncBtnConfig.Y + mode_->FuncBtnConfig.OffY);
		}
		if (mode_->FuncBtnConfig.VOffY.Cmp(varId))
		{
			mode_->FuncBtnConfig.OffY = UI::UIData::Number<int>(mode_->FuncBtnConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->FuncBtnConfig.Perm, mode_->FuncBtnConfig.X + mode_->FuncBtnConfig.OffX, mode_->FuncBtnConfig.Y + mode_->FuncBtnConfig.OffY);
		}
		if (mode_->FuncBtnConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->FuncBtnConfig.Perm, pView_);
		if (mode_->FuncBtnConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->FuncBtnConfig.Perm, pView_);
	}

	int BtnFunctionControl::PeekHMIMessage(Message::Msg* msg)
	{
		Project::DataVarId varId;
		switch (msg->GetCode())
		{
		case WM_EVENT_DATEUPDATE:
			varId = Project::DataVarId(msg->LParam);
			HandleDataVar(varId);
			break;
		case WM_EVENT_CLICKED:
			HandleBtnFunc(FunctionAct::PRESS);
			HandleBtnFunc(FunctionAct::RELEASE);
			break;
		default:
			break;
		}
		return 1;//handled;
	}
}
