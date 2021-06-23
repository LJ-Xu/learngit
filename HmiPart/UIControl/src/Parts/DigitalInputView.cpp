/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DropMenuControl.cpp
 * Author   : qiaodan
 * Date     : 2020-12-03
 * Descript : �����˵��Ŀ���Դ�룬���������˵����ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "ResourceService.h"
#include "stdafx.h"
#include "DigitalInputView.h"
#include "DigitalInputControl.h"
#include "DigitalInputModel.h"
#include <FL/fl_draw.H>
#include "Message.h"
#include "HMIWindow.h"
#include "UIData.h"
#include "NoticesUtility.h"
#include "KeyInputUtility.h"
#include "OperatorGControl.h"
#include "OperatorStorage.h"
#include "SysSetApi.h"
namespace UI
{

	DigitalInputView::DigitalInputView(int X, int Y, int W, int H, const char* l) : InputView(X, Y, W, H, l) {}
	DigitalInputView::~DigitalInputView() {}

	void DigitalInputView::FoucsNextSequence()
	{
		shared_ptr<DigitalInputModel> model = BaseView.GetModel<DigitalInputModel>();
		shared_ptr<DigitalInputControl> ctrl = BaseView.GetControl<DigitalInputControl>();
		map<int, vector<WinCtrlID>>::iterator  iter;
		iter = model->InputNumConfig.SequenceGroup.find(model->InputNumConfig.Sequence.GroupId);
		if (iter != model->InputNumConfig.SequenceGroup.end())			//��������
		{
			//if (!model->InputNumConfig.Sequence.CancelSequenceAfterCompleted )
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
	bool DigitalInputView::HandleOperatePerm()
	{
		shared_ptr<DigitalInputModel> model = BaseView.GetModel<DigitalInputModel>();
		shared_ptr<DigitalInputControl> ctrl = BaseView.GetControl<DigitalInputControl>();
		if ((model->InputNumConfig.Perm.RequireRegion != 0)
			&& (!UI::PermUtility::HasUserPerm(model->InputNumConfig.Perm.RequireRegion)))		//�û������˰�ȫȨ�޲���û��Ȩ��
		{
			if (model->InputNumConfig.Perm.IsHideElement)			//����ԭ��
			{
				model->InputNumConfig.Perm.HasShowPerm = false;
				hide();
				redraw();
				return false;
			}
			if (model->InputNumConfig.Perm.IsPopwin)				//������ʾ����
			{
				ctrl->Win()->OpenDialogPage(SafeTipsWinNum);
				return false;
			}
		}
		return true;
	}
	bool DigitalInputView::HandleOperateConfirm()
	{
		shared_ptr<DigitalInputModel> model = BaseView.GetModel<DigitalInputModel>();
		shared_ptr<DigitalInputControl> ctrl = BaseView.GetControl<DigitalInputControl>();
		if (!PermUtility::HandleConfirmPerm(model->InputNumConfig.Perm, ctrl->CtrlId()))
			return false;
		else
		{
			EnterValue();
			return true;
		}
	}
	void DigitalInputView::EnterValue()			//ȷ�Ϻ�д��
	{
		shared_ptr<DigitalInputModel> model = BaseView.GetModel<DigitalInputModel>();
		shared_ptr<DigitalInputControl> ctrl = BaseView.GetControl<DigitalInputControl>();

		if (model->InputNumConfig.RwDiffAddrUsed)  //��дʹ�ò�ͬ�Ĵ���
		{
			if (model->InputNumConfig.IsRecord)
				OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->InputNumConfig.CtrlName, Storage::OperatorAction::OA_SET,
					model->InputNumConfig.WriteVar, UIData::Number<double>(model->InputNumConfig.WriteVar), InputValue);

			UI::UIData::Number(model->InputNumConfig.WriteVar, InputValue);
		}
		else
		{
			if (model->InputNumConfig.IsRecord)
				OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->InputNumConfig.CtrlName, Storage::OperatorAction::OA_SET,
					model->InputNumConfig.ReadVar, UIData::Number<double>(model->InputNumConfig.ReadVar), InputValue);
			UI::UIData::Number(model->InputNumConfig.ReadVar, InputValue);
		}
		
		NoticesUtility::NoticeWrite(model->InputNumConfig.ResAft);
		PermUtility::QuitLimitPerm(model->InputNumConfig.Perm);
	}
	void DigitalInputView::InitStartInput()
	{
		shared_ptr<DigitalInputControl> ctrl = BaseView.GetControl<DigitalInputControl>();
		shared_ptr<DigitalInputModel> model = BaseView.GetModel<DigitalInputModel>();
		char dataflag;
		if (model->InputNumConfig.RwDiffAddrUsed)  //��дʹ�ò�ͬ�Ĵ���
			dataflag = DataApi::GetDataFlag(model->InputNumConfig.WriteVar);
		else
			dataflag = DataApi::GetDataFlag(model->InputNumConfig.ReadVar);
		if (dataflag == 0 || !HandleOperatePerm())		//û��Ȩ���򷵻�
			return;	
		/*д��ǰ֪ͨ*/
		NoticesUtility::NoticeWrite(model->InputNumConfig.ResBef);
		/*����Ҫ����focus�¼�*/
		NeedHandleFocus = false;
		OpenKeypage = true;

		/*�򿪼��̴���*/
		if (model->InputNumConfig.KeyMethod.KeypadSrc == 0)	//ʹ�õ�������
			ctrl->Win()->OpenDialogPage(model->InputNumConfig.KeyMethod.Keypad.KeypadIdx, nullptr,
				model->InputNumConfig.KeyMethod.Keypad.KeypadPos.X,
				model->InputNumConfig.KeyMethod.Keypad.KeypadPos.Y);
		//ctrl->Win()->focus(this);
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
		//LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, value());
		/*������ֵ*/
		if (model->InputNumConfig.IsInputConvertUsed)
		{
			MaxValue = model->InputNumConfig.NovUpperSrcInput.Num;
			MinValue = model->InputNumConfig.NovLowerSrcInput.Num;
			LocalData::SetNumberData(SYS_PSW_INPUTKEY_UPPERERLMT, model->InputNumConfig.NovUpperResInput.Num);
			LocalData::SetNumberData(SYS_PSW_INPUTKEY_LOWERLMT, model->InputNumConfig.NovLowerResInput.Num);
		}
		else
		{
			MaxValue = model->InputNumConfig.WarnUpper.Num;
			MinValue = model->InputNumConfig.WarnLower.Num;
			LocalData::SetNumberData(SYS_PSW_INPUTKEY_UPPERERLMT, model->InputNumConfig.WarnUpper.Num);
			LocalData::SetNumberData(SYS_PSW_INPUTKEY_LOWERLMT, model->InputNumConfig.WarnLower.Num);
		}
		redraw();
	}
	void DigitalInputView::EndInput()
	{
		shared_ptr<DigitalInputModel> model = BaseView.GetModel<DigitalInputModel>();
		shared_ptr<DigitalInputControl> ctrl = BaseView.GetControl<DigitalInputControl>();

		/*�رռ���*/
		if (model->InputNumConfig.KeyMethod.KeypadSrc == 0)	//ʹ�õ�������
			ctrl->Win()->ClosePage(model->InputNumConfig.KeyMethod.Keypad.KeypadIdx);
		OpenKeypage = false;
		/*������Ҫ����focus�¼�*/
		NeedHandleFocus = false;	
		Fl::focus(0);
		redraw();
	}
	int DigitalInputView::handle(int event)
	{
		shared_ptr<DigitalInputModel> model = BaseView.GetModel<DigitalInputModel>();
		shared_ptr<DigitalInputControl> ctrl = BaseView.GetControl<DigitalInputControl>();
		
		switch (event)
		{
		case FL_FOCUS:
		{
			if (NeedHandleFocus)							//��ǰ�ؼ��Ƿ���Ҫ����focus�¼�
			{
				if (model->InputNumConfig.KeyMethod.Method)	//λ����ģʽ
				{
					/*���δʹ�ܵ�ǰ����*/
					if (!UI::UIData::Bit(model->InputNumConfig.KeyMethod.KeypadVar) /*||
						!model->InputNumConfig.KeyMethod.KeypadVar.Cmp(BitVid)*/)
					{
						FoucsNextSequence();				//ʹ��һ��������򲿼���ȡ����
						NeedHandleFocus = false;
						return 0;
					}
				}
				InitStartInput();
				return 0;
			}
			return 1;
		}
		case FL_UNFOCUS:
		{
			if (Fl::focus() != this)
				OpenKeypage = false;
			return  HMIBaseInput::handle(event);
		}
		case FL_PUSH:
		{	
			/*�ж�ģʽ*/
			SysSetApi::TriggerBeep();
			if (model->InputNumConfig.KeyMethod.Method)			//λ����ģʽ
			{
				Fl::focus(0);
				return 1;
			}
			InitStartInput();
			return 1;
		}
		case FL_KEYBOARD:
		{
			XJDataType tp;
			if (model->InputNumConfig.RwDiffAddrUsed)  //��дʹ�ò�ͬ�Ĵ���
				tp = UI::UIDataService::Ins().GetDataType(model->InputNumConfig.WriteVar);	
			else
				tp = UI::UIDataService::Ins().GetDataType(model->InputNumConfig.ReadVar);
			/*�����ֵ*/
			int curmark = mark();
			string perValue = value();				//������ǰֵ
			char ascii = Fl::event_text()[0];		//��ȡ�����ֵ
			int del;
			if (tp.Type == Project::NT_Hex && model->InputNumConfig.Accuracy.Act > 14)
				model->InputNumConfig.Accuracy.Act = 14;
			if (Fl::compose(del)) {
				int ip = position() < mark() ? position() : mark();
				if ((!ip && ascii == '-')	|| (ascii >= '0' && ascii <= '9') || (ascii == '.')
					||(tp.Type == Project::NT_Hex && ((ascii >= 'A'&& ascii <= 'F') || (ascii >= 'a'&& ascii <= 'f'))))
				{
					if (ascii == '.')
					{
						if (perValue.find(".") == -1 && model->InputNumConfig.Accuracy.Bct != 0) 	//�жϵ�ǰ�Ƿ���С����
							replace(position(), mark(), &ascii, 1);
					}
					else
					{
						replace(position(), mark(), &ascii, 1);
					}
					string currentValue = value();
					/*��ȡ����λ��С��λ*/
					int pos = currentValue.find(".");
					int act, bct;
					if (pos == 0)	//С������ͷ��
					{
						act = 0;
						bct = (currentValue.substr(pos + 1, currentValue.size())).size();
					}
					else if (pos == -1)
					{
						act = currentValue.size();
						bct = 0;
					}
					else
					{
						act = (currentValue.substr(0, pos)).size();
						bct = (currentValue.substr(pos + 1, currentValue.size())).size();
					}
					/*�Ƿ������ֵ*/
					//if (act > model->InputNumConfig.Accuracy.Act || bct > model->InputNumConfig.Accuracy.Bct
					//	|| stod(currentValue.c_str()) > LocalData::GetNumberData<DOUBLE>(SYS_PSW_INPUTKEY_UPPERERLMT))
					if (currentValue != "-" && currentValue != "." && currentValue != "-.")
					{
						double data;
						if (tp.Type == Project::NT_Hex || tp.Type == Project::NT_BCD)
						{
							if(act <= model->InputNumConfig.Accuracy.Act)
								data = stoull(currentValue, nullptr, 16);
							//data = d;
						}
						else
							data = stod(currentValue.c_str());
						if (act > model->InputNumConfig.Accuracy.Act || bct > model->InputNumConfig.Accuracy.Bct
							|| data > MaxValue )
						{
							value(perValue.c_str());
							mark(curmark);
						}
					}
					else
					{
						if (((currentValue == "-" || currentValue != "-.")&& MinValue < 0 )|| currentValue == "." )
							value(currentValue.c_str());
					}
					int len = LocalData::GetLocalVarLen(SYS_PSW_INPUTKEY_CURVAL);
					char *data = new char[len + 1];
					memset(data, '\0', len + 1);
					memcpy(data, value(), strlen(value()) > len ? len : strlen(value()));
					LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, data);
					delete[] data;
					//LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, value());
				}
				return 1;
			}

			if (Fl::event_key() == FL_Enter || Fl::event_key() == FL_KP_Enter)
			{
				/*��ȡ�û�������ֵ*/
				string inputstring = value();
				if (inputstring.empty() || inputstring == "-" || inputstring == "." || inputstring == "-.")
				{
					EndInput();
					if (!model->InputNumConfig.Sequence.CancelSequenceAfterCompleted)
						FoucsNextSequence();
					return 1;
				}

				if (tp.Type == Project::NT_Hex || tp.Type == Project::NT_BCD)
					InputValue = stoull(inputstring, nullptr, 16);
				else
					InputValue = stod(inputstring.c_str());
				//InputValue = (DOUBLE)stod(inputstring);
				if (model->InputNumConfig.IsInputConvertUsed)
				{
					if (model->InputNumConfig.NovUpperSrcInput.Num == model->InputNumConfig.NovLowerSrcInput.Num)
						InputValue = 0.0;
					else
					{
						InputValue = model->InputNumConfig.NovLowerResInput.Num + (InputValue - model->InputNumConfig.NovLowerSrcInput.Num) *
							(model->InputNumConfig.NovUpperResInput.Num - model->InputNumConfig.NovLowerResInput.Num) /
							(model->InputNumConfig.NovUpperSrcInput.Num - model->InputNumConfig.NovLowerSrcInput.Num);
						if (InputValue > model->InputNumConfig.WarnUpper.Num || 
							InputValue < model->InputNumConfig.WarnLower.Num)
						{
							EndInput();
							if (!model->InputNumConfig.Sequence.CancelSequenceAfterCompleted)
								FoucsNextSequence();
							return 1;
						}
					}
				}
				EndInput();
				/*���������д�Ĵ���*/
				//�����д�Ĵ�����ͬ����ctrl������Ϣ�ı���ʾֵ
				//�����д�Ĵ�������ͬ����ֱ����ʾԭ�����ַ���
				/*д���֪ͨ*/
				HandleOperateConfirm();
					
				/*�����������*/
				if (!model->InputNumConfig.Sequence.CancelSequenceAfterCompleted)
					FoucsNextSequence();
				return 1;
			}
			//if (Fl::event_key() == FL_Tab)	
			//{
			//	std::shared_ptr<BaseControl> nextctrl = ctrl->Page()->FindCtrl(1);
			//	InputView *nextview = (InputView *)nextctrl->GetView();
			//	nextview->OpenKeypage;
			//	Fl::focus(nextview);
			//	return 1;
			//}
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
				memcpy(data, value(), strlen(value()) > len ? len: strlen(value()));
				LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, data);
				delete[] data;
				//LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, value());
				return 1;
			}
			return HMIBaseInput::handle(event);
		}
		default:
			return HMIBaseInput::handle(event);
		}
		return 1;
	}
	void DigitalInputView::draw()
	{
		shared_ptr<DigitalInputControl> ctrl = BaseView.GetControl<DigitalInputControl>();
		shared_ptr<DigitalInputModel> model = BaseView.GetModel<DigitalInputModel>();
		//maximum_size()
		//���ƿ���
		Fl_Image *rgbImage = IResourceService::Ins()->GetImage(model->InputNumConfig.Key.KeyVal);
		if (!rgbImage) {
			LOG_INFO_("Digital Input Pic is NULL\n");
		}
		else {
			image(rgbImage);
			align(FL_ALIGN_IMAGE_BACKDROP);		//���ñ���ͼƬ
			box(FL_NO_BOX);
		}
		/*���û��򿪼���ʱ��ʵʱ��ʾ�û�����ֵ*/
		if (OpenKeypage)
		{
			Fl_Boxtype b = box();
			if (damage() & FL_DAMAGE_ALL) {
				draw_box();
			}
			//string password;
			//if (model->InputNumConfig.PswMode)
			//{
			//	size_t size = strlen(value());
			//	for (size_t i = 0; i < size; i++)
			//	{
			//		password += '*';
			//	}
			//	value(password.c_str());
			//}
			/*��������*/
			textfont(UI::IResourceService::GetFontIdx(model->InputNumConfig.TextStyle.Font.Name));
			textsize(model->InputNumConfig.TextStyle.Font.Size);
			textcolor(fl_rgb_color(RGBColor(model->InputNumConfig.TextStyle.Colors)));
			drawtext(model->InputNumConfig.X + model->InputNumConfig.OffX,
				model->InputNumConfig.Y + model->InputNumConfig.OffY,
				model->InputNumConfig.Width, model->InputNumConfig.Height,
				model->InputNumConfig.TextStyle.Align, rgbImage, model->InputNumConfig.PswMode);
		}
		else   /*�û��رռ��̺���ʾ��ֵ��ReadVar��ȡ*/
		{
			///*������������*/
			//string password;
			//if (model->InputNumConfig.PswMode)
			//{
			//	size_t size = DisplayValue.size();
			//	for (size_t i = 0; i < size; i++)
			//	{
			//		password += '*';
			//	}
			//}

			Fl_Boxtype b = box();
			if (damage() & FL_DAMAGE_ALL) {
				draw_box();
			}
			/*��������*/
			textfont(UI::IResourceService::GetFontIdx(model->InputNumConfig.TextStyle.Font.Name));
			textsize(model->InputNumConfig.TextStyle.Font.Size);
			/*����������ɫ������*/
			Fl_Color tcolor;
			value(DisplayValue.c_str());
			if (model->InputNumConfig.IsWarnStyleUsed)
			{
				if (IsExceedMaxLmt)		//�������ֵ
					tcolor = fl_rgb_color(RGBColor(model->InputNumConfig.WarnStyleUpper.Color));
				else if (IsExceedMinLmt)	//������Сֵ
					tcolor = fl_rgb_color(RGBColor(model->InputNumConfig.WarnStyleLower.Color));
				else
					tcolor = fl_rgb_color(RGBColor(model->InputNumConfig.TextStyle.Colors));
			}
			else
				tcolor = fl_rgb_color(RGBColor(model->InputNumConfig.TextStyle.Colors));
			textcolor(tcolor);
			drawtext(model->InputNumConfig.X + model->InputNumConfig.OffX,
				model->InputNumConfig.Y + model->InputNumConfig.OffY,
				model->InputNumConfig.Width, model->InputNumConfig.Height,
				model->InputNumConfig.TextStyle.Align, rgbImage, model->InputNumConfig.PswMode, IsHideText);
		}
	}
}
