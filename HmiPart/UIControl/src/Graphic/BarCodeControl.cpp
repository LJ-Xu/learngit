#include "stdafx.h"
#include "HMIWindow.h"
#include "BarCodeControl.h"
#include "BarCodeView.h"
#include "Message.h"
#include "SysCtrlApi.h"
#include "HMIPage.h"
#include "UIData.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"

namespace UI
{
	BarCodeControl::BarCodeControl(HMIPage* w) :BaseControl(w)
	{
		//创建BarCodeModel
		mode_ = std::shared_ptr < BarCodeModel>(new BarCodeModel());
		InitMVCModel(mode_);
	}

	void BarCodeControl::CreateView()
	{
		//初始化VIEW
		BarCodeView* view = new BarCodeView(mode_->BarcodeConfig.X, mode_->BarcodeConfig.Y,
			mode_->BarcodeConfig.Width, mode_->BarcodeConfig.Height);
		InitMVCView(view);
	}

	bool BarCodeControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->BarcodeConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->BarcodeConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->BarcodeConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->BarcodeConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->BarcodeConfig.Perm, mode_->BarcodeConfig.X + mode_->BarcodeConfig.OffX, mode_->BarcodeConfig.Y + mode_->BarcodeConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}

	void BarCodeControl::OnReady()
	{
		HandleSysChange(Permission);
	}

	int BarCodeControl::PeekHMIMessage(Message::Msg* msg)
	{
		Project::DataVarId varId;
		switch (msg->Code)
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

	void BarCodeControl::HandleDataVar(Project::DataVarId & varId)
	{
		BarCodeModel* pMode = static_cast<BarCodeModel*>(pModel_.get());
		BarCodeView* pView = static_cast<BarCodeView*>(pView_);
		//偏移和权限
		if (mode_->BarcodeConfig.VOffX.Cmp(varId))
		{
			mode_->BarcodeConfig.OffX = UI::UIData::Number<int>(mode_->BarcodeConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->BarcodeConfig.Perm, mode_->BarcodeConfig.X + mode_->BarcodeConfig.OffX, mode_->BarcodeConfig.Y + mode_->BarcodeConfig.OffY);
		}
		if (mode_->BarcodeConfig.VOffY.Cmp(varId))
		{
			mode_->BarcodeConfig.OffY = UI::UIData::Number<int>(mode_->BarcodeConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->BarcodeConfig.Perm, mode_->BarcodeConfig.X + mode_->BarcodeConfig.OffX, mode_->BarcodeConfig.Y + mode_->BarcodeConfig.OffY);
		}
		if (mode_->BarcodeConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->BarcodeConfig.Perm, pView_);
		if (mode_->BarcodeConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->BarcodeConfig.Perm, pView_);
		if (!mode_->BarcodeConfig.UseConstValue)
		{
			UIDataService::Ins().GetString(mode_->BarcodeConfig.ReadVar, pView->BuffValue, sizeof(pView->BuffValue));
			/*int value = UIData::Number<int>(mode_->BarcodeConfig.ReadVar);
			memset(pView->BuffValue, 0, QRBUFFLEN);
			sprintf(pView->BuffValue, "%d", value);*/
			//memcpy(pView->BuffValue, value, sizeof(int));
		}
		
		pView->damage();
		pView->redraw();
	}

}


