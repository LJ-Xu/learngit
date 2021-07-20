/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ChineseDisplayControl.cpp
 * Author   : qiaodan
 * Date     : 2020-10-15
 * Descript : 创建中文显示框，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "ChineseDisplayControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
#include "CodeFormatUtility.h"
namespace UI
{
	ChineseDisplayControl::ChineseDisplayControl(HMIPage* w) : BaseControl(w)
	{
		//创建CharacterInputModel
		mode_ = std::shared_ptr <ChineseDisplayModel>(new ChineseDisplayModel());
		InitMVCModel(mode_);
	}

	void ChineseDisplayControl::CreateView()
	{
		//初始化VIEW 
		ChineseDisplayView* view = new ChineseDisplayView(
			mode_->DisChineseConfig.X + mode_->DisChineseConfig.OffX,
			mode_->DisChineseConfig.Y + mode_->DisChineseConfig.OffY,
			mode_->DisChineseConfig.Width,
			mode_->DisChineseConfig.Height, nullptr);
		if (IResourceService::Ins()->IsRenderMode())
			view->DisplayValue = mode_->DisChineseConfig.Text;
		else
			view->DisplayValue = "";
		UI::IResourceService::GB2312toUtf8(view->DisplayValue);
		InitMVCView(view);
	}
	void ChineseDisplayControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->DisChineseConfig.VOffX != Project::DataVarId::NullId)
			mode_->DisChineseConfig.OffX = UI::UIData::Number<int>(mode_->DisChineseConfig.VOffX) + pageoffx;
		else
			mode_->DisChineseConfig.OffX = pageoffx;


		if (mode_->DisChineseConfig.VOffY != Project::DataVarId::NullId)
			mode_->DisChineseConfig.OffY = UI::UIData::Number<int>(mode_->DisChineseConfig.VOffY) + pageoffy;
		else
			mode_->DisChineseConfig.OffY = pageoffy;
	}
	void ChineseDisplayControl::OnReady()
	{
		HandleSysChange(Permission);
	}

	bool ChineseDisplayControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->DisChineseConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->DisChineseConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->DisChineseConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->DisChineseConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->DisChineseConfig.Perm, mode_->DisChineseConfig.X + mode_->DisChineseConfig.OffX, mode_->DisChineseConfig.Y + mode_->DisChineseConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}
	
	void ChineseDisplayControl::HandleDataVar(Project::DataVarId &varId)
	{
		ChineseDisplayView* pView = static_cast<ChineseDisplayView*>(pView_);
		if (mode_->DisChineseConfig.VOffX.Cmp(varId))
		{
			mode_->DisChineseConfig.OffX = UI::UIData::Number<int>(mode_->DisChineseConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->DisChineseConfig.Perm, mode_->DisChineseConfig.X + mode_->DisChineseConfig.OffX, mode_->DisChineseConfig.Y + mode_->DisChineseConfig.OffY);
		}
		if (mode_->DisChineseConfig.VOffY.Cmp(varId))
		{
			mode_->DisChineseConfig.OffY = UI::UIData::Number<int>(mode_->DisChineseConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->DisChineseConfig.Perm, mode_->DisChineseConfig.X + mode_->DisChineseConfig.OffX, mode_->DisChineseConfig.Y + mode_->DisChineseConfig.OffY);
		}
		if (mode_->DisChineseConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->DisChineseConfig.Perm, pView_);
		if (mode_->DisChineseConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->DisChineseConfig.Perm, pView_);
		if (mode_->DisChineseConfig.ReadVar.Cmp(varId))
		{
			int count = UIDataService::Ins().GetDataCounts(mode_->DisChineseConfig.ReadVar);
			char *buf = new char[count + 1];
			memset(buf, '\0', count + 1);
			UIDataService::Ins().GetString(mode_->DisChineseConfig.ReadVar, buf, count);
			pView->DisplayValue = buf;
			if(!UI::CodeFormatUtility::IsStrUtf8(pView->DisplayValue.c_str()))
				UI::IResourceService::GB2312toUtf8(pView->DisplayValue);
			UI::ViewShowUtility::ShowView(pView, mode_->DisChineseConfig.Perm,
				mode_->DisChineseConfig.X + mode_->DisChineseConfig.OffX,
				mode_->DisChineseConfig.Y + mode_->DisChineseConfig.OffY);
			delete[] buf;
		}

	}

	int ChineseDisplayControl::PeekHMIMessage(Message::Msg* msg)
	{
		ChineseDisplayView* pView = static_cast<ChineseDisplayView*>(pView_);
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
