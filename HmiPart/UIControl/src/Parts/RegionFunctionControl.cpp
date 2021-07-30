/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RegionFunctionControl.cpp
 * Author   : qiaodan
 * Date     : 2020-11-2
 * Descript : 创建功能域绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "RegionFunctionControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
#include "ProjectDataTables.h"
#include "XJMacro.h"
#include "System.h"
#include "RecipeStorage.h"
#include "PrintSetControl.h"
namespace UI
{
	RegionFunctionControl::RegionFunctionControl(HMIPage* w) :BaseControl(w)
	{
		//创建KeyBtnModel
		mode_ = std::shared_ptr<RegionFunctionModel>(new RegionFunctionModel());
		InitMVCModel(mode_);
	}
	void RegionFunctionControl::CreateView()
	{
		//初始化VIEW
		RegionFunctionView* view = new RegionFunctionView(
			mode_->FuncRegionConfig.X + mode_->FuncRegionConfig.OffX,
			mode_->FuncRegionConfig.Y + mode_->FuncRegionConfig.OffY,
			mode_->FuncRegionConfig.Width,
			mode_->FuncRegionConfig.Height, nullptr);
		view->Text = "功能域";
		UI::IResourceService::GB2312toUtf8(view->Text);
		InitMVCView(view);
	}
	bool RegionFunctionControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->FuncRegionConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->FuncRegionConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->FuncRegionConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->FuncRegionConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->FuncRegionConfig.Perm, mode_->FuncRegionConfig.X + mode_->FuncRegionConfig.OffX, mode_->FuncRegionConfig.Y + mode_->FuncRegionConfig.OffY);
				if (needswitchwin_)
				{
					if (!Win()->PageHasPrem(switchwinno_, false))		//没有权限
					{
						needswitchwin_ = false;
						switchwinno_ = 0;
						return true;
					}
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
		return true;
	}


	//void RegionTimerCb(void *data)
	//{
	//	RegionFunctionControl *ctrl = (RegionFunctionControl *)data;
	//	ctrl->StartRegionFunc();		//执行功能
	//}
	void RegionModeTimerCb(void *data)
	{
		RegionFunctionControl *ctrl = (RegionFunctionControl *)data;
		ctrl->StartRegionFunc();		//执行功能
	}

	void RegionFunctionControl::OnReady()
	{
		RegionFunctionView* pView = static_cast<RegionFunctionView*>(pView_);
		pView->Text = "";
		precoilstatus_ = UI::UIData::Bit(mode_->FuncRegionConfig.CoilVar);
		if (mode_->FuncRegionConfig.Perm.EnableVID != Project::DataVarId::NullId)
			UI::PermUtility::HandleEnablePerm(mode_->FuncRegionConfig.Perm, pView_);
		if (mode_->FuncRegionConfig.TimeLmtVar != Project::DataVarId::NullId)
		{
			bool stopstatus = UI::UIData::Bit(mode_->FuncRegionConfig.TimeLmtVar);
			if ((mode_->FuncRegionConfig.StopMode == 1 && stopstatus == false) ||
				(mode_->FuncRegionConfig.StopMode == 0 && stopstatus == true))
				stopfunc_ = true;
		}
		//if (mode_->FuncRegionConfig.Mode == Project::ActionMode::FuncTimer)		//定时模式
		//{
		//	if (mode_->FuncRegionConfig.TimeValueVarId != Project::DataVarId::NullId)
		//		mode_->FuncRegionConfig.Time = UI::UIData::Number<double>(mode_->FuncRegionConfig.TimeValueVarId);
		//}
		TimeCount = 0;
		if (mode_->FuncRegionConfig.Mode == Project::ActionMode::FuncContinue ||
			mode_->FuncRegionConfig.Mode == Project::ActionMode::NumLogic)
			mode_->FuncRegionConfig.Time = 500;			//设置一个默认周期
		if (mode_->FuncRegionConfig.Mode == Project::ActionMode::FuncTimer && !stopfunc_)
		{
			Page()->RemoveTimeout(RegionModeTimerCb, (void*)this);
			//if (mode_->FuncRegionConfig.Time != 0)
			//{
				hastimer_ = true;
				Page()->AddTimeout(100, RegionModeTimerCb, (void*)this, true);
			//}
		}
		if(mode_->FuncRegionConfig.Mode == Project::ActionMode::FuncContinue && !stopfunc_)		//定时模式
		{
			Page()->RemoveTimeout(RegionModeTimerCb, (void*)this);
			if (mode_->FuncRegionConfig.Time != 0)
			{
				hastimer_ = true;
				Page()->AddTimeout((size_t)(mode_->FuncRegionConfig.Time), RegionModeTimerCb, (void*)this, true);
			}
		}
		OnStartPage();
		HandleSysChange(Permission);
	}
	void RegionFunctionControl::OnClose()
	{
		OnClosePage();
	}

	void RegionFunctionControl::OnStartPage()
	{
		if (mode_->FuncRegionConfig.Mode == Project::ActionMode::Startframe)
			StartRegionFunc();
	}
	void RegionFunctionControl::OnClosePage()
	{
		if (mode_->FuncRegionConfig.Mode == Project::ActionMode::CloseFrame)
			StartRegionFunc();
		hastimer_ = false;
		Page()->RemoveTimeout(RegionModeTimerCb, (void*)this);
	}

	void RegionFunctionControl::HandleSetCoil(Project::FunctionSetup param)
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
	void RegionFunctionControl::HandleSetData(Project::FunctionSetup param)
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
			}
			break;

		}
		default:
			break;
		}
	}
	void RegionFunctionControl::HandleArithmetic(Project::FunctionSetup param)
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
	void RegionFunctionControl::HandleSendData(Project::FunctionSetup param)
	{
		UI::UIDataService::Ins().MoveDataVars(param.SendData.SrcVar, param.SendData.DstVar, param.SendData.RegNum);
	}
	void RegionFunctionControl::HandleSwitchScreen(Project::FunctionSetup param)
	{
		int curwinno = Page()->Winno();			//获取当前页面
		switch (param.SwitchScreen.Frame)
		{
		case Project::FrameAction::START:
		{
			if (curwinno != Win()->SwitchStartPage())
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
			if (curwinno != frontwinno)
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
		//if (param.SwitchScreen.UsePasswd)		//打开密码页面
		//	Win()->OpenDialogPage(2);
	}
	void RegionFunctionControl::HandleOpenWin(Project::FunctionSetup param)
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
				switchwin_ = true;
			}
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
	void RegionFunctionControl::HandleCloseWin(Project::FunctionSetup param)
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
		if (param.CloseWin.UsePasswd)
			Win()->OpenDialogPage(2);
	}
	void RegionFunctionControl::HandleImportCSVdata(Project::BtnFunctionRes res)
	{
		//导入CSV
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
		DataApi::DataImportCSV(param.ImportCSVdata.FileAddr, filename, param.ImportCSVdata.Count * regnum,
			param.ImportCSVdata.OrgVarIdRef, typeinfos, param.ImportCSVdata.StatusVarRef,
			param.ImportCSVdata.ResultVarRef, param.ImportCSVdata.ProgressVarRef);
	}
	void RegionFunctionControl::HandleExportCSVdata(Project::BtnFunctionRes res)
	{
		//csv导出
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
		DataApi::DataExportCSV(param.ExportCSVdata.FileAddr, filename, param.ExportCSVdata.Count * regnum,
			param.ExportCSVdata.OrgVarIdRef, typeinfos, param.ExportCSVdata.StatusVarRef,
			param.ExportCSVdata.ResultVarRef, param.ExportCSVdata.ProgressVarRef);
	}
	void RegionFunctionControl::HandleDownloadRecipe(Project::BtnFunctionRes res)
	{
		Project::FunctionSetup param = res.FunctionParam;
		vector<Project::ColDataTypeInfo> colinfo;
		RecipeDT::Ins()->GetDataTypes(res.FileName, colinfo);
		short index;
		DataVarId indexvar = RecipeDT::Ins()->GetIndexVar(res.FileName);
		if (indexvar == DataVarId::NullId)
			index = LocalData::GetNumberData<short>(SYS_PSW_RecipeIndex);
		else
			index = UI::UIData::Number<short>(indexvar);
		int row = Storage::RecipeStorage::Ins()->GetCountByRepiceName(res.FileName);
		DataApi::RecipeToPLC(res.FileName, param.DownloadRecipe.Size, index,
			param.DownloadRecipe.RegVar, colinfo, param.DownloadRecipe.TransferVarIdRef);
	}
	void RegionFunctionControl::HandleUploadRecipe(Project::BtnFunctionRes res)
	{
		Project::FunctionSetup param = res.FunctionParam;
		vector<Project::ColDataTypeInfo> colinfo;
		RecipeDT::Ins()->GetDataTypes(res.FileName, colinfo);
		short index;
		DataVarId indexvar = RecipeDT::Ins()->GetIndexVar(res.FileName);
		if (indexvar == DataVarId::NullId)
			index = LocalData::GetNumberData<short>(SYS_PSW_RecipeIndex);
		else
			index = UI::UIData::Number<short>(indexvar);
		int row = Storage::RecipeStorage::Ins()->GetCountByRepiceName(res.FileName);
		DataApi::RecipeFromPLC(res.FileName, param.UploadRecipe.Size, index,
			param.UploadRecipe.RegVar, colinfo, param.UploadRecipe.TransferVarIdRef);
	}
	void RegionFunctionControl::HandleCallbackFunc(Project::BtnFunctionRes res)
	{
		Project::FunctionSetup param = res.FunctionParam;
		char * name = new char[res.FileName.size() + 1];
		memset(name, '\0', res.FileName.size() + 1);
		strcpy(name, res.FileName.c_str());
		UI::XJMacro::Trig_Macro(name);
		LOG_INFO("CallbackFunc %s\n", name);
		delete[] name;
		/*UI::XJMacro::Trig_Macro(param.CallbackFunc.CbFuncName);*/
	}
	void RegionFunctionControl::HandlePrintScreen(Project::FunctionSetup param)
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
	bool RegionFunctionControl::HavePerm()
	{
		RegionFunctionView* pView = static_cast<RegionFunctionView*>(pView_);
		if (mode_->FuncRegionConfig.Perm.IsEnableVerity && (!pView->active()))		//使用使能控制
			return false;
		if ((mode_->FuncRegionConfig.Perm.RequireRegion != 0) &&
			(!UI::PermUtility::HasUserPerm(mode_->FuncRegionConfig.Perm.RequireRegion)))	//用户设置了安全权限并且没有权限
		{
			if (mode_->FuncRegionConfig.Perm.IsPopwin)				//弹出提示窗口
			{
				Win()->OpenDialogPage(SafeTipsWinNum);
				return false;
			}
		}
		return true;
	}


	void RegionFunctionControl::StartRegionFunc()
	{
		if (mode_->FuncRegionConfig.Mode == Project::ActionMode::FuncTimer && HavePerm())
		{
			TimeCount++;
			if(mode_->FuncRegionConfig.TimeValueVarId != Project::DataVarId::NullId)
				UI::UIData::Number(mode_->FuncRegionConfig.TimeValueVarId, TimeCount * 100);
			if (TimeCount * 100 != mode_->FuncRegionConfig.Time)
				return;
			else
				TimeCount = 0;
		}
		switchwin_ = false;
		for (size_t i = 0; i < mode_->FuncRegionConfig.Functions.size(); i++)
		{
			if (switchwin_)		//切换画面
			{
				hastimer_ = false;
				//Page()->RemoveTimeout(RegionModeTimerCb, (void*)this);
				return;
			}
			if (!HavePerm())
			{
				hastimer_ = false;
				Page()->RemoveTimeout(RegionModeTimerCb, (void*)this);
				return;
			}
			if (mode_->FuncRegionConfig.Functions[i].FunctionName == "SetCoil")
				HandleSetCoil(mode_->FuncRegionConfig.Functions[i].FunctionParam);
			if (mode_->FuncRegionConfig.Functions[i].FunctionName == "SetData")
				HandleSetData(mode_->FuncRegionConfig.Functions[i].FunctionParam);
			if (mode_->FuncRegionConfig.Functions[i].FunctionName == "Arithmetic")
				HandleArithmetic(mode_->FuncRegionConfig.Functions[i].FunctionParam);
			if (mode_->FuncRegionConfig.Functions[i].FunctionName == "SendData")
				HandleSendData(mode_->FuncRegionConfig.Functions[i].FunctionParam);
			if (mode_->FuncRegionConfig.Functions[i].FunctionName == "SwitchScreen")
				HandleSwitchScreen(mode_->FuncRegionConfig.Functions[i].FunctionParam);
			if (mode_->FuncRegionConfig.Functions[i].FunctionName == "OpenWin")
				HandleOpenWin(mode_->FuncRegionConfig.Functions[i].FunctionParam);
			if (mode_->FuncRegionConfig.Functions[i].FunctionName == "CloseWin")
				HandleCloseWin(mode_->FuncRegionConfig.Functions[i].FunctionParam);
			if (mode_->FuncRegionConfig.Functions[i].FunctionName == "ImportCSVdata")
				HandleImportCSVdata(mode_->FuncRegionConfig.Functions[i]);
			if (mode_->FuncRegionConfig.Functions[i].FunctionName == "ExportCSVdata")
				HandleExportCSVdata(mode_->FuncRegionConfig.Functions[i]);
			if (mode_->FuncRegionConfig.Functions[i].FunctionName == "DownloadRecipe")
				HandleDownloadRecipe(mode_->FuncRegionConfig.Functions[i]);
			if (mode_->FuncRegionConfig.Functions[i].FunctionName == "UploadRecipe")
				HandleUploadRecipe(mode_->FuncRegionConfig.Functions[i]);
			if (mode_->FuncRegionConfig.Functions[i].FunctionName == "CallbackFunc")
				HandleCallbackFunc(mode_->FuncRegionConfig.Functions[i]);
			if (mode_->FuncRegionConfig.Functions[i].FunctionName == "PrintScreen")
				HandlePrintScreen(mode_->FuncRegionConfig.Functions[i].FunctionParam);
		}
		PermUtility::QuitLimitPerm(mode_->FuncRegionConfig.Perm);
	}


	void RegionFunctionControl::HandleDataVar(Project::DataVarId &varId)
	{
		RegionFunctionView* pView = static_cast<RegionFunctionView*>(pView_);
		if (mode_->FuncRegionConfig.VOffX.Cmp(varId))
		{
			mode_->FuncRegionConfig.OffX = UI::UIData::Number<int>(mode_->FuncRegionConfig.VOffX);
			UI::ViewShowUtility::ShowView(pView_, mode_->FuncRegionConfig.Perm, mode_->FuncRegionConfig.Y + mode_->FuncRegionConfig.OffY);
		}
		if (mode_->FuncRegionConfig.VOffY.Cmp(varId))
		{
			mode_->FuncRegionConfig.OffY = UI::UIData::Number<int>(mode_->FuncRegionConfig.VOffY);
			UI::ViewShowUtility::ShowView(pView_, mode_->FuncRegionConfig.Perm, mode_->FuncRegionConfig.Y + mode_->FuncRegionConfig.OffY);
		}
		if (mode_->FuncRegionConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->FuncRegionConfig.Perm, pView_);
		if (mode_->FuncRegionConfig.Perm.EnableVID.Cmp(varId))
		{
			UI::PermUtility::HandleEnablePerm(mode_->FuncRegionConfig.Perm, pView_);
			if (!pView->active())		//不使能
			{
				Page()->RemoveTimeout(RegionModeTimerCb, (void*)this);
				hastimer_ = false;
			}
			else
			{
				if (mode_->FuncRegionConfig.Mode == Project::ActionMode::FuncTimer && !stopfunc_ && !hastimer_)
				{
					Page()->RemoveTimeout(RegionModeTimerCb, (void*)this);
					hastimer_ = true;
					Page()->AddTimeout(100, RegionModeTimerCb, (void*)this, true);
				}
				if ((mode_->FuncRegionConfig.Mode == Project::ActionMode::FuncContinue && !stopfunc_ && !hastimer_)
					|| (mode_->FuncRegionConfig.Mode == Project::ActionMode::NumLogic && !stopfunc_ && !hastimer_ && meetcond_))		//定时模式
				{
					Page()->RemoveTimeout(RegionModeTimerCb, (void*)this);
					if (mode_->FuncRegionConfig.Time != 0)
					{
						hastimer_ = true;
						Page()->AddTimeout((size_t)(mode_->FuncRegionConfig.Time), RegionModeTimerCb, (void*)this, true);
					}
				}
			}
		}
		if (mode_->FuncRegionConfig.CoilVar.Cmp(varId))
		{
			if (mode_->FuncRegionConfig.Mode == Project::ActionMode::CoilTrigger)
			{
				bool coilView = UI::UIData::Bit(mode_->FuncRegionConfig.CoilVar);
				if ((coilView == true && precoilstatus_ == false && mode_->FuncRegionConfig.TriggerMode == 0) ||
					(coilView == false && precoilstatus_ == true && mode_->FuncRegionConfig.TriggerMode == 1))
					StartRegionFunc();
				precoilstatus_ = coilView;
			}
		}
		if (mode_->FuncRegionConfig.ValueVarId.Cmp(varId))
		{
			double value = UI::UIData::Number<DOUBLE>(mode_->FuncRegionConfig.ValueVarId);
			/*判断是否满足条件*/
			meetcond_ = false;
			XJDataType tp = UIDataService::Ins().GetDataType(mode_->FuncRegionConfig.ValueVarId);
			if(JudgeCondition::MeetCondition(mode_->FuncRegionConfig.NumlogicCond,
				value, mode_->FuncRegionConfig.Value, tp))
				meetcond_ = true;
			if (!meetcond_)			//没有满足条件，停止定时器
			{
				Page()->RemoveTimeout(RegionModeTimerCb, (void*)this);
				hastimer_ = false;
			}
			else
			{
				if (!stopfunc_ && !hastimer_ && mode_->FuncRegionConfig.Time != 0)
				{
					Page()->AddTimeout(500, RegionModeTimerCb, (void*)this, true);
					hastimer_ = true;
				}
			}
		}
		/*停止定时模式、连续模式和数值逻辑标志*/
		if (mode_->FuncRegionConfig.TimeLmtVar.Cmp(varId))
		{
			bool stopstatus = UI::UIData::Bit(mode_->FuncRegionConfig.TimeLmtVar);
			if ((mode_->FuncRegionConfig.StopMode == 1 && stopstatus == false) ||
				(mode_->FuncRegionConfig.StopMode == 0 && stopstatus == true))
			{
				if (mode_->FuncRegionConfig.Mode == Project::ActionMode::FuncContinue ||
					mode_->FuncRegionConfig.Mode == Project::ActionMode::FuncTimer ||
					mode_->FuncRegionConfig.Mode == Project::ActionMode::NumLogic)
				{
					Page()->RemoveTimeout(RegionModeTimerCb, (void*)this);		//停止执行
					hastimer_ = false;
					stopfunc_ = true;
				}
			}
			else
			{
				if (mode_->FuncRegionConfig.Mode == Project::ActionMode::FuncContinue ||
					mode_->FuncRegionConfig.Mode == Project::ActionMode::FuncTimer)
				{
					stopfunc_ = false;
					if (!hastimer_)
					{
						Page()->RemoveTimeout(RegionModeTimerCb, (void*)this);
						if (mode_->FuncRegionConfig.Mode == Project::ActionMode::FuncContinue)
						{
							Page()->AddTimeout(500, RegionModeTimerCb, (void*)this, true);		//停止执行
							hastimer_ = true;
						}
						if (mode_->FuncRegionConfig.Mode == Project::ActionMode::FuncTimer)
						{
							Page()->AddTimeout(100, RegionModeTimerCb, (void*)this, true);		//停止执行
							hastimer_ = true;
						}
					}
				}
				if (mode_->FuncRegionConfig.Mode == Project::ActionMode::NumLogic)
				{
					if (meetcond_)		//满足开始条件
					{
						stopfunc_ = false;
						if (!hastimer_)
						{
							Page()->RemoveTimeout(RegionModeTimerCb, (void*)this);
							Page()->AddTimeout(500, RegionModeTimerCb, (void*)this, true);		//停止执行
						}
					}
				}
			}
			
		}
		if (mode_->FuncRegionConfig.FirstDownLoadVar.Cmp(varId))
		{
			bool flag = UI::UIData::Bit(mode_->FuncRegionConfig.FirstDownLoadVar);
			if (flag && mode_->FuncRegionConfig.Mode == ActionMode::FirstDownload)
				StartRegionFunc();
			/*清除标志位*/
			LocalData::SetBit(SYS_PSB_ClearFlag, true);
		}
		if (mode_->FuncRegionConfig.PowerOnVar.Cmp(varId))
		{
			bool flag = UI::UIData::Bit(mode_->FuncRegionConfig.PowerOnVar);
			if (flag && mode_->FuncRegionConfig.Mode == ActionMode::FirstPowerON)
				StartRegionFunc();
			/*清除标志位*/
			LocalData::SetBit(SYS_PSB_ClearFlag, true);
		}

	}

	int RegionFunctionControl::PeekHMIMessage(Message::Msg* msg)
	{
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
