/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeyBtnView.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ���Ƽ��̰���
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "UIComm.h"
#include "KeyBtnView.h"
#include "ResourceService.h"
#include "StringUtility.h"
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include "UIData.h"
#include "KeyInputUtility.h"
#include "HMIWindow.h"
#include "PermUtility.h"
#include "ViewShowUtility.h"
#include "SysSetApi.h"
using namespace std;

namespace UI
{
	KeyBtnView::KeyBtnView(int X, int Y, int W, int H, const char *L) : HMIBaseButton(X, Y, W, H, L) {}
	KeyBtnView::~KeyBtnView() {}


	//�������㰲ȫʱ��
	void BtnKeySafeTimerCb(void *data)
	{
		KeyBtnView * view = (KeyBtnView *)data;
		if (!view->IsReleased)
			view->HandleKeyMsg();
	}

	bool KeyBtnView::HandleOperatePush()
	{
		shared_ptr<KeyBtnModel> model = BaseView.GetModel<KeyBtnModel>();
		shared_ptr<KeyBtnControl> ctrl = BaseView.GetControl<KeyBtnControl>();
		IsReleased = false;
		if ((model->KeyBtnConfig.Perm.RequireRegion != 0)
			&& (!UI::PermUtility::HasUserPerm(model->KeyBtnConfig.Perm.RequireRegion)))		//�û������˰�ȫȨ�޲���û��Ȩ��
		{
			if (model->KeyBtnConfig.Perm.IsHideElement)			//����ԭ��
			{
				model->KeyBtnConfig.Perm.HasShowPerm = false;
				hide();
				redraw();
				return false;
			}
			if (model->KeyBtnConfig.Perm.IsPopwin)				//������ʾ����
			{
				ctrl->Win()->OpenDialogPage(SafeTipsWinNum);
				return false;
			}
		}
		if (model->KeyBtnConfig.Perm.IsKeydelay)
		{
			ctrl->Page()->AddTimeout(model->KeyBtnConfig.Perm.DelayTime * 100, BtnKeySafeTimerCb, (void*)this, false);
			return false;
		}
		else if (!PermUtility::HandleConfirmPerm(model->KeyBtnConfig.Perm, ctrl->CtrlId()))
			return false;

		HandleKeyMsg();
		return true;
	}
	void KeyBtnView::HandleKeyMsg()
	{
		shared_ptr<KeyBtnControl> ctrl = BaseView.GetControl<KeyBtnControl>();
		shared_ptr<KeyBtnModel> model = BaseView.GetModel<KeyBtnModel>();
		/*���ͼ�ֵ*/
		ctrl->Page()->Win()->SendKey(model->KeyBtnConfig.AsciiNum);
		if (model->KeyBtnConfig.Perm.HasShowPerm && model->KeyBtnConfig.Perm.HasLimitShowPerm)
			redraw();
		PermUtility::QuitLimitPerm(model->KeyBtnConfig.Perm);
	}

	int KeyBtnView::handle(int event)
	{
		shared_ptr<KeyBtnModel> model = BaseView.GetModel<KeyBtnModel>();
		shared_ptr<KeyBtnControl> ctrl = BaseView.GetControl<KeyBtnControl>();
		switch (event)
		{
		case FL_PUSH:
			SysSetApi::TriggerBeep();
			CurrentStatus = 1;
			if (model->KeyBtnConfig.IsCloseBtn)			//�رհ�ť
			{
				ctrl->Win()->ClosePage(ctrl->Page()->Winno());
				return 1;
			}
			if (HandleOperatePush())
			{
				redraw();
				return HMIBaseButton::handle(event);
			}			
			redraw();
			return 1;
		case FL_RELEASE:
			if (when() & FL_WHEN_RELEASE)
			{
				ctrl->Page()->RemoveTimeout(BtnKeySafeTimerCb, (void*)this);		//�Ƴ���ʱ��
				CurrentStatus = 0;
				IsReleased = true;
			}
			redraw();
			return HMIBaseButton::handle(event);
		default:
			return HMIBaseButton::handle(event);
		}
	}
		 
	void KeyBtnView::draw()
	{
		shared_ptr<KeyBtnControl> ctrl = BaseView.GetControl<KeyBtnControl>();
		shared_ptr<KeyBtnModel> model = BaseView.GetModel<KeyBtnModel>();
		IResourceService::Ins()->SetRenderStatus(CurrentStatus,
			model->KeyBtnConfig.PicKey.size() < model->KeyBtnConfig.StrStyles.size()
			? model->KeyBtnConfig.PicKey.size() : model->KeyBtnConfig.StrStyles.size());		//��ȡ��ǰ״̬
		int status = CurrentStatus;;
		if (model->KeyBtnConfig.AsciiNum == 16)	//shift����
			status = KeyInputUtility::IsChinese();
		if(model->KeyBtnConfig.AsciiNum == 20)	//caps���� 
			status = KeyInputUtility::IsUpper();
		
		//��ȡ��ǰ״̬ͼƬkeyֵ
		Fl_Image * btnImage = nullptr;
		if((size_t)status < model->KeyBtnConfig.PicKey.size())
			btnImage = IResourceService::Ins()->GetImage(model->KeyBtnConfig.PicKey[status].KeyVal);
		if (btnImage) {
			image(btnImage);
			align(FL_ALIGN_IMAGE_BACKDROP);		//���ñ���ͼƬ
		}
		else
		{
			box(FL_NO_BOX);
			LOG_INFO_("Key Btn Pic is NULL\n");
		}

		//����ͼƬ����
		//box(FL_FLAT_BOX);
		draw_box();

		//�жϵ�ǰ״̬�Ƿ�Խ��
		if ((size_t)status >= model->KeyBtnConfig.StrStyles.size())
			return;
		//��ȡ��ǰ״̬������
		string text = StringUtility::GetDrawString(IResourceService::Ins(),
			model->KeyBtnConfig.Txt, status);
		UI::IResourceService::GB2312toUtf8(text);

		/*����������ʽ����С*/
		fl_font(UI::IResourceService::GetFontIdx(model->KeyBtnConfig.StrStyles[status].Font.Name),
			model->KeyBtnConfig.StrStyles[status].Font.Size);

		/*����������ɫ*/
		Fl_Color textcolor = fl_rgb_color(RGBColor(model->KeyBtnConfig.StrStyles[status].Colors));
		fl_color(active() ? textcolor : fl_inactive(textcolor));

		/*�����ı�*/
		fl_draw(text.data(), model->KeyBtnConfig.X + model->KeyBtnConfig.OffX,
			model->KeyBtnConfig.Y + model->KeyBtnConfig.OffY,
			model->KeyBtnConfig.Width, model->KeyBtnConfig.Height,
			model->KeyBtnConfig.StrStyles[status].Align);	//FL_ALIGN_CENTER
	}
}
