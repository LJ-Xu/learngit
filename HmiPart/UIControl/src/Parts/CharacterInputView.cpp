/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : CharacterInputView.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ������ʾ������֡�����ͼƬ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "CharacterInputView.h"
#include "CharacterInputControl.h"
#include "CharacterInputModel.h"
#include <FL/Fl_Input_.H>
#include "ResourceService.h"
#include "HMIWindow.h"
#include "Message.h"
#include "UIData.h"
#include "KeyInputUtility.h"
#include "NoticesUtility.h"
#include "OperatorGControl.h"
#include "SysSetApi.h"
namespace UI
{
	CharacterInputView::CharacterInputView(int X, int Y, int W, int H, const char* l) : InputView(X, Y, W, H, l) {}
	CharacterInputView::~CharacterInputView() {}

	void CharacterInputView::draw()
	{
		shared_ptr<CharacterInputModel> model = BaseView.GetModel<CharacterInputModel>();
		//���ƿ���
		Fl_Image *rgbImage = IResourceService::Ins()->GetImage(model->InputStrConfig.Key.KeyVal);
		if (!rgbImage) 
			LOG_INFO_("Character Input Pic is NULL\n");
		else {
			image(rgbImage);
			align(FL_ALIGN_IMAGE_BACKDROP);		//���ñ���ͼƬ
			box(FL_NO_BOX);
		}
		if (OpenKeypage)
		{
			Fl_Boxtype b = box();
			if (damage() & FL_DAMAGE_ALL) {
				draw_box();
			}
			
			/*��������*/
			textfont(UI::IResourceService::GetFontIdx(model->InputStrConfig.TextStyle.Font.Name));
			textsize(model->InputStrConfig.TextStyle.Font.Size);
			textcolor(fl_rgb_color(RGBColor(model->InputStrConfig.TextStyle.Colors)));
			drawtext(model->InputStrConfig.X + model->InputStrConfig.OffX,
				model->InputStrConfig.Y + model->InputStrConfig.OffY,
				model->InputStrConfig.Width, model->InputStrConfig.Height,
				model->InputStrConfig.TextStyle.Align, rgbImage, model->InputStrConfig.PswMode);
		}
		else
		{
			/*ת����ʽ*/
			//char *dst = new char[DisplayValue.size() * 4];
			//string codestring = CodeFormatUtility::CodeConversion(DisplayValue, model->InputStrConfig.Rule);
			//value(dst);
			//delete[] dst;
			//if (model->InputStrConfig.ByteConversion == 1)		//�ߵ��ֽ�ת��
			//	codestring = CodeFormatUtility::EndianSwap(codestring);
			/*�ߵ��ֽ�ת��*/
			string codestring = DisplayValue;
			if (model->InputStrConfig.ByteConversion == 1
				&& model->InputStrConfig.Rule != CodeFormatUtility::UTF_16)
				codestring = CodeFormatUtility::EndianSwap(codestring);
			value(codestring.c_str());
			/*������������*/
			Fl_Boxtype b = box();
			if (damage() & FL_DAMAGE_ALL) {
				draw_box();
			}
			/*��������*/
			textfont(UI::IResourceService::GetFontIdx(model->InputStrConfig.TextStyle.Font.Name));
			textsize(model->InputStrConfig.TextStyle.Font.Size);
			/*����������ɫ������*/
			Fl_Color tcolor = fl_rgb_color(RGBColor(model->InputStrConfig.TextStyle.Colors));
			textcolor(tcolor);

			drawtext(model->InputStrConfig.X + model->InputStrConfig.OffX,
				model->InputStrConfig.Y + model->InputStrConfig.OffY,
				model->InputStrConfig.Width, model->InputStrConfig.Height,
				model->InputStrConfig.TextStyle.Align, rgbImage,model->InputStrConfig.PswMode);
		}
		
	}

	void CharacterInputView::FoucsNextSequence()
	{
		shared_ptr<CharacterInputModel> model = BaseView.GetModel<CharacterInputModel>();
		shared_ptr<CharacterInputControl> ctrl = BaseView.GetControl<CharacterInputControl>();
		map<int, vector<WinCtrlID>>::iterator  iter;
		iter = model->InputStrConfig.SequenceGroup.find(model->InputStrConfig.Sequence.GroupId);
		if (iter != model->InputStrConfig.SequenceGroup.end())			//��������
		{
			//if (!model->InputStrConfig.Sequence.CancelSequenceAfterCompleted )
			//{
			for (size_t i = 0; i < iter->second.size(); i++)
			{
				if (iter->second[i] == ctrl->CtrlId())
				{
					if (i == iter->second.size() - 1)			//�������һ���ؼ���ѭ��
					{
						std::shared_ptr<BaseControl> nextctrl = ctrl->Page()->FindCtrl(GetCtrlNo(iter->second[0]));
						InputView *nextview = (InputView *)nextctrl->GetView();
						nextview->NeedHandleFocus = true;
						nextview->take_focus();
						//Fl::focus(nextview);
						break;
					}
					else if (i < iter->second.size() - 1)
					{
						std::shared_ptr<BaseControl> nextctrl = ctrl->Page()->FindCtrl(GetCtrlNo(iter->second[i + 1]));
						InputView *nextview = (InputView *)nextctrl->GetView();
						nextview->NeedHandleFocus = true;
						nextview->take_focus();
						break;
					}
				}
			}
			//}
		}
		NeedHandleFocus = false;
	}
	bool CharacterInputView::HandleOperatePerm()
	{
		shared_ptr<CharacterInputModel> model = BaseView.GetModel<CharacterInputModel>();
		shared_ptr<CharacterInputControl> ctrl = BaseView.GetControl<CharacterInputControl>();
		if ((model->InputStrConfig.Perm.RequireRegion != 0)
			&& (!UI::PermUtility::HasUserPerm(model->InputStrConfig.Perm.RequireRegion)))		//�û������˰�ȫȨ�޲���û��Ȩ��
		{
			if (model->InputStrConfig.Perm.IsHideElement)			//����ԭ��
			{
				model->InputStrConfig.Perm.HasShowPerm = false;
				hide();
				redraw();
				return false;
			}
			if (model->InputStrConfig.Perm.IsPopwin)				//������ʾ����
			{
				ctrl->Win()->OpenDialogPage(SafeTipsWinNum);
				return false;
			}
		}
		return true;
	}
	void CharacterInputView::InitStartInput()
	{
		shared_ptr<CharacterInputControl> ctrl = BaseView.GetControl<CharacterInputControl>();
		shared_ptr<CharacterInputModel> model = BaseView.GetModel<CharacterInputModel>();
		char dataflag = DataApi::GetDataFlag(model->InputStrConfig.WriteVar);
		if (dataflag == 0 || !HandleOperatePerm())		//û��Ȩ���򷵻�
			return;
		/*д��ǰ֪ͨ*/
		NoticesUtility::NoticeWrite(model->InputStrConfig.ResBef);
		/*����Ҫ����focus�¼�*/
		NeedHandleFocus = false;
		OpenKeypage = true;

		/*�򿪼��̴���*/
		if (model->InputStrConfig.KeyMethod.KeypadSrc == 0)	//ʹ�õ�������
			ctrl->Win()->OpenDialogPage(model->InputStrConfig.KeyMethod.Keypad.KeypadIdx, nullptr,
				model->InputStrConfig.KeyMethod.Keypad.KeypadPos.X,
				model->InputStrConfig.KeyMethod.Keypad.KeypadPos.Y);
		if (Fl::focus() != this) {
			Fl::focus(this);
		}
		mark(0);			//ѡ����������
		/*���õ�ǰֵ*/
		int len = LocalData::GetLocalVarLen(SYS_PSW_INPUTKEY_CURVAL);
		char *data = new char[len + 1];
		memset(data, '\0', len + 1);
		memcpy(data, value(), strlen(value()) > len ? len : strlen(value()));
		LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, data);
		delete[] data;
		redraw();
	}
	void CharacterInputView::EndInput()
	{
		shared_ptr<CharacterInputModel> model = BaseView.GetModel<CharacterInputModel>();
		shared_ptr<CharacterInputControl> ctrl = BaseView.GetControl<CharacterInputControl>();

		/*�رռ���*/
		if (model->InputStrConfig.KeyMethod.KeypadSrc == 0)	//ʹ�õ�������
			ctrl->Win()->ClosePage(model->InputStrConfig.KeyMethod.Keypad.KeypadIdx);
		OpenKeypage = false;
		/*������Ҫ����focus�¼�*/
		NeedHandleFocus = false;
		Fl::focus(0);
		redraw();
	}
	void CharacterInputView::SetWriteData()
	{
		shared_ptr<CharacterInputControl> ctrl = BaseView.GetControl<CharacterInputControl>();
		shared_ptr<CharacterInputModel> model = BaseView.GetModel<CharacterInputModel>();
		//string inputstring = value();
		/*���������д�Ĵ���*/
		int count = UIDataService::Ins().GetDataCounts(model->InputStrConfig.WriteVar);
		char *buf = new char[count + 1];
		memset(buf, '\0', count + 1);
		/*�ߵ��ֽ�ת��*/
		if (model->InputStrConfig.ByteConversion == 1
			&& model->InputStrConfig.Rule != CodeFormatUtility::UTF_16)
			Inputstring = CodeFormatUtility::EndianSwap(Inputstring);
		memcpy(buf, Inputstring.c_str(), Inputstring.size());
		/*����ת��*/
		CodeFormatUtility::Utf8ToCode(model->InputStrConfig.Rule, buf, count);

		/*д������*/
		if (model->InputStrConfig.IsRecord)
		{
			int count = UIDataService::Ins().GetDataCounts(model->InputStrConfig.WriteVar);
			char *prebuf = new char[count + 1];
			memset(prebuf, '\0', count + 1);
			UIDataService::Ins().GetString(model->InputStrConfig.WriteVar, prebuf, count);
			OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->InputStrConfig.CtrlName,
				model->InputStrConfig.WriteVar, prebuf, buf);
			delete[] prebuf;
		}
		DataApi::SetDataBytes(model->InputStrConfig.WriteVar, buf, count);
		/*д���֪ͨ*/
		NoticesUtility::NoticeWrite(model->InputStrConfig.ResAft);
		PermUtility::QuitLimitPerm(model->InputStrConfig.Perm);

	}
	bool CharacterInputView::HandleOperateConfirm()
	{
		shared_ptr<CharacterInputModel> model = BaseView.GetModel<CharacterInputModel>();
		shared_ptr<CharacterInputControl> ctrl = BaseView.GetControl<CharacterInputControl>();
		if (!PermUtility::HandleConfirmPerm(model->InputStrConfig.Perm, ctrl->CtrlId()))
			return false;
		else
		{
			SetWriteData();
			return true;
		}
	}
	int CharacterInputView::handle(int event)
	{
		shared_ptr<CharacterInputModel> model = BaseView.GetModel<CharacterInputModel>();
		shared_ptr<CharacterInputControl> ctrl = BaseView.GetControl<CharacterInputControl>();
		switch (event)
		{
		case FL_FOCUS:
		{
			if (NeedHandleFocus)							//��ǰ�ؼ��Ƿ���Ҫ����focus�¼�
			{
				if (model->InputStrConfig.KeyMethod.Method)	//λ����ģʽ
				{
					/*���δʹ�ܵ�ǰ����*/
					if (!UI::UIData::Bit(model->InputStrConfig.KeyMethod.KeypadVar))
					{
						FoucsNextSequence();				//ʹ��һ��������򲿼���ȡ����
						NeedHandleFocus = false;
						return 0;
					}
				}
				InitStartInput();
				return 0;
			}
			else if (OpenKeypage)
			{
				Fl::focus(this);
			}
			return 1;
		}
		case FL_UNFOCUS:
		{
			if (Fl::focus() != this && model->InputStrConfig.KeyMethod.KeypadSrc)
				OpenKeypage = false;
			return  HMIBaseInput::handle(event);
		}
		case FL_PUSH:
		{
			SysSetApi::TriggerBeep();
			if (model->InputStrConfig.KeyMethod.Method)			//λ����ģʽ
			{
				Fl::focus(0);
				return 1;
			}
			InitStartInput();
			return 1;
		}
		case FL_KEYBOARD:
		{
			/*�����ֵ*/
			int curmark = mark();
			string perValue = value();				//������ǰֵ
			char ascii = Fl::event_text()[0];		//��ȡ�����ֵ
			int del;
			int maxsize = UIDataService::Ins().GetDataCounts(model->InputStrConfig.WriteVar);
			if (model->InputStrConfig.Rule == CodeFormatUtility::UTF_16)
				maxsize = maxsize / 2;
			if (Fl::compose(del)) {
				if (del || Fl::event_length())
				{
					if ((ascii >= 'A'&& ascii <= 'Z') || (ascii >= 'a'&& ascii <= 'z'))
					{
						if (KeyInputUtility::IsUpper() && (ascii >= 'a'&& ascii <= 'z'))		//��д
							ascii -= 32;
						if (!KeyInputUtility::IsUpper() && (ascii >= 'A'&& ascii <= 'Z'))		//Сд
							ascii += 32;
						replace(position(), del ? position() - del : mark(),
							&ascii, 1);
					}
					else
						replace(position(), del ? position() - del : mark(),
							Fl::event_text(), Fl::event_length());
				}
				if (strlen(value()) > (size_t)maxsize)
					value(perValue.c_str());
				int len = LocalData::GetLocalVarLen(SYS_PSW_INPUTKEY_CURVAL);
				char *data = new char[len + 1];
				memset(data, '\0', len + 1);
				memcpy(data, value(), strlen(value()) > len ? len : strlen(value()));
				//memcpy(data, value(), 32);
				LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, data);
				delete[] data;
				return 1;
			}

			if (Fl::event_key() == FL_Enter || Fl::event_key() == FL_KP_Enter)
			{
				//UIDataService::Ins().SetString(model->InputStrConfig.WriteVar, inputstring.c_str());
				Inputstring = value();
				OpenKeypage = false;	//�رռ��̣������д�Ĵ�����ͬ����ctrl������Ϣ�ı���ʾֵ
										//�����д�Ĵ�������ͬ����ֱ����ʾԭ�����ַ���
				EndInput();
				//SetWriteData();		//д������
				HandleOperateConfirm();
				/*�����������*/
				if (!model->InputStrConfig.Sequence.CancelSequenceAfterCompleted)
					FoucsNextSequence();
				return 1;
			}
			if (Fl::event_key() == FL_Escape)	//�˳�����
			{
				EndInput();
				return 1;
			}
			if (Fl::event_key() == FL_Shift_R || Fl::event_key() == FL_Shift_L)
			{
				KeyInputUtility::SetChineseState();
				return 1;
			}
			if (Fl::event_key() == FL_Caps_Lock)
			{
			/*	unsigned int mods = Fl::event_state() & FL_Caps_Lock;
				if(model->InputStrConfig.KeyMethod.KeypadSrc)
					KeyInputUtility::SetUpperState(mods);
				else*/
					KeyInputUtility::SetUpperState();
				return 1;
			}
			if (Fl::event_key() == FL_BackSpace)	//ɾ������
			{
				if (mark() != position()) cut();
				else cut(-1);

				int len = LocalData::GetLocalVarLen(SYS_PSW_INPUTKEY_CURVAL);
				char *data = new char[len + 1];
				memset(data, '\0', len + 1);
				memcpy(data, value(), strlen(value()) > len ? len : strlen(value()));
				LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, data);
				delete[] data;

				return 1;
			}
			return 1;
			//return HMIBaseInput::handle(event);
		}
		default:
			return 1;

			//return HMIBaseInput::handle(event);
		}
		return 1;
	}
}
