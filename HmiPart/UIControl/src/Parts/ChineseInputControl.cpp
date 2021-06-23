/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ChineseInputControl.cpp
 * Author   : qiaodan
 * Date     : 2020-10-19
 * Descript : 创建中文输入框，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "ChineseInputControl.h"
#include "ChineseInputView.h"
#include "HMIWindow.h"
#include "Message.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
#include "CodeFormatUtility.h"
namespace UI
{
	ChineseInputControl::ChineseInputControl(HMIPage* w) : BaseControl(w)
	{
		//创建CharacterInputModel
		mode_ = std::shared_ptr <ChineseInputModel>(new ChineseInputModel());
		InitMVCModel(mode_);
	}
	void ChineseInputControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->InputChineseConfig.VOffX != Project::DataVarId::NullId)
			mode_->InputChineseConfig.OffX = UI::UIData::Number<int>(mode_->InputChineseConfig.VOffX) + pageoffx;
		else
			mode_->InputChineseConfig.OffX = pageoffx;


		if (mode_->InputChineseConfig.VOffY != Project::DataVarId::NullId)
			mode_->InputChineseConfig.OffY = UI::UIData::Number<int>(mode_->InputChineseConfig.VOffY) + pageoffy;
		else
			mode_->InputChineseConfig.OffY = pageoffy;
	}
	void ChineseInputControl::CreateView()
	{
		//初始化VIEW
		ChineseInputView*	view= new ChineseInputView(
			mode_->InputChineseConfig.X + mode_->InputChineseConfig.OffX,
			mode_->InputChineseConfig.Y + mode_->InputChineseConfig.OffY,
			mode_->InputChineseConfig.Width,
			mode_->InputChineseConfig.Height, nullptr);
		if (IResourceService::Ins()->IsRenderMode())
			view->DisplayValue = mode_->InputChineseConfig.Text;
		else
			view->DisplayValue = "";
		UI::IResourceService::GB2312toUtf8(view->DisplayValue);
		InitMVCView(view);;
	}

	void ChineseInputControl::OnReady()
	{
		HandleSysChange(Permission);
	}

	void ChineseInputControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->InputChineseConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->InputChineseConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->InputChineseConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->InputChineseConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->InputChineseConfig.Perm, mode_->InputChineseConfig.X + mode_->InputChineseConfig.OffX, mode_->InputChineseConfig.Y + mode_->InputChineseConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}

	void ChineseInputControl::HandleDataVar(Project::DataVarId &varId)
	{
		ChineseInputView* pView = static_cast<ChineseInputView*>(pView_);
		if (mode_->InputChineseConfig.VOffX.Cmp(varId))
		{
			mode_->InputChineseConfig.OffX = UI::UIData::Number<int>(mode_->InputChineseConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->InputChineseConfig.Perm, mode_->InputChineseConfig.X + mode_->InputChineseConfig.OffX, mode_->InputChineseConfig.Y + mode_->InputChineseConfig.OffY);
		}
		if (mode_->InputChineseConfig.VOffY.Cmp(varId))
		{
			mode_->InputChineseConfig.OffY = UI::UIData::Number<int>(mode_->InputChineseConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->InputChineseConfig.Perm, mode_->InputChineseConfig.X + mode_->InputChineseConfig.OffX, mode_->InputChineseConfig.Y + mode_->InputChineseConfig.OffY);
			//pView_->show();
		}
		if (mode_->InputChineseConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->InputChineseConfig.Perm, pView_);
		if (mode_->InputChineseConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->InputChineseConfig.Perm, pView_);

		if (mode_->InputChineseConfig.WriteVar.Cmp(varId))
		{
			int count = UIDataService::Ins().GetDataCounts(mode_->InputChineseConfig.WriteVar);
			char *buf = new char[count + 1];
			memset(buf, '\0', count + 1);
			UIDataService::Ins().GetString(mode_->InputChineseConfig.WriteVar, buf, count);
			pView->DisplayValue = buf;
			if (!UI::CodeFormatUtility::IsStrUtf8(pView->DisplayValue.c_str()))
				UI::IResourceService::GB2312toUtf8(pView->DisplayValue);
			if (mode_->InputChineseConfig.Perm.HasShowPerm && mode_->InputChineseConfig.Perm.HasLimitShowPerm)
				pView->redraw();
			delete[] buf;
		}
		if (mode_->InputChineseConfig.KeyMethod.KeypadVar.Cmp(varId))
		{
			if (UI::UIData::Bit(mode_->InputChineseConfig.KeyMethod.KeypadVar))
			{
				map<int, vector<WinCtrlID>>::iterator  iter;
				iter = mode_->InputChineseConfig.SequenceGroup.find(mode_->InputChineseConfig.Sequence.GroupId);
				if (iter != mode_->InputChineseConfig.SequenceGroup.end())
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

	int ChineseInputControl::PeekHMIMessage(Message::Msg* msg)
	{
		ChineseInputView* pView = static_cast<ChineseInputView*>(pView_);
		Project::DataVarId varId;
		switch (msg->GetCode())
		{
		case WM_EVENT_DATEUPDATE:
			varId = Project::DataVarId(msg->LParam);
			HandleDataVar(varId);
			break;
		case WM_EVENT_CLICKED:
			pView->EnterValue();
			break;
		default:
			break;
		}
		return 1;//handled;
	}
}
