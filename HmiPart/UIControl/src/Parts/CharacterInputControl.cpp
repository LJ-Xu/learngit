/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : CharacterInputControl.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 创建字符输入框，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "CharacterInputControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
namespace UI
{
	CharacterInputControl::CharacterInputControl(HMIPage* w) : BaseControl(w)
	{
		//创建CharacterInputModel
		mode_ = std::shared_ptr < CharacterInputModel>(new CharacterInputModel());
		InitMVCModel(mode_);
	}
	void CharacterInputControl::OnReady()
	{
		HandleSysChange(Permission);
	}

	void CharacterInputControl::CreateView()
	{
		//初始化VIEW
		CharacterInputView*	view = new CharacterInputView(
			mode_->InputStrConfig.X + mode_->InputStrConfig.OffX,
			mode_->InputStrConfig.Y + mode_->InputStrConfig.OffY,
			mode_->InputStrConfig.Width,
			mode_->InputStrConfig.Height, nullptr);
		if (IResourceService::Ins()->IsRenderMode())
			view->DisplayValue = mode_->InputStrConfig.FmtStr;
		else
			view->DisplayValue = "";

		InitMVCView(view);
	}
	void CharacterInputControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->InputStrConfig.VOffX != Project::DataVarId::NullId)
			mode_->InputStrConfig.OffX = UI::UIData::Number<int>(mode_->InputStrConfig.VOffX) + pageoffx;
		else
			mode_->InputStrConfig.OffX = pageoffx;


		if (mode_->InputStrConfig.VOffY != Project::DataVarId::NullId)
			mode_->InputStrConfig.OffY = UI::UIData::Number<int>(mode_->InputStrConfig.VOffY) + pageoffy;
		else
			mode_->InputStrConfig.OffY = pageoffy;
	}
	void CharacterInputControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->InputStrConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->InputStrConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->InputStrConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->InputStrConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->InputStrConfig.Perm,
					mode_->InputStrConfig.X + mode_->InputStrConfig.OffX,
					mode_->InputStrConfig.Y + mode_->InputStrConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}
	void CharacterInputControl::HandleDataVar(Project::DataVarId &varId)
	{
		CharacterInputView* pView = static_cast<CharacterInputView*>(pView_);
		if (mode_->InputStrConfig.VOffX.Cmp(varId))
		{
			mode_->InputStrConfig.OffX = UIDataService::Ins().GetNumber<int>(mode_->InputStrConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->InputStrConfig.Perm,
				mode_->InputStrConfig.X + mode_->InputStrConfig.OffX, mode_->InputStrConfig.Y + mode_->InputStrConfig.OffY);
		}
		if (mode_->InputStrConfig.VOffY.Cmp(varId))
		{
			mode_->InputStrConfig.OffY = UIDataService::Ins().GetNumber<int>(mode_->InputStrConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->InputStrConfig.Perm,
				mode_->InputStrConfig.X + mode_->InputStrConfig.OffX, mode_->InputStrConfig.Y + mode_->InputStrConfig.OffY);
			pView_->show();
		}
		if (mode_->InputStrConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->InputStrConfig.Perm, pView_);
		if (mode_->InputStrConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->InputStrConfig.Perm, pView_);
		if (mode_->InputStrConfig.WriteVar.Cmp(varId))
		{
			/*读取字符串*/
			int count = UIDataService::Ins().GetDataCounts(mode_->InputStrConfig.WriteVar);
			int size = count > 256 ? count : 256;
			char *buf = new char[size + 1];
			memset(buf,'\0', size + 1);
			UIDataService::Ins().GetString(mode_->InputStrConfig.WriteVar, buf, count);
			/*字符串转成utf8*/
			CodeFormatUtility::CodeToUtf8(mode_->InputStrConfig.Rule, buf, size);
			/*填补空格*/
			if (mode_->InputStrConfig.Rule == CodeFormatUtility::ASCII ||
				mode_->InputStrConfig.Rule == CodeFormatUtility::UTF_8)
			{
				unsigned short *utf16 = new unsigned short[count / 2 + 1];
				int z = sizeof(short);
				memset(utf16, 0,(count / 2 + 1) * sizeof(short));
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
				memset(buf, '\0', count);
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
			if (mode_->InputStrConfig.Perm.HasShowPerm && mode_->InputStrConfig.Perm.HasLimitShowPerm)
				pView->redraw();
		}
		if (mode_->InputStrConfig.KeyMethod.KeypadVar.Cmp(varId))
		{
			if (UI::UIData::Bit(mode_->InputStrConfig.KeyMethod.KeypadVar))
			{
				map<int, vector<WinCtrlID>>::iterator  iter;
				iter = mode_->InputStrConfig.SequenceGroup.find(mode_->InputStrConfig.Sequence.GroupId);
				if (iter != mode_->InputStrConfig.SequenceGroup.end())
				{
					if (!iter->second.empty())
					{
						bool needfocus = true;
						for (size_t i = 0; i < iter->second.size(); i++)
						{
							std::shared_ptr<BaseControl> nextctrl = Page()->FindCtrl(GetCtrlNo(iter->second[i]));
							InputView *nextview = (InputView *)nextctrl->GetView();
							if (nextview->NeedHandleFocus || nextview->OpenKeypage)
							{
								needfocus = false;
								break;
							}
						}
						if (needfocus)
						{
							pView->NeedHandleFocus = true;
							pView->take_focus();
						}
					}
				}
			}
		}

	}

	int CharacterInputControl::PeekHMIMessage(Message::Msg* msg)
	{
		CharacterInputView* pView = static_cast<CharacterInputView*>(pView_);
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
