/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DropMenuControl.cpp
 * Author   : qiaodan
 * Date     : 2020-12-03
 * Descript : 下拉菜单的控制源码，创建下拉菜单绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "Message.h"
#include "HMIWindow.h"
#include "DropMenuControl.h"
#include "IDataService.h"
#include "DropMenuItem.h"
#include "NoticesUtility.h"
#include "UIData.h"
#include "PermUtility.h"
#include "ViewShowUtility.h"

namespace UI
{
	DropMenuControl::DropMenuControl(HMIPage* w) : BaseControl(w)
	{
		mode_ = std::shared_ptr<DropMenuModel>(new DropMenuModel());
		InitMVCModel(mode_);
	}
	void DropMenuControl::CreateView()
	{
		//初始化VIEW
		DropMenuView *view = new DropMenuView(mode_->MenuConfig.X + mode_->MenuConfig.OffX,
										      mode_->MenuConfig.Y + mode_->MenuConfig.OffY, 
											  mode_->MenuConfig.Width, mode_->MenuConfig.Height, "");
		InitMVCView(view);
		view->InitDraw();
	}

	void DropMenuControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->MenuConfig.VOffX != Project::DataVarId::NullId)
			mode_->MenuConfig.OffX = UI::UIData::Number<int>(mode_->MenuConfig.VOffX) + pageoffx;
		else
			mode_->MenuConfig.OffX = pageoffx;


		if (mode_->MenuConfig.VOffY != Project::DataVarId::NullId)
			mode_->MenuConfig.OffY = UI::UIData::Number<int>(mode_->MenuConfig.VOffY) + pageoffy;
		else
			mode_->MenuConfig.OffY = pageoffy;
	}
	void DropMenuControl::OnReady()
	{
		HandleSysChange(Permission);
	}

	void DropMenuControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->MenuConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->MenuConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->MenuConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->MenuConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->MenuConfig.Perm,
					mode_->MenuConfig.X + mode_->MenuConfig.OffX,
					mode_->MenuConfig.Y + mode_->MenuConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}

	void DropMenuControl::HandleDataVar(Project::DataVarId &varId)
	{
		DropMenuView *pView = static_cast<DropMenuView *>(pView_);
		if (mode_->MenuConfig.VOffX.Cmp(varId))
		{
			mode_->MenuConfig.OffX = UI::UIData::Number<int>(mode_->MenuConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->MenuConfig.Perm, mode_->MenuConfig.X + mode_->MenuConfig.OffX, mode_->MenuConfig.Y + mode_->MenuConfig.OffY);
		}
		if (mode_->MenuConfig.VOffY.Cmp(varId))
		{
			mode_->MenuConfig.OffY = UI::UIData::Number<int>(mode_->MenuConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->MenuConfig.Perm, mode_->MenuConfig.X + mode_->MenuConfig.OffX, mode_->MenuConfig.Y + mode_->MenuConfig.OffY);
		}
		if (mode_->MenuConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->MenuConfig.Perm, pView_);
		if (mode_->MenuConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->MenuConfig.Perm, pView_);
		if (mode_->MenuConfig.ReadVar.Cmp(varId)) 
		{
			if (mode_->MenuConfig.ContentMode && mode_->MenuConfig.HMICommandItem.Cmd == "GetUserList")		//命令模式
			{
				LocalData::SetString(SYS_PSW_LOGINName, pView->label());		//设置登陆用户名
				return;
			}
			int index;
			DOUBLE value = UI::UIData::Number<DOUBLE>(mode_->MenuConfig.ReadVar);
			bool hasvalue = false;
			for (unsigned i = 0; i < mode_->MenuConfig.MenuInfo.size(); i++)
			{
				if (mode_->MenuConfig.MenuInfo[i].Value == value)	//通过数值寻找索引
				{
					hasvalue = true;
					index = i;
				}
			}
			if (hasvalue == false)
			{
				pView->SelectNum = mode_->MenuConfig.MenuNum;
				//pView->label("");
				if (mode_->MenuConfig.Perm.HasShowPerm && mode_->MenuConfig.Perm.HasLimitShowPerm)
					pView->redraw();
				//错误通知
				NoticesUtility::NoticeWrite(mode_->MenuConfig.ErrorNotice);
				//if (mode_->MenuConfig.NotifyBitVar != Project::DataVarId::NullId)
				//{
				//	//通知位
				//	switch (mode_->MenuConfig.BitMode)
				//	{
				//	case Project::SetON:
				//		UIDataService::Ins().SetBit(mode_->MenuConfig.NotifyBitVar, true);
				//		break;
				//	case Project::SetOFF:
				//		UIDataService::Ins().SetBit(mode_->MenuConfig.NotifyBitVar, false);
				//		break;
				//	case Project::Rising:
				//		UIDataService::Ins().SetBit(mode_->MenuConfig.NotifyBitVar, false);
				//		UIDataService::Ins().SetBit(mode_->MenuConfig.NotifyBitVar, true);
				//		break;
				//	case Project::Falling:
				//		UIDataService::Ins().SetBit(mode_->MenuConfig.NotifyBitVar, true);
				//		UIDataService::Ins().SetBit(mode_->MenuConfig.NotifyBitVar, false);
				//		break;
				//	default:
				//		break;
				//	}
				//}
				//if (mode_->MenuConfig.NotifyByteVar != Project::DataVarId::NullId)		//通知字
				//	UIDataService::Ins().SetNumber<int>(mode_->MenuConfig.NotifyByteVar,
				//		mode_->MenuConfig.NotifyValue);
			}
			else
			{
				//if (pView->CurrentIndex != index)
				//{
					DropMenuItem* menu = pView->menu_->next(index);		//通过索引寻找内容
					//pView->align(menu->labelalign());
					//pView->labelcolor(menu->labelcolor());
					//pView->labelfont(menu->labelfont());
					//pView->labelsize(menu->labelsize());
					//pView->label(menu->text.c_str());
					
					pView->SelectNum = index;
					if (menu->MenuState != nullptr)
					{
						menu->MenuState->state = 2;
						//menu->MenuState->p[0]->set_selected(index);
						//menu->MenuState->p[0]->hscrollbar->value(
						//	menu->MenuState->p[0]->Startnum * menu->MenuState->p[0]->RealH /
						//	(menu->MenuState->p[0]->numitems - (double)menu->MenuState->p[0]->RealH / (double)menu->MenuState->p[0]->itemheight));
						//menu->MenuState->p[0]->redraw();
					}
					if (mode_->MenuConfig.Perm.HasShowPerm && mode_->MenuConfig.Perm.HasLimitShowPerm)
						pView->redraw();
			}
			//char *text = new char[32];
			//memset(text, '\0', 32);
			//memcpy(text,pView->label(), 32);


		}
	}
	int DropMenuControl::PeekHMIMessage(Message::Msg* msg)
	{
		DropMenuView *pView = static_cast<DropMenuView *>(pView_);
		Project::DataVarId varId;
		switch (msg->GetCode())
		{
		case WM_EVENT_DATEUPDATE:
			varId = Project::DataVarId(msg->LParam);
			HandleDataVar(varId);
			break;
		case WM_EVENT_CLICKED:
			pView->SetWriteData();
			break;
		default:
			break;
		}
		return 1;//handled;
	}
}
