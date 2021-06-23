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
		///算值
		//当前数值
		pView->CurValue_ = UIData::Number<int>(mode_->StickChartConfig.BarVar);
		//当前最大值
		pView->CurMaxValue_ = mode_->StickChartConfig.UseMaxAddr ? UIData::Number<int>(mode_->StickChartConfig.MaxData) : mode_->StickChartConfig.MaxValue;
		//当前最小值
		pView->CurMinValue_ = mode_->StickChartConfig.UseMinAddr ? UIData::Number<int>(mode_->StickChartConfig.MinData) : mode_->StickChartConfig.MinValue;
		//限制范围
		pView->CurValue_ = pView->CurValue_ > pView->CurMaxValue_ ? pView->CurMaxValue_ : pView->CurValue_;
		pView->CurValue_ = pView->CurValue_ < pView->CurMinValue_ ? pView->CurMinValue_ : pView->CurValue_;
		//目标区间
		if (mode_->StickChartConfig.UseDstField)
		{
			//当前目标区间
			pView->CurDstValue_ = mode_->StickChartConfig.UseDstValueByAddr ? UIDataService::Ins().GetNumber<int>(mode_->StickChartConfig.DstValueAddr) : mode_->StickChartConfig.DstValue;
			//当前误差范围
			pView->CurRangeValue_ = mode_->StickChartConfig.UseDstRangeByAddr ? UIDataService::Ins().GetNumber<int>(mode_->StickChartConfig.DstValueAddr) : mode_->StickChartConfig.DstRange;
		}
		//范围报警
		if (mode_->StickChartConfig.UseRangeWarn)
		{
			//当前上警报值
			pView->CurUpperValue_ = mode_->StickChartConfig.UseUpperWarnAddr ? UIDataService::Ins().GetNumber<int>(mode_->StickChartConfig.UpperWarnAddr) : mode_->StickChartConfig.UpperWarnValue;
			//当前下警报值
			pView->CurLowerValue_ = mode_->StickChartConfig.UseLowerWarnAddr ? UIDataService::Ins().GetNumber<int>(mode_->StickChartConfig.LowerWarnAddr) : mode_->StickChartConfig.LowerWarnValue;

		}

		pView->damage();
		pView->redraw();
	}

	void GStickChartControl::HandleSysChange(SysChangeEM catogray)
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
	}

	//获取数据 
	/*void GStickChartControl::AddVarData(vector<Project::DataVarId*> vars)
	{
	}*/
}
