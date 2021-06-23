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
		//����GStickChartModel
		mode_ = std::shared_ptr < GStickChartModel>(new GStickChartModel());
		InitMVCModel(mode_);
	}

	void GStickChartControl::CreateView()
	{
		//��ʼ��VIEW
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

		//ƫ�ƺ�Ȩ��
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
		///��ֵ
		//��ǰ��ֵ
		pView->CurValue_ = UIData::Number<int>(mode_->StickChartConfig.BarVar);
		//��ǰ���ֵ
		pView->CurMaxValue_ = mode_->StickChartConfig.UseMaxAddr ? UIData::Number<int>(mode_->StickChartConfig.MaxData) : mode_->StickChartConfig.MaxValue;
		//��ǰ��Сֵ
		pView->CurMinValue_ = mode_->StickChartConfig.UseMinAddr ? UIData::Number<int>(mode_->StickChartConfig.MinData) : mode_->StickChartConfig.MinValue;
		//���Ʒ�Χ
		pView->CurValue_ = pView->CurValue_ > pView->CurMaxValue_ ? pView->CurMaxValue_ : pView->CurValue_;
		pView->CurValue_ = pView->CurValue_ < pView->CurMinValue_ ? pView->CurMinValue_ : pView->CurValue_;
		//Ŀ������
		if (mode_->StickChartConfig.UseDstField)
		{
			//��ǰĿ������
			pView->CurDstValue_ = mode_->StickChartConfig.UseDstValueByAddr ? UIDataService::Ins().GetNumber<int>(mode_->StickChartConfig.DstValueAddr) : mode_->StickChartConfig.DstValue;
			//��ǰ��Χ
			pView->CurRangeValue_ = mode_->StickChartConfig.UseDstRangeByAddr ? UIDataService::Ins().GetNumber<int>(mode_->StickChartConfig.DstValueAddr) : mode_->StickChartConfig.DstRange;
		}
		//��Χ����
		if (mode_->StickChartConfig.UseRangeWarn)
		{
			//��ǰ�Ͼ���ֵ
			pView->CurUpperValue_ = mode_->StickChartConfig.UseUpperWarnAddr ? UIDataService::Ins().GetNumber<int>(mode_->StickChartConfig.UpperWarnAddr) : mode_->StickChartConfig.UpperWarnValue;
			//��ǰ�¾���ֵ
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
			/*�ж��Ƿ���Ȩ��*/
			if (!UI::PermUtility::HasUserPerm(mode_->StickChartConfig.Perm.RequireRegion))	//û��Ȩ�޲�������
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

	//��ȡ���� 
	/*void GStickChartControl::AddVarData(vector<Project::DataVarId*> vars)
	{
	}*/
}
