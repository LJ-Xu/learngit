/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ValveView.h
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : 阀门控件绘制类
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "ValveView.h"
#include "ValveModel.h"
#include "ValveControl.h"
#include "ResourceService.h"
#include "HMIWindow.h"
#include "UIData.h"
#include "PermUtility.h"
#include "ViewShowUtility.h"
#include "OperatorGControl.h"
#include "SysSetApi.h"

namespace UI
{
	//按键满足安全时间
	void ValveSafeTimerCb(void *data)
	{
		ValveView * view = (ValveView *)data;
		if (!view->IsReleased)
		{
			view->HaveOperateLimit = true;
			view->HandleClick();
		}
	}
	ValveView::ValveView(int x, int y, int w, int h, const char* l)
		: HMIBaseButton(x, y, w, h, l) {
	}

	ValveView::~ValveView() {

	}

	// 处理点击事件
	int ValveView::handle(int event) {
#if 1
		shared_ptr<ValveModel> model = BaseView.GetModel<ValveModel>();
		shared_ptr<ValveControl> ctrl = BaseView.GetControl<ValveControl>();
		switch (event)
		{
		case FL_PUSH:
			//model->ValveUnit.CurrentStatus = 1;
			SysSetApi::TriggerBeep();
			if (HandleOperatePush())
				return HMIBaseButton::handle(event);
			return 1;
		case FL_RELEASE:
			if (when() & FL_WHEN_RELEASE)
			{
				//移除定时器
				ctrl->Page()->RemoveTimeout(ValveSafeTimerCb, (void*)this);	
				//model->ValveUnit.CurrentStatus = 0;
				IsReleased = true;
				if (HaveOperateLimit && model->ValveUnit.Act == Project::BtnAction::BtnSHORTON)
				{
					HaveOperateLimit = false;
#if TEMPRWDIF
					if (model->ValveUnit.WriteVar != NULL_VID_VALUE)
#else
					if (model->ValveUnit.IsRWDiff)  //读写使用不同寄存器
#endif
					{
						if (model->ValveUnit.IsRecord)
							OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->ValveUnit.CtrlName, Storage::OperatorAction::OA_SET,
								model->ValveUnit.WriteVar, UIData::Bit(model->ValveUnit.WriteVar), false);
						UI::DataApi::AppBit(model->ValveUnit.WriteVar, false);
					}
					else
					{
						if (model->ValveUnit.IsRecord)
							OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->ValveUnit.CtrlName, Storage::OperatorAction::OA_SET,
								model->ValveUnit.ReadVar, UIData::Bit(model->ValveUnit.ReadVar), false);
						UI::DataApi::AppBit(model->ValveUnit.ReadVar, false);
					}
				}
			}
			return HMIBaseButton::handle(event);
		default:
			return HMIBaseButton::handle(event);
		}
#else
		shared_ptr<ValveModel> model = BaseView.GetModel<ValveModel>();
		shared_ptr<ValveControl> ctrl = BaseView.GetControl<ValveControl>();
		int button = -1;
		switch (event)
		{
		case FL_PUSH:
		case FL_DRAG:
		case FL_RELEASE:
			// 左击或者右击
			button = Fl::event_button();
			if (button == 3) {	// Right Button
				return 1;
			}
			break;
		default:
			break;
		}
		// 绑定瞬时ON事件
		switch (event)
		{
		case FL_PUSH:
			// 记录鼠标按下控件时间
			//begin_ = std::chrono::high_resolution_clock::now();
			if (model->ValveUnit.Act == Project::BtnAction::BtnSHORTON) {
				// ctrl->Page()->AddTimeout(1, ShortON_CB, (void*)this, true);
				model->ValveUnit.CurrentStatus = 1;
				// 设置写入值
				// UIData::Number<WORD>(model->ValveUnit.WriteVar, 1);
				redraw();
			}
			break;
		case FL_RELEASE:
			if (when() & FL_WHEN_RELEASE) {
				// 瞬时ON时，移除定时器
				// if (model->ValveUnit.Act == Project::BtnAction::SHORTON) {
				// 	ctrl->Page()->RemoveTimeout(ShortON_CB, (void*)this);
				// }
				// 记录鼠标松开控件时间
				end_ = std::chrono::high_resolution_clock::now();
				MilliSecs interval = std::chrono::duration_cast<MilliSecs>(end_ - begin_);
				if (interval.count() > 0) {
					HandleClick();
					redraw();
				}
				// 设置写入值
				// UIData::Number<WORD>(model->ValveUnit.WriteVar, model->ValveUnit.CurrentStatus);
			}
			break;
		default:
			break;
		}
		return HMIBaseButton::handle(event);
#endif
	}

	//// 置ON并重绘
	//void ShortON_CB(void * data) {
	//	ValveView * view = (ValveView *)data;
	//	shared_ptr<ValveModel> model = view->BaseView.GetModel<ValveModel>();
	//	model->ValveUnit.CurrentStatus = 1;
	//	// 设置写入值
	//	UIData::Number<WORD>(model->ValveUnit.WriteVar, 1);
	//	view->redraw();
	//}

	// 处理控件点击
	void ValveView::HandleClick() {
		shared_ptr<ValveModel> model = BaseView.GetModel<ValveModel>();
		shared_ptr<ValveControl> ctrl = BaseView.GetControl<ValveControl>();
		switch (model->ValveUnit.Act)
		{
		case Project::BtnAction::BtnSETON:		// 导通

			model->ValveUnit.CurrentStatus = 1;
			break;
		case Project::BtnAction::BtnSETOFF:	// 关闭
			model->ValveUnit.CurrentStatus = 0;
			break;
		case Project::BtnAction::BtnSHORTON:	// 瞬时
			model->ValveUnit.CurrentStatus = !model->ValveUnit.Logic;
			break;
		case Project::BtnAction::BtnOPPOSITE:	// 取反
			model->ValveUnit.CurrentStatus ^= 1;
			break;
		default:
			break;
		}
#if TEMPRWDIF
		if (model->ValveUnit.WriteVar!= NULL_VID_VALUE)
#else
		if (model->ValveUnit.IsRWDiff)  //读写使用不同寄存器
#endif
		{

			if (model->ValveUnit.IsRecord)
				OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->ValveUnit.CtrlName, Storage::OperatorAction::OA_SET,
					model->ValveUnit.WriteVar, UIData::Bit(model->ValveUnit.WriteVar), (bool)(model->ValveUnit.CurrentStatus^ (bool)model->ValveUnit.Logic));
			UI::DataApi::AppBit(model->ValveUnit.WriteVar, (bool)model->ValveUnit.CurrentStatus^ (bool)model->ValveUnit.Logic);
		}
		else
		{
			if (model->ValveUnit.IsRecord)
				OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->ValveUnit.CtrlName, Storage::OperatorAction::OA_SET,
					model->ValveUnit.ReadVar, UIData::Bit(model->ValveUnit.ReadVar), (bool)(model->ValveUnit.CurrentStatus^ (bool)model->ValveUnit.Logic));
			UI::DataApi::AppBit(model->ValveUnit.ReadVar, (bool)model->ValveUnit.CurrentStatus^ (bool)model->ValveUnit.Logic);
		}
	}

	bool ValveView::HandleOperatePush()
	{
		shared_ptr<ValveModel> model = BaseView.GetModel<ValveModel>();
		shared_ptr<ValveControl> ctrl = BaseView.GetControl<ValveControl>();
		IsReleased = false;
		if ((model->ValveUnit.Perm.RequireRegion != 0)
			&& (!UI::PermUtility::HasUserPerm(model->ValveUnit.Perm.RequireRegion)))		//用户设置了安全权限并且没有权限
		{
			if (model->ValveUnit.Perm.IsHideElement)			//隐藏原件
			{
				model->ValveUnit.Perm.HasShowPerm = false;
				hide();
				redraw();
				return false;
			}
			if (model->ValveUnit.Perm.IsPopwin)				//弹出提示窗口
			{
				ctrl->Win()->OpenDialogPage(SafeTipsWinNum);
				return false;
			}
		}
		if (model->ValveUnit.Perm.IsKeydelay)
		{
			ctrl->Page()->AddTimeout(model->ValveUnit.Perm.DelayTime * 100, ValveSafeTimerCb, (void*)this, false);
			return false;
		}
		else if (!PermUtility::HandleConfirmPerm(model->ValveUnit.Perm, ctrl->CtrlId()))
			return false;
		HaveOperateLimit = true;
		HandleClick();
		return true;
	}


	// 绘制控件
	void ValveView::draw() {
		shared_ptr<ValveModel> model = BaseView.GetModel<ValveModel>();
		shared_ptr<ValveControl> ctl = BaseView.GetControl<ValveControl>();
		FinX = model->ValveUnit.X + model->ValveUnit.OffX;
		FinY = model->ValveUnit.Y + model->ValveUnit.OffY;
		// 获取资源图片信息
		//vector<Project::ImageResId> resIds = model->ValveUnit.PicKey;
		// 获取当前状态资源图片
		unique_ptr<Project::ImageResId> key;
		//key.reset(new Project::ImageResId(resIds[model->ValveUnit.CurrentStatus]));
		IResourceService::Ins()->SetRenderStatus(model->ValveUnit.CurrentStatus, model->ValveUnit.PicKey.size());
		switch (model->ValveUnit.CurrentStatus)
		{
		case 0:									// Close
			if (model->ValveUnit.PicKey.size() > 0) {
				key.reset(new Project::ImageResId(model->ValveUnit.PicKey[0]));
			}
			break;
		case 1:									// Open
			if (model->ValveUnit.PicKey.size() > 1) {
				key.reset(new Project::ImageResId(model->ValveUnit.PicKey[1]));
			}
			break;
		default:
			break;
		}
		
		if (!key) {
			return;
		}
		// 获取当前图片
		Fl_Image * img = IResourceService::Ins()->GetImage(key->KeyVal);
		if (img) {
			// 设置控件图片
			image(img);
			// 设置图片为背景
			align(FL_ALIGN_IMAGE_BACKDROP);
			// 设置绘制样式
			box(FL_NO_BOX);
		}
		// 绘制背景图片
		draw_box();

		if (model->ValveUnit.CurrentStatus)
		{
			//表现流速
			//坐标起点
			int curmk = model->ValveUnit.Dir ? (MarkCount - CurrentSpeendMark) : CurrentSpeendMark;
			int mkx = FinX + model->ValveUnit.InnerRectX + model->ValveUnit.InnerRectW / MarkCount * curmk;
			int mky = FinY + model->ValveUnit.InnerRectY;
			fl_color(FL_WHITE);
			fl_rectf(mkx,
				mky,
				ctl->FluidSpeedMarkWidth,
				model->ValveUnit.InnerRectH);
		}
	}
}

