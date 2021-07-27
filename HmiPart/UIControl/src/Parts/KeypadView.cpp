/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeypadView.cpp
 * Author   : qiaodan
 * Date     : 2020-10-22
 * Descript : ���ư���
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "UIComm.h"
#include "KeypadView.h"
#include "ResourceService.h"
#include "StringUtility.h"
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include "UIData.h"
#include "PermUtility.h"
#include "HMIWindow.h"
#include "OperatorGControl.h"
#include "SysSetApi.h"
using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

namespace UI
{
	KeypadView::KeypadView(int X, int Y, int W, int H, const char *L) : HMIBaseButton(X, Y, W, H, L) {}
	KeypadView::~KeypadView() {}

	//��������˲ʱONʱ��󣬽�״̬��OFF
	//void BtnshortON_cb(void *data)
	//{
	//	KeypadView * view = (KeypadView *)data;
	//	shared_ptr<KeypadModel> model = view->BaseView.GetModel<KeypadModel>();
	//	//��ΪOFF״̬
	//	UI::UIData::Bit(model->KeypadConfig.WriteVar, false);
	//	view->HasShortOnEvent = false;
	//}

	void KeypadView::HandleClick()
	{
		shared_ptr<KeypadModel> model = BaseView.GetModel<KeypadModel>();
		shared_ptr<KeypadControl> ctrl = BaseView.GetControl<KeypadControl>();

		switch (model->KeypadConfig.Action)
		{
		case Project::BtnAction::BtnSETON:			//��ΪON״̬
		{
			if (model->KeypadConfig.IsRecord)
				OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->KeypadConfig.CtrlName, Storage::OA_SET,
					model->KeypadConfig.WriteVar,UIData::Bit(model->KeypadConfig.WriteVar), true);
			UI::UIData::Bit(model->KeypadConfig.WriteVar, true);
			break;
		}
		case Project::BtnAction::BtnSETOFF:		//��ΪOFF״̬
		{
			if (model->KeypadConfig.IsRecord)
				OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->KeypadConfig.CtrlName, Storage::OA_SET,
					model->KeypadConfig.WriteVar,UIData::Bit(model->KeypadConfig.WriteVar), false);
			UI::UIData::Bit(model->KeypadConfig.WriteVar, false);
			break;
		}
		case Project::BtnAction::BtnOPPOSITE:		//ȡ��
		{
			bool perstatus = UI::UIData::Bit(model->KeypadConfig.WriteVar);
			bool status = !perstatus;
			if (model->KeypadConfig.IsRecord)
				OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->KeypadConfig.CtrlName, Storage::OA_TOGGLE,
					model->KeypadConfig.WriteVar, perstatus, status);
			UI::UIData::Bit(model->KeypadConfig.WriteVar, status);
			break;
		}
		case Project::BtnAction::BtnSHORTON:				//˲ʱON״̬
		{
			if (model->KeypadConfig.IsRecord)
				OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->KeypadConfig.CtrlName, Storage::OA_SET,
					model->KeypadConfig.WriteVar,UIData::Bit(model->KeypadConfig.WriteVar), true);
			UI::UIData::Bit(model->KeypadConfig.WriteVar, true);
			break;
		}
		default:
			break;
		}
		PermUtility::QuitLimitPerm(model->KeypadConfig.Perm);
	}

	//�������㰲ȫʱ��
	void BtnSafeTimerCb(void *data)
	{
		KeypadView * view = (KeypadView *)data;
		if (!view->IsReleased)
		{
			view->HaveOperateLimit = true;
			view->HandleClick();
		}
	}

	bool KeypadView::HandleOperatePush()
	{
		shared_ptr<KeypadModel> model = BaseView.GetModel<KeypadModel>();
		shared_ptr<KeypadControl> ctrl = BaseView.GetControl<KeypadControl>();
		IsReleased = false;
		if ((model->KeypadConfig.Perm.RequireRegion != 0)
			&& (!UI::PermUtility::HasUserPerm(model->KeypadConfig.Perm.RequireRegion)))		//�û������˰�ȫȨ�޲���û��Ȩ��
		{
			HaveOperateLimit = false;
			if (model->KeypadConfig.Perm.IsHideElement)			//����ԭ��
			{
				model->KeypadConfig.Perm.HasShowPerm = false;
				hide();
				redraw();
				return false;
			}
			if (model->KeypadConfig.Perm.IsPopwin)				//������ʾ����
			{
				ctrl->Win()->OpenDialogPage(SafeTipsWinNum);
				return false;
			}
		}
		if (model->KeypadConfig.Perm.IsKeydelay)
		{
			ctrl->Page()->AddTimeout(model->KeypadConfig.Perm.DelayTime * 100, BtnSafeTimerCb, (void*)this, false);
			HaveOperateLimit = false;
			return false;
		}
		else if (!PermUtility::HandleConfirmPerm(model->KeypadConfig.Perm, ctrl->CtrlId()))
		{
			HaveOperateLimit = false;
			return false;
		}
		HaveOperateLimit = true;
		HandleClick();
		return true;
	}
	int KeypadView::handle(int event)
	{
		shared_ptr<KeypadModel> model = BaseView.GetModel<KeypadModel>();
		shared_ptr<KeypadControl> ctrl = BaseView.GetControl<KeypadControl>();

		switch (event)
		{
		case FL_PUSH:
		{
			char dataflag = DataApi::GetDataFlag(model->KeypadConfig.WriteVar);
			if (dataflag == 0)		return 1;
			SysSetApi::TriggerBeep();
			CurrentStatus = 1;
			if (HandleOperatePush())
			{
				//beginTime_ = high_resolution_clock::now();
				redraw();
				return HMIBaseButton::handle(event);
			}
			redraw();
			return 1;
		}
		case FL_RELEASE:
		{
			if (when() & FL_WHEN_RELEASE)
			{
				CurrentStatus = 0;
				ctrl->Page()->RemoveTimeout(BtnSafeTimerCb, (void*)this);		//�Ƴ���ʱ��
				//char dataflag = DataApi::GetDataFlag(model->KeypadConfig.WriteVar);
				//if (dataflag == 0)
				//{
				//	redraw();
				//	return 1;
				//}
				IsReleased = true;
				if (HaveOperateLimit && model->KeypadConfig.Action == Project::BtnAction::BtnSHORTON)
				{
					HaveOperateLimit = false;
					if (model->KeypadConfig.IsRecord)
						OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->KeypadConfig.CtrlName, Storage::OA_SET,
							model->KeypadConfig.WriteVar,UIData::Bit(model->KeypadConfig.WriteVar), false);
					UI::UIData::Bit(model->KeypadConfig.WriteVar, false);
				}
				//endTime_ = high_resolution_clock::now();
				//milliseconds timeInterval = std::chrono::duration_cast<milliseconds>(endTime_ - beginTime_);
				//if (timeInterval.count() > model->KeypadConfig.Perm.SafeTime)
				//	HandleClick();		//ȷ���������º��ɿ�
					redraw();			//�ػ�
			}
			return HMIBaseButton::handle(event);
		}
		default:
			return HMIBaseButton::handle(event);
		}
	}

	void KeypadView::draw()
	{
		shared_ptr<KeypadControl> ctrl = BaseView.GetControl<KeypadControl>();
		shared_ptr<KeypadModel> model = BaseView.GetModel<KeypadModel>();
		//��ȡ��ǰ״̬
		//int status = value();
		IResourceService::Ins()->SetRenderStatus(CurrentStatus, 
			model->KeypadConfig.PicKey.size() < model->KeypadConfig.StrStyles.size()
			? model->KeypadConfig.PicKey.size() : model->KeypadConfig.StrStyles.size());

		int status = CurrentStatus;
		//��ȡ��ǰ״̬ͼƬkeyֵ
		Fl_Image * btnImage = nullptr;
		if((size_t)status < model->KeypadConfig.PicKey.size())
			btnImage = IResourceService::Ins()->GetImage(model->KeypadConfig.PicKey[status].KeyVal);
		//Fl_Image * btnImage = IResourceService::Ins()->GetImage(model->KeypadConfig.PicKey[status].KeyVal);
		if (!btnImage) {
			LOG_INFO_("btn Image is NULL\n");
		}
		else {
			image(btnImage);
			align(FL_ALIGN_IMAGE_BACKDROP);		//���ñ���ͼƬ
			box(FL_NO_BOX);
		}
		//����ͼƬ����
		draw_box();
		//�жϵ�ǰ״̬�Ƿ�Խ��
		if ((size_t)status >= model->KeypadConfig.StrStyles.size())
			return;
		//��ȡ��ǰ״̬������
		string text = StringUtility::GetDrawString(IResourceService::Ins(),
			model->KeypadConfig.Txt, status);
		UI::IResourceService::GB2312toUtf8(text);

		/*����������ʽ����С*/
		fl_font(UI::IResourceService::GetFontIdx(model->KeypadConfig.StrStyles[status].Font.Name), 
				model->KeypadConfig.StrStyles[status].Font.Size);

		/*����������ɫ*/
		Fl_Color textcolor = fl_rgb_color(RGBColor(model->KeypadConfig.StrStyles[status].Colors));
		//fl_color(textcolor);
		fl_color(active() ? textcolor : fl_inactive(textcolor));

		/*�����ı�*/
		fl_draw(text.data(), model->KeypadConfig.X + model->KeypadConfig.OffX,
			model->KeypadConfig.Y + model->KeypadConfig.OffY,
			model->KeypadConfig.Width, model->KeypadConfig.Height,
			model->KeypadConfig.StrStyles[status].Align);	//FL_ALIGN_CENTER
	}
}
