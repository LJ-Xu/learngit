/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RecipeTransferView.cpp
 * Author   : qiaodan
 * Date     : 2021-03-02
 * Descript : 绘制配方传输
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "UIComm.h"
#include "RecipeTransferView.h"
#include "ResourceService.h"
#include "StringUtility.h"
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include "UIData.h"
#include "RecipeStorage.h"
#include "ProjectDataTables.h"
#include "PermUtility.h"
#include "HMIWindow.h"
#include "SysSetApi.h"
#include "OperatorGControl.h"

using namespace std;

namespace UI
{
	RecipeTransferView::RecipeTransferView(int X, int Y, int W, int H, const char *L) : HMIBaseButton(X, Y, W, H, L) {}
	RecipeTransferView::~RecipeTransferView() {}

	void RecipeTransferView::InitDraw()
	{
		shared_ptr<RecipeTransferModel> model = BaseView.GetModel<RecipeTransferModel>();
		RecipeDT::Ins()->GetDataTypes(model->TransferRecipeConfig.RecipeId, ColInfo);
	}

	void RecipeTransferView::HandleClick(bool push)
	{
		shared_ptr<RecipeTransferModel> model = BaseView.GetModel<RecipeTransferModel>();
		shared_ptr<RecipeTransferControl> ctrl = BaseView.GetControl<RecipeTransferControl>();
		if ((push && model->TransferRecipeConfig.Action == 0) ||
			(!push && model->TransferRecipeConfig.Action == 1))		//触发
		{
			short index;
			DataVarId indexvar = RecipeDT::Ins()->GetIndexVar(model->TransferRecipeConfig.RecipeId);
			if (indexvar == DataVarId::NullId)
				index = LocalData::GetNumberData<short>(SYS_PSW_RecipeIndex);
			else
				index = UI::UIData::Number<short>(indexvar);
			if (model->TransferRecipeConfig.TransmitMode == Project::RecipeTransferAction::DownLoadToPLC)
			{
				int row = Storage::RecipeStorage::Ins()->GetCountByRepiceName(model->TransferRecipeConfig.RecipeId);
				if (model->TransferRecipeConfig.IsRecord)
					OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->TransferRecipeConfig.CtrlName, model->TransferRecipeConfig.PlcVarIdRef,
						Storage::OperatorAction::OA_DOWNLOND, model->TransferRecipeConfig.RecipeId, model->TransferRecipeConfig.RecipeCount);
				DataApi::RecipeToPLC(model->TransferRecipeConfig.RecipeId, model->TransferRecipeConfig.RecipeCount, index,
					model->TransferRecipeConfig.PlcVarIdRef, ColInfo, model->TransferRecipeConfig.CompleteVarIdRef);
			}
			else if (model->TransferRecipeConfig.TransmitMode == Project::RecipeTransferAction::UpLoadFromPLC)
			{
				int row = Storage::RecipeStorage::Ins()->GetCountByRepiceName(model->TransferRecipeConfig.RecipeId);
				if (model->TransferRecipeConfig.IsRecord)
					OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->TransferRecipeConfig.CtrlName, model->TransferRecipeConfig.PlcVarIdRef,
						Storage::OperatorAction::OA_UPLOAD, model->TransferRecipeConfig.RecipeId, model->TransferRecipeConfig.RecipeCount);
				DataApi::RecipeFromPLC(model->TransferRecipeConfig.RecipeId, model->TransferRecipeConfig.RecipeCount, index,
					model->TransferRecipeConfig.PlcVarIdRef, ColInfo, model->TransferRecipeConfig.CompleteVarIdRef);
			}
			PermUtility::QuitLimitPerm(model->TransferRecipeConfig.Perm);

		}
		
	}

	//按键满足安全时间
	void TransferRecipeTimerCb(void *data)
	{
		RecipeTransferView * view = (RecipeTransferView *)data;
		shared_ptr<RecipeTransferModel> model = view->BaseView.GetModel<RecipeTransferModel>();

		if (!view->IsReleased)
		{
			view->HaveOperateLimit = true;
			if (model->TransferRecipeConfig.Action == 0)
				view->HandleClick(true);
		}
	}


	bool RecipeTransferView::HandleOperatePush()
	{
		shared_ptr<RecipeTransferModel> model = BaseView.GetModel<RecipeTransferModel>();
		shared_ptr<RecipeTransferControl> ctrl = BaseView.GetControl<RecipeTransferControl>();
		IsReleased = false;
		if ((model->TransferRecipeConfig.Perm.RequireRegion != 0)
			&& (!UI::PermUtility::HasUserPerm(model->TransferRecipeConfig.Perm.RequireRegion)))		//用户设置了安全权限并且没有权限
		{
			HaveOperateLimit = false;
			if (model->TransferRecipeConfig.Perm.IsHideElement)			//隐藏原件
			{
				model->TransferRecipeConfig.Perm.HasShowPerm = false;
				hide();
				redraw();
				return false;
			}
			if (model->TransferRecipeConfig.Perm.IsPopwin)				//弹出提示窗口
			{
				ctrl->Win()->OpenDialogPage(SafeTipsWinNum);
				return false;
			}
		}
		if (model->TransferRecipeConfig.Perm.IsKeydelay)
		{
			ctrl->Page()->AddTimeout(model->TransferRecipeConfig.Perm.DelayTime * 100, TransferRecipeTimerCb, (void*)this, false);
			HaveOperateLimit = false;
			return false;
		}
		else if (!PermUtility::HandleConfirmPerm(model->TransferRecipeConfig.Perm, ctrl->CtrlId()))
		{
			HaveOperateLimit = false;
			return false;
		}
		HaveOperateLimit = true;
		if(model->TransferRecipeConfig.Action == 0)
			HandleClick(true);
		return true;
	}

	int RecipeTransferView::handle(int event)
	{
		shared_ptr<RecipeTransferModel> model = BaseView.GetModel<RecipeTransferModel>();
		shared_ptr<RecipeTransferControl> ctrl = BaseView.GetControl<RecipeTransferControl>();

		switch (event)
		{
		case FL_PUSH:
			SysSetApi::TriggerBeep();
			CurrentStatus = 1;
			if (HandleOperatePush())
				return HMIBaseButton::handle(event);
			redraw();
			return 1;
		case FL_RELEASE:
			if (when() & FL_WHEN_RELEASE)
			{
				IsReleased = true;
				ctrl->Page()->RemoveTimeout(TransferRecipeTimerCb, (void*)this);		//移除定时器
				CurrentStatus = 0;
				if (HaveOperateLimit && model->TransferRecipeConfig.Action == 1)
				{
					HaveOperateLimit = false;
					HandleClick(false);
				}
				redraw();			//重绘
			}
			return HMIBaseButton::handle(event);
		default:
			return HMIBaseButton::handle(event);
		}
	}

	void RecipeTransferView::draw()
	{
		shared_ptr<RecipeTransferControl> ctrl = BaseView.GetControl<RecipeTransferControl>();
		shared_ptr<RecipeTransferModel> model = BaseView.GetModel<RecipeTransferModel>();
		//获取当前状态
		int status = CurrentStatus;
		IResourceService::Ins()->SetRenderStatus(status,
			model->TransferRecipeConfig.PicKey.size() < model->TransferRecipeConfig.StrStyles.size()
			? model->TransferRecipeConfig.PicKey.size(): model->TransferRecipeConfig.StrStyles.size());
		//判断当前状态是否越界
		if ((size_t)status >= model->TransferRecipeConfig.PicKey.size() || (size_t)status >= model->TransferRecipeConfig.StrStyles.size())
			return;
		//获取当前状态图片key值
		//if (model->TransferRecipeConfig.PicKey.size() == 0 || model->TransferRecipeConfig.StrStyles.size() == 0)
		//	return;
		Fl_Image * btnImage = IResourceService::Ins()->GetImage(model->TransferRecipeConfig.PicKey[status].KeyVal);
		if (!btnImage) {
			LOG_INFO_("RecipeBtn image is NULL\n");
		}
		else {
			image(btnImage);
			align(FL_ALIGN_IMAGE_BACKDROP);		//设置背景图片
			box(FL_NO_BOX);
		}
		//绘制图片框体
		draw_box();
		//获取当前状态下文字
		string text = StringUtility::GetDrawString(IResourceService::Ins(),
			model->TransferRecipeConfig.Txt, status);
		UI::IResourceService::GB2312toUtf8(text);

		/*设置字体样式及大小*/
		fl_font(UI::IResourceService::GetFontIdx(model->TransferRecipeConfig.StrStyles[status].Font.Name),
			model->TransferRecipeConfig.StrStyles[status].Font.Size);

		/*设置字体颜色*/
		Fl_Color textcolor = fl_rgb_color(RGBColor(model->TransferRecipeConfig.StrStyles[status].Colors));
		//fl_color(textcolor);
		fl_color(active() ? textcolor : fl_inactive(textcolor));

		/*绘制文本*/
		fl_draw(text.data(), model->TransferRecipeConfig.X + model->TransferRecipeConfig.OffX,
			model->TransferRecipeConfig.Y + model->TransferRecipeConfig.OffY,
			model->TransferRecipeConfig.Width, model->TransferRecipeConfig.Height,
			model->TransferRecipeConfig.StrStyles[status].Align);	//FL_ALIGN_CENTER
	}
}
