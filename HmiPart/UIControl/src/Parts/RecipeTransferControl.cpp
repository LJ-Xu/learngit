/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RecipeTransferControl.cpp
 * Author   : qiaodan
 * Date     : 2021-03-02
 * Descript : 创建配方传输按键绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "RecipeTransferControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
#include "RecipeStorage.h"
namespace UI
{
	RecipeTransferControl::RecipeTransferControl(HMIPage* w) :BaseControl(w)
	{
		//创建RecipeTransferModel
		mode_ = std::shared_ptr<RecipeTransferModel>(new RecipeTransferModel());
		InitMVCModel(mode_);
	}

	void RecipeTransferControl::CreateView()
	{
		//初始化VIEW
		RecipeTransferView* view = new RecipeTransferView(
			mode_->TransferRecipeConfig.X + mode_->TransferRecipeConfig.OffX,
			mode_->TransferRecipeConfig.Y + mode_->TransferRecipeConfig.OffY,
			mode_->TransferRecipeConfig.Width,
			mode_->TransferRecipeConfig.Height, nullptr);
		InitMVCView(view);
		view->InitDraw();
	}

	void RecipeTransferControl::OnReady()
	{
		for (size_t i = 0; i < mode_->TransferRecipeConfig.TransmitVarIdRefs.size(); i++)
		{
			if (mode_->TransferRecipeConfig.TransmitVarIdRefs[i] != Project::DataVarId::NullId)
				perstatus_.push_back(UI::UIData::Bit(mode_->TransferRecipeConfig.TransmitVarIdRefs[i]));
		}
		HandleSysChange(Permission);
	}

	void RecipeTransferControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->TransferRecipeConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->TransferRecipeConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->TransferRecipeConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->TransferRecipeConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->TransferRecipeConfig.Perm,
					mode_->TransferRecipeConfig.X + mode_->TransferRecipeConfig.OffX,
					mode_->TransferRecipeConfig.Y + mode_->TransferRecipeConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}

	void RecipeTransferControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->TransferRecipeConfig.VOffX != Project::DataVarId::NullId)
			mode_->TransferRecipeConfig.OffX = UI::UIData::Number<int>(mode_->TransferRecipeConfig.VOffX) + pageoffx;
		else
			mode_->TransferRecipeConfig.OffX = pageoffx;


		if (mode_->TransferRecipeConfig.VOffY != Project::DataVarId::NullId)
			mode_->TransferRecipeConfig.OffY = UI::UIData::Number<int>(mode_->TransferRecipeConfig.VOffY) + pageoffy;
		else
			mode_->TransferRecipeConfig.OffY = pageoffy;
	}
	void RecipeTransferControl::HandleDataVar(Project::DataVarId &varId)
	{
		RecipeTransferView* pView = static_cast<RecipeTransferView*>(pView_);
		if (mode_->TransferRecipeConfig.VOffX.Cmp(varId))
		{
			mode_->TransferRecipeConfig.OffX = UI::UIData::Number<int>(mode_->TransferRecipeConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->TransferRecipeConfig.Perm, mode_->TransferRecipeConfig.X + mode_->TransferRecipeConfig.OffX, mode_->TransferRecipeConfig.Y + mode_->TransferRecipeConfig.OffY);
		}
		if (mode_->TransferRecipeConfig.VOffY.Cmp(varId))
		{
			mode_->TransferRecipeConfig.OffY = UI::UIData::Number<int>(mode_->TransferRecipeConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->TransferRecipeConfig.Perm, mode_->TransferRecipeConfig.X + mode_->TransferRecipeConfig.OffX, mode_->TransferRecipeConfig.Y + mode_->TransferRecipeConfig.OffY);
		}
		if (mode_->TransferRecipeConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->TransferRecipeConfig.Perm, pView_);
		if (mode_->TransferRecipeConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->TransferRecipeConfig.Perm, pView_);
		for (size_t i = 0; i < mode_->TransferRecipeConfig.TransmitVarIdRefs.size(); i++)
		{
			if (mode_->TransferRecipeConfig.TransmitVarIdRefs[i].Cmp(varId))
			{
				if (mode_->TransferRecipeConfig.TriggerFunc.size() > i)
				{
					int status = UI::UIData::Bit(mode_->TransferRecipeConfig.TransmitVarIdRefs[i]);
					if (perstatus_.size() <= i)
						return;
					if ((mode_->TransferRecipeConfig.TriggerFunc[i] ==  0 && perstatus_[i] == false && status == true)
						|| (mode_->TransferRecipeConfig.TriggerFunc[i] == 1 && perstatus_[i] == true && status == false))	//上升沿
					//if (mode_->TransferRecipeConfig.TriggerFunc[i] != (int)UI::UIData::Bit(mode_->TransferRecipeConfig.TransmitVarIdRefs[i]))
					{
						if (i == 0) //下载
							pView->DownloadRecipe = true;
						if (i == 1)	//上传
							pView->UploadRecipe = true;
					}
					else
					{
						if (i == 0) //下载
							pView->DownloadRecipe = false;
						if (i == 1)	//上传
							pView->UploadRecipe = false;
					}
				}
				if (pView->DownloadRecipe && pView->active())
				{
					int row = Storage::RecipeStorage::Ins()->GetCountByRepiceName(mode_->TransferRecipeConfig.RecipeId);
					short index;
					DataVarId indexvar = RecipeDT::Ins()->GetIndexVar(mode_->TransferRecipeConfig.RecipeId);
					if (indexvar == DataVarId::NullId)
						index = LocalData::GetNumberData<short>(SYS_PSW_RecipeIndex);
					else
						index = UI::UIData::Number<short>(indexvar);
					DataApi::RecipeFromPLC(mode_->TransferRecipeConfig.RecipeId, mode_->TransferRecipeConfig.RecipeCount * row, index,
						mode_->TransferRecipeConfig.PlcVarIdRef, pView->ColInfo, mode_->TransferRecipeConfig.CompleteVarIdRef);
				}
				if (pView->UploadRecipe && pView->active())
				{
					int row = Storage::RecipeStorage::Ins()->GetCountByRepiceName(mode_->TransferRecipeConfig.RecipeId);
					short index;
					DataVarId indexvar = RecipeDT::Ins()->GetIndexVar(mode_->TransferRecipeConfig.RecipeId);
					if (indexvar == DataVarId::NullId)
						index = LocalData::GetNumberData<short>(SYS_PSW_RecipeIndex);
					else
						index = UI::UIData::Number<short>(indexvar);
					DataApi::RecipeToPLC(mode_->TransferRecipeConfig.RecipeId, mode_->TransferRecipeConfig.RecipeCount * row, index,
						mode_->TransferRecipeConfig.PlcVarIdRef, pView->ColInfo, mode_->TransferRecipeConfig.CompleteVarIdRef);
				}
			}
			
		}
	}

	int RecipeTransferControl::PeekHMIMessage(Message::Msg* msg)
	{
		Project::DataVarId varId;
		RecipeTransferView* pView = static_cast<RecipeTransferView*>(pView_);
		switch (msg->GetCode())
		{
		case WM_EVENT_DATEUPDATE:
			varId = Project::DataVarId(msg->LParam);
			HandleDataVar(varId);
			break;
		case WM_EVENT_CLICKED:
		{
			if (mode_->TransferRecipeConfig.Action == 1)
				pView->HandleClick(false);
			else
				pView->HandleClick(true);
			break;
		}
		default:
			break;
		}
		return 1;//handled;
	}
}
