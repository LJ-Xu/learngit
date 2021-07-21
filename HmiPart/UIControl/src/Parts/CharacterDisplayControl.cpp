/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : CharacterDisplayControl.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 创建字符显示框，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "CharacterDisplayControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
namespace UI
{
	CharacterDisplayControl::CharacterDisplayControl(HMIPage* w) : BaseControl(w)
	{
		//创建CharacterInputModel
		mode_ = std::shared_ptr <CharacterDisplayModel>(new CharacterDisplayModel());
		InitMVCModel(mode_);
	}

	void CharacterDisplayControl::CreateView()
	{
		//初始化VIEW
		CharacterDisplayView* view = new CharacterDisplayView(
								mode_->DisStrConfig.X + mode_->DisStrConfig.OffX,
								mode_->DisStrConfig.Y + mode_->DisStrConfig.OffY,
								mode_->DisStrConfig.Width, 
								mode_->DisStrConfig.Height, nullptr);
		if (IResourceService::Ins()->IsRenderMode())
			view->DisplayValue = mode_->DisStrConfig.FmtStr;
		else
			view->DisplayValue = "";
		InitMVCView(view);
	}
	void CharacterDisplayControl::OnReady()
	{
		HandleSysChange(Permission);
	}

	bool CharacterDisplayControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->DisStrConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->DisStrConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->DisStrConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->DisStrConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->DisStrConfig.Perm,
					mode_->DisStrConfig.X + mode_->DisStrConfig.OffX,
					mode_->DisStrConfig.Y + mode_->DisStrConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}


	void CharacterDisplayControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->DisStrConfig.VOffX != Project::DataVarId::NullId)
			mode_->DisStrConfig.OffX = UI::UIData::Number<int>(mode_->DisStrConfig.VOffX) + pageoffx;
		else
			mode_->DisStrConfig.OffX = pageoffx;


		if (mode_->DisStrConfig.VOffY != Project::DataVarId::NullId)
			mode_->DisStrConfig.OffY = UI::UIData::Number<int>(mode_->DisStrConfig.VOffY) + pageoffy;
		else
			mode_->DisStrConfig.OffY = pageoffy;
	}
	void CharacterDisplayControl::HandleDataVar(Project::DataVarId &varId)
	{
		CharacterDisplayView* pView = static_cast<CharacterDisplayView*>(pView_);
		if (mode_->DisStrConfig.VOffX.Cmp(varId))
		{
			mode_->DisStrConfig.OffX = UI::UIData::Number<int>(mode_->DisStrConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->DisStrConfig.Perm, mode_->DisStrConfig.X + mode_->DisStrConfig.OffX, mode_->DisStrConfig.Y + mode_->DisStrConfig.OffY);
		}
		if (mode_->DisStrConfig.VOffY.Cmp(varId))
		{
			mode_->DisStrConfig.OffY = UI::UIData::Number<int>(mode_->DisStrConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->DisStrConfig.Perm, mode_->DisStrConfig.X + mode_->DisStrConfig.OffX, mode_->DisStrConfig.Y + mode_->DisStrConfig.OffY);
		}
		if (mode_->DisStrConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->DisStrConfig.Perm, pView_);
		if (mode_->DisStrConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->DisStrConfig.Perm, pView_);
		if (mode_->DisStrConfig.ReadVar.Cmp(varId))
		{
			/*读取字符串*/
			int count = UIDataService::Ins().GetDataCounts(mode_->DisStrConfig.ReadVar);
			int size = count > 256 ? count : 256;
			char *buf = new char[size + 2];
			memset(buf, '\0', size + 2);
			UIDataService::Ins().GetString(mode_->DisStrConfig.ReadVar, buf, count);
			/*字符串转成utf8*/
			CodeFormatUtility::CodeToUtf8(mode_->DisStrConfig.Rule, buf, size);
			/*填补空格*/
			if (mode_->DisStrConfig.Rule == CodeFormatUtility::ASCII ||
				mode_->DisStrConfig.Rule == CodeFormatUtility::UTF_8)
			{
				unsigned short *utf16 = new unsigned short[count / 2 + 1];
				int z = sizeof(short);
				memset(utf16, 0, (count / 2 + 1) * sizeof(short));
				memcpy((void *)utf16, buf, count);
				int size = 0;
				for (size_t i = 0; i < (size_t)(count / 2 + 1); i++)
				{
					if (utf16[i] == 0)
					{
						size = i * 2;
						break;
					}
				}
				memset(buf, '\0', count + 1);
				memcpy(buf, utf16, size);
				delete[] utf16;
				for (size_t j = 0; j < (size_t)size; j++)
				{
					if (buf[j] == '\0' && j != size - 1)
						buf[j] = ' ';
				}
			}
			pView->DisplayValue = buf;
			delete[] buf;
			if (mode_->DisStrConfig.Perm.HasShowPerm && mode_->DisStrConfig.Perm.HasLimitShowPerm)
				pView->redraw();			//int count = UIDataService::Ins().GetDataCounts(mode_->DisStrConfig.ReadVar);
			//char *buf = new char[count + 1];
			//memset(buf, '\0', count + 1);

			//UIDataService::Ins().GetString(mode_->DisStrConfig.ReadVar, buf, count);
			//pView->DisplayValue = buf;
			//delete[] buf;
			//pView->redraw();
		
		}
			
	}

	int CharacterDisplayControl::PeekHMIMessage(Message::Msg* msg)
	{		
		CharacterDisplayView* pView = static_cast<CharacterDisplayView*>(pView_);

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
