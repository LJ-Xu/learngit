#include "stdafx.h"
#include "HMIWindow.h"
#include "ScaleControl.h"
#include "ScaleView.h"
#include "ScaleConfig.h"
#include "Message.h"
#include "HMIPage.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"

namespace UI
{
	ScaleControl::ScaleControl(HMIPage* w) :BaseControl(w)
	{
		//创建ScaleControl
		mode_ = std::shared_ptr < ScaleModel>(new ScaleModel());
		InitMVCModel(mode_);
	}
	void ScaleControl::CreateView()
	{
		//初始化VIEW
		ScaleView* view = new ScaleView(mode_->ScaleConfig.X, mode_->ScaleConfig.Y,
			mode_->ScaleConfig.Width, mode_->ScaleConfig.Height);

		InitMVCView(view);

	}

	bool ScaleControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->ScaleConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->ScaleConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->ScaleConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->ScaleConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->ScaleConfig.Perm, mode_->ScaleConfig.X + mode_->ScaleConfig.OffX, mode_->ScaleConfig.Y + mode_->ScaleConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}

	void ScaleControl::OnReady()
	{
		HandleSysChange(Permission);
	}
	int ScaleControl::PeekHMIMessage(Message::Msg* msg)
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
	void ScaleControl::HandleDataVar(Project::DataVarId & varId)
	{
		ScaleView* pView = static_cast<ScaleView*>(pView_);

		//偏移和权限
		if (mode_->ScaleConfig.VOffX.Cmp(varId))
		{
			mode_->ScaleConfig.OffX = UI::UIData::Number<int>(mode_->ScaleConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->ScaleConfig.Perm, mode_->ScaleConfig.X + mode_->ScaleConfig.OffX, mode_->ScaleConfig.Y + mode_->ScaleConfig.OffY);
		}
		if (mode_->ScaleConfig.VOffY.Cmp(varId))
		{
			mode_->ScaleConfig.OffY = UI::UIData::Number<int>(mode_->ScaleConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->ScaleConfig.Perm, mode_->ScaleConfig.X + mode_->ScaleConfig.OffX, mode_->ScaleConfig.Y + mode_->ScaleConfig.OffY);
		}
		if (mode_->ScaleConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->ScaleConfig.Perm, pView_);

		if (mode_->ScaleConfig.IsMarkMaxValueUseRegister)
		{
			pView->CurMaxValue = UIData::Number<int>(mode_->ScaleConfig.MarkMaxValueVarIdRef);
		}
		else
		{
			pView->CurMaxValue = mode_->ScaleConfig.MarkMaxValue;
		}
		if (mode_->ScaleConfig.IsMarkMinValueUseRegister)
		{
			pView->CurMinValue = UIData::Number<int>(mode_->ScaleConfig.MarkMinValueVarIdRef);
		}
		else
		{
			pView->CurMinValue = mode_->ScaleConfig.MarkMinValue;
		}
		pView_->damage();
		pView_->redraw();
	}
}

