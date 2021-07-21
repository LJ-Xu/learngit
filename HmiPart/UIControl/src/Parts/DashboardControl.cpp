#include "stdafx.h"
#include "HMIWindow.h"
#include "DashboardControl.h"
#include "DashboardView.h"
#include "ViewShowUtility.h"
#include "Message.h"
#include "UIData.h"
#include "PermUtility.h"
namespace UI
{
	DashboardControl::DashboardControl(HMIPage* w) :BaseControl(w)
	{
		//创建GStickChartModel
		mode_ = std::shared_ptr < DashboardModel>(new DashboardModel());
		InitMVCModel(mode_);
	}

	void DashboardControl::CreateView()
	{
		//初始化VIEW
		DashboardView* view = new DashboardView(mode_->DashboardConfig.X, mode_->DashboardConfig.Y,
			mode_->DashboardConfig.Width, mode_->DashboardConfig.Height);

		InitMVCView(view);
		
		//初始化绘制角度
		//view->InitArcLimit(mode_.get());
	}

	void DashboardControl::OnReady()
	{
		HandleSysChange(Permission);
	}

	bool DashboardControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->DashboardConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->DashboardConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->DashboardConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->DashboardConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->DashboardConfig.Perm, mode_->DashboardConfig.X + mode_->DashboardConfig.OffX, mode_->DashboardConfig.Y + mode_->DashboardConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}

	int DashboardControl::PeekHMIMessage(Message::Msg* msg)
	{
		Project::DataVarId varId = msg->LParam;
		switch (msg->Code)
		{
		case WM_EVENT_DATEUPDATE:
			HandleDataVar(varId);
			break;
		default:
			break;
		}
		return 1;//handled;
	}
	void DashboardControl::HandleDataVar(Project::DataVarId & varId)
	{
		DashboardModel* pMode = static_cast<DashboardModel*>(pModel_.get());
		DashboardView* pView = static_cast<DashboardView*>(pView_);
		if (mode_->DashboardConfig.VOffX.Cmp(varId))
		{
			mode_->DashboardConfig.OffX = UI::UIData::Number<int>(mode_->DashboardConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->DashboardConfig.Perm,
				mode_->DashboardConfig.X + mode_->DashboardConfig.OffX,
				mode_->DashboardConfig.Y + mode_->DashboardConfig.OffY);
		}
		if (mode_->DashboardConfig.VOffY.Cmp(varId))
		{
			mode_->DashboardConfig.OffY = UI::UIData::Number<int>(mode_->DashboardConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->DashboardConfig.Perm,
				mode_->DashboardConfig.X + mode_->DashboardConfig.OffX,
				mode_->DashboardConfig.Y + mode_->DashboardConfig.OffY);
		}
		if (mode_->DashboardConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->DashboardConfig.Perm, pView_);
		pView->HmiMainMode = true;
		pView->damage();
		pView->redraw();
	}
}
