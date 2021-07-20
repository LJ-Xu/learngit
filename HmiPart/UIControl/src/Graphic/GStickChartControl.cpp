#include "stdafx.h"
#include "HMIWindow.h"
#include "GStickChartControl.h"
#include "GStickChartView.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "Message.h"
#include "UIData.h"
namespace UI
{

	//static shared_ptr<GStickChartModel> Mode;
	GStickChartControl::GStickChartControl(HMIPage* w) :BaseControl(w)
	{
		//创建GStickChartModel
		mode_ = std::shared_ptr < GStickChartModel>(new GStickChartModel());
		InitMVCModel(mode_);
	}

	void GStickChartControl::CreateView()
	{
		//初始化VIEW
		GStickChartView* view = new GStickChartView(mode_->StickChartConfig.X, mode_->StickChartConfig.Y,
			mode_->StickChartConfig.Width, mode_->StickChartConfig.Height);

		InitMVCView(view);

	}

	void GStickChartControl::OnReady()
	{
		HandleSysChange(Permission);
	}

	int GStickChartControl::PeekHMIMessage(Message::Msg* msg)
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

	void GStickChartControl::HandleDataVar(Project::DataVarId & varId)
	{
		GStickChartView* pView = static_cast<GStickChartView*>(pView_);

		//偏移和权限
		if (mode_->StickChartConfig.VOffX.Cmp(varId))
		{
			mode_->StickChartConfig.OffX = UI::UIData::Number<int>(mode_->StickChartConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->StickChartConfig.Perm, mode_->StickChartConfig.X + mode_->StickChartConfig.OffX, mode_->StickChartConfig.Y + mode_->StickChartConfig.OffY);
		}
		if (mode_->StickChartConfig.VOffY.Cmp(varId))
		{
			mode_->StickChartConfig.OffY = UI::UIData::Number<int>(mode_->StickChartConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->StickChartConfig.Perm, mode_->StickChartConfig.X + mode_->StickChartConfig.OffX, mode_->StickChartConfig.Y + mode_->StickChartConfig.OffY);
		}
		if (mode_->StickChartConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->StickChartConfig.Perm, pView_);
		if (mode_->StickChartConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->StickChartConfig.Perm, pView_);

		pView->HmiMainMode = true;

		pView->damage();
		pView->redraw();
	}

	bool GStickChartControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->StickChartConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->StickChartConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->StickChartConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->StickChartConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->StickChartConfig.Perm, mode_->StickChartConfig.X + mode_->StickChartConfig.OffX, mode_->StickChartConfig.Y + mode_->StickChartConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}

	//获取数据 
	/*void GStickChartControl::AddVarData(vector<Project::DataVarId*> vars)
	{
	}*/
}
