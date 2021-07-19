/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ChineseInputView.cpp
 * Author   : qiaodan
 * Date     : 2020-10-19
 * Descript : 绘制中文显示框和文字、背景图片
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "ChineseInputView.h"
#include "ChineseInputControl.h"
#include "ChineseInputModel.h"
#include <FL/Fl_Input_.H>
#include "ResourceService.h"
#include "NoticesUtility.h"
#include "Message.h"
#include "UIData.h"
#include "LocalData.h"
#include "HMIWindow.h"
#include "CodeFormatUtility.h"
#include "KeyInputUtility.h"
#include "PinYinPageView.h"
#include "PermUtility.h"
#include "OperatorGControl.h"
#include "SysSetApi.h"
namespace UI
{
	ChineseInputView::ChineseInputView(int X, int Y, int W, int H, const char* l) : InputView(X, Y, W, H, l) {}
	ChineseInputView::~ChineseInputView() {}

	void ChineseInputView::draw()
	{
		shared_ptr<ChineseInputModel> model = BaseView.GetModel<ChineseInputModel>();

		Fl_Image *rgbImage = IResourceService::Ins()->GetImage(model->InputChineseConfig.Key.KeyVal);
		if (!rgbImage) 			
			LOG_INFO_("Chinese Input Pic is NULL\n");
		else {
			image(rgbImage);
			align(FL_ALIGN_IMAGE_BACKDROP);		//设置背景图片
			box(FL_NO_BOX);
		}
		if (OpenKeypage)
		{
			Fl_Boxtype b = box();
			if (damage() & FL_DAMAGE_ALL) {
				draw_box();
			}
			
			/*设置字体*/
			textfont(UI::IResourceService::GetFontIdx(model->InputChineseConfig.TextStyle.Font.Name));
			textsize(model->InputChineseConfig.TextStyle.Font.Size);
			textcolor(fl_rgb_color(RGBColor(model->InputChineseConfig.TextStyle.Colors)));
			drawtext(model->InputChineseConfig.X + model->InputChineseConfig.OffX,
				model->InputChineseConfig.Y + model->InputChineseConfig.OffY,
				model->InputChineseConfig.Width, model->InputChineseConfig.Height,
				model->InputChineseConfig.TextStyle.Align, rgbImage, model->InputChineseConfig.PswMode);
		}
		else
		{
			value(DisplayValue.c_str());
			Fl_Boxtype b = box();
			if (damage() & FL_DAMAGE_ALL) {
				draw_box();
			}
			/*设置字体*/
			textfont(UI::IResourceService::GetFontIdx(model->InputChineseConfig.TextStyle.Font.Name));
			textsize(model->InputChineseConfig.TextStyle.Font.Size);
			/*设置字体颜色和文字*/
			Fl_Color tcolor = fl_rgb_color(RGBColor(model->InputChineseConfig.TextStyle.Colors));
			textcolor(tcolor);
			drawtext(model->InputChineseConfig.X + model->InputChineseConfig.OffX,
				model->InputChineseConfig.Y + model->InputChineseConfig.OffY,
				model->InputChineseConfig.Width, model->InputChineseConfig.Height,
				model->InputChineseConfig.TextStyle.Align, rgbImage, model->InputChineseConfig.PswMode);
		}
	}
	
	void ChineseInputView::FoucsNextSequence()
	{
		shared_ptr<ChineseInputModel> model = BaseView.GetModel<ChineseInputModel>();
		shared_ptr<ChineseInputControl> ctrl = BaseView.GetControl<ChineseInputControl>();
		map<int, vector<WinCtrlID>>::iterator  iter;
		iter = model->InputChineseConfig.SequenceGroup.find(model->InputChineseConfig.Sequence.GroupId);
		if (iter != model->InputChineseConfig.SequenceGroup.end())			//存在数据
		{
			//if (!model->InputChineseConfig.Sequence.CancelSequenceAfterCompleted )
			//{
			for (size_t i = 0; i < iter->second.size(); i++)
			{
				if (iter->second[i] == ctrl->CtrlId())
				{
					if (i == iter->second.size() - 1)			//该组最后一个控件，循环
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
	bool ChineseInputView::HandleOperatePerm()
	{
		shared_ptr<ChineseInputModel> model = BaseView.GetModel<ChineseInputModel>();
		shared_ptr<ChineseInputControl> ctrl = BaseView.GetControl<ChineseInputControl>();
		if ((model->InputChineseConfig.Perm.RequireRegion != 0)
			&& (!UI::PermUtility::HasUserPerm(model->InputChineseConfig.Perm.RequireRegion)))		//用户设置了安全权限并且没有权限
		{
			if (model->InputChineseConfig.Perm.IsHideElement)			//隐藏原件
			{
				model->InputChineseConfig.Perm.HasShowPerm = false;
				hide();
				redraw();
				return false;
			}
			if (model->InputChineseConfig.Perm.IsPopwin)				//弹出提示窗口
			{
				ctrl->Win()->OpenDialogPage(SafeTipsWinNum);
				return false;
			}
		}
		return true;
	}
	bool ChineseInputView::HandleOperateConfirm()
	{
		shared_ptr<ChineseInputModel> model = BaseView.GetModel<ChineseInputModel>();
		shared_ptr<ChineseInputControl> ctrl = BaseView.GetControl<ChineseInputControl>();
		if (!PermUtility::HandleConfirmPerm(model->InputChineseConfig.Perm, ctrl->CtrlId()))
			return false;
		else
		{
			EnterValue();
			return true;
		}
	}
	void ChineseInputView::EnterValue()			//确认后写入
	{
		shared_ptr<ChineseInputModel> model = BaseView.GetModel<ChineseInputModel>();
		shared_ptr<ChineseInputControl> ctrl = BaseView.GetControl<ChineseInputControl>();

		int count = UIDataService::Ins().GetDataCounts(model->InputChineseConfig.WriteVar);
		char *buf = new char[count + 1];
		memset(buf, '\0', count + 1);
		memcpy(buf, InputString.c_str(), InputString.size());
		/*设置输入框写寄存器*/
		//if (model->InputChineseConfig.Rule == CodeFormatUtility::GB2312)
		//	CodeFormatUtility::Utf8ToCode(model->InputChineseConfig.Rule, buf, count);
		CodeFormatUtility::Utf8ToCode(CodeFormatUtility::GB2312, buf, count);
		if (model->InputChineseConfig.IsRecord)
		{
			int count = UIDataService::Ins().GetDataCounts(model->InputChineseConfig.WriteVar);
			char *prebuf = new char[count + 1];
			memset(prebuf, '\0', count + 1);
			UIDataService::Ins().GetString(model->InputChineseConfig.WriteVar, prebuf, count);
			OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->InputChineseConfig.CtrlName,
				model->InputChineseConfig.WriteVar, prebuf, buf);
			delete[] prebuf;
		}

		DataApi::SetDataBytes(model->InputChineseConfig.WriteVar, buf, count);
		NoticesUtility::NoticeWrite(model->InputChineseConfig.ResAft);
		PermUtility::QuitLimitPerm(model->InputChineseConfig.Perm);
	}
	void ChineseInputView::InitStartInput()
	{
		shared_ptr<ChineseInputControl> ctrl = BaseView.GetControl<ChineseInputControl>();
		shared_ptr<ChineseInputModel> model = BaseView.GetModel<ChineseInputModel>();
		char dataflag = DataApi::GetDataFlag(model->InputChineseConfig.WriteVar);
		if (dataflag == 0 || !HandleOperatePerm())		//没有权限则返回
			return;
		/*写入前通知*/
		NoticesUtility::NoticeWrite(model->InputChineseConfig.ResBef);
		/*不需要处理focus事件*/
		NeedHandleFocus = false;
		OpenKeypage = true;
		InputString = "";
		PinYin = "";
		/*打开键盘窗口*/
		if (model->InputChineseConfig.KeyMethod.KeypadSrc == 0)	//使用弹出键盘
		{
			ctrl->Win()->OpenDialogPage(model->InputChineseConfig.KeyMethod.Keypad.KeypadIdx,
				nullptr, model->InputChineseConfig.KeyMethod.Keypad.KeypadPos.X,
				model->InputChineseConfig.KeyMethod.Keypad.KeypadPos.Y);
			//if (!KeyInputUtility::IsChinese())
			//	KeyInputUtility::SetChineseState();		//设置为中文
		}
		if (Fl::focus() != this) {
			Fl::focus(this);
		}
		mark(0);			//选中所有数据
		/*设置当前值*/
		int len = LocalData::GetLocalVarLen(SYS_PSW_INPUTKEY_CURVAL);
		char *data = new char[len + 1];
		memset(data, '\0', len + 1);
		memcpy(data, value(), strlen(value()) > len ? len : strlen(value()));
		LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, data);
		delete[] data;
		redraw();
	}
	void ChineseInputView::EndInput()
	{
		shared_ptr<ChineseInputModel> model = BaseView.GetModel<ChineseInputModel>();
		shared_ptr<ChineseInputControl> ctrl = BaseView.GetControl<ChineseInputControl>();

		/*关闭键盘*/
		if (model->InputChineseConfig.KeyMethod.KeypadSrc == 0)	//使用弹出键盘
		{
			ctrl->Win()->ClosePage(model->InputChineseConfig.KeyMethod.Keypad.KeypadIdx);
			ctrl->Win()->ClosePage(SYS_PINYIN_PAGENUM);
		}
		OpenKeypage = false;
		HasPinYinpage = false;
		/*不在需要处理focus事件*/
		NeedHandleFocus = false;
		Fl::focus(0);
		redraw();
	}
	int ChineseInputView::handle(int event)
	{
		shared_ptr<ChineseInputModel> model = BaseView.GetModel<ChineseInputModel>();
		shared_ptr<ChineseInputControl> ctrl = BaseView.GetControl<ChineseInputControl>();
		
		switch (event)
 		{
		case FL_FOCUS:
		{
			if (NeedHandleFocus)							//当前控件是否需要处理focus事件
			{
				if (model->InputChineseConfig.KeyMethod.Method)	//位控制模式
				{
					/*如果未使能当前输入*/
					if (!UI::UIData::Bit(model->InputChineseConfig.KeyMethod.KeypadVar))
					{
						FoucsNextSequence();				//使下一个输入次序部件获取焦点
						NeedHandleFocus = false;
						return 0;
					}
				}
				InitStartInput();
				return 1;
			}
			else if (OpenKeypage)
			{
				Fl::focus(this);
			}
			return 1;
		}
		case FL_UNFOCUS:
		{
			if (Fl::focus() != this && model->InputChineseConfig.KeyMethod.KeypadSrc)
				OpenKeypage = false;
			return  HMIBaseInput::handle(event);
		}
		case FL_PUSH:
		{
			/*判断模式*/
			SysSetApi::TriggerBeep();
			if (model->InputChineseConfig.KeyMethod.Method)			//位控制模式
			{
				Fl::focus(0);
				return 1;
			}
			InitStartInput();

			return 1;
		}
		case FL_KEYBOARD:
		{
			/*处理键值*/
			int curmark = mark();
			string perValue = value();				//保留当前值
			int maxsize = UIDataService::Ins().GetDataCounts(model->InputChineseConfig.WriteVar);
			char ascii = Fl::event_text()[0];		//获取输入键值
			int del;
			if (Fl::compose(del)) {
				if ((ascii >= 'A'&& ascii <= 'Z') || (ascii >= 'a'&& ascii <= 'z'))
				{
					if (KeyInputUtility::IsUpper() && (ascii >= 'a'&& ascii <= 'z'))		//大写
						ascii -= 32;
					if (!KeyInputUtility::IsUpper() && (ascii >= 'A'&& ascii <= 'Z'))		//小写
						ascii += 32;
					if (!KeyInputUtility::IsChinese())		//不是中文
					{
						if (!mark())
							InputString = ascii;
						else
							InputString += ascii;
						
						//int m = mark();
						//char inputchar[32] = { 0 };
						//LocalData::GetBytes(SYS_PSW_INPUTKEY_CURVAL, inputchar);
						//string inputstring = inputchar;
						//inputstring += ascii;
						//char *data = new char[32 + 1];
						//memset(data, '\0', 33);
						//memcpy(data, inputstring.c_str(), inputstring.size());
						//LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, data);
						//delete[] data;
						////LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, inputstring.c_str());
						//if (!UI::CodeFormatUtility::IsStrUtf8(inputstring.c_str()))
						//	UI::IResourceService::GB2312toUtf8(inputstring);
						//value(inputstring.c_str());

					}
					if (!HasPinYinpage && KeyInputUtility::IsChinese())
					{
						ctrl->Win()->OpenDialogPage(SYS_PINYIN_PAGENUM, nullptr, 0,0);
						HasPinYinpage = true;
						PinYin.clear();
					}
					/*else
					{
						char pinyin[32] = { 0 };
						LocalData::GetBytes(SYS_PSW_PINYIN_CURVAL, pinyin);
						PinYin = pinyin;
					}*/
					//设置拼音字符串
					if (HasPinYinpage)
					{
						PinYin += ascii;
						LocalData::SetString(SYS_PSW_PINYIN_CURVAL, PinYin.c_str());
					}
				}
				else if ((ascii >= '0' && ascii <= '9') && HasPinYinpage)
				{
					if (PinYinPageView::Ins()->ChineseString.size() > (size_t)(ascii - '1'))
					{
						string china = PinYinPageView::Ins()->ChineseString[ascii - '1'];
						if (!mark())
							InputString = china;
						else
							InputString += china;

						//char inputchar[32] = { 0 };
						//LocalData::GetBytes(SYS_PSW_INPUTKEY_CURVAL, inputchar);
						//string inputstring = inputchar;
						//inputstring += PinYinPageView::Ins()->ChineseString[ascii - '1'];
						//char *data = new char[32 + 1];
						//memset(data, '\0', 33);
						//memcpy(data, inputstring.c_str(), inputstring.size());
						//LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, data);
						//delete[] data;
						////LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, inputstring.c_str());
						//if (!UI::CodeFormatUtility::IsStrUtf8(inputstring.c_str()))
						//	UI::IResourceService::GB2312toUtf8(inputstring);
						//value(inputstring.c_str());
					}
					HasPinYinpage = false;
					ctrl->Win()->ClosePage(SYS_PINYIN_PAGENUM);
					PinYin.clear();
					redraw();
				}
				else
				{
					if (!KeyInputUtility::IsChinese())		//不是中文
					{
						if (!mark())
							InputString = ascii;
						else
							InputString += ascii;
						//char inputchar[32] = { 0 };
						//LocalData::GetBytes(SYS_PSW_INPUTKEY_CURVAL, inputchar);
						//string inputstring = inputchar;
						//inputstring += ascii;
						//char *data = new char[32 + 1];
						//memset(data, '\0', 33);
						//memcpy(data, inputstring.c_str(), sizeof(data));
						//LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, data);
						//delete[] data;
						////LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, inputstring.c_str());
						//if (!UI::CodeFormatUtility::IsStrUtf8(inputstring.c_str()))
						//	UI::IResourceService::GB2312toUtf8(inputstring);
						//value(inputstring.c_str());
					}
				}

				//int count = UIDataService::Ins().GetDataCounts(model->InputChineseConfig.WriteVar);
				char *buf = new char[InputString.size() + 1];
				memset(buf, '\0', InputString.size() + 1);
				memcpy(buf, InputString.c_str(), InputString.size());
				/*设置输入框写寄存器*/
				//if (model->InputChineseConfig.Rule == CodeFormatUtility::GB2312)
				//	CodeFormatUtility::Utf8ToCode(model->InputChineseConfig.Rule, buf, count);
				CodeFormatUtility::Utf8ToCode(CodeFormatUtility::GB2312, buf, InputString.size());
				string tmpstring = buf;
				if (tmpstring.size() > (size_t)maxsize)
					InputString = perValue;

				//if (InputString.size() > (size_t)maxsize)
					//InputString = perValue;
				if (!UI::CodeFormatUtility::IsStrUtf8(InputString.c_str()))
					UI::IResourceService::GB2312toUtf8(InputString);
				value(InputString.c_str());
				int len = LocalData::GetLocalVarLen(SYS_PSW_INPUTKEY_CURVAL);
				char *data = new char[len + 1];
				memset(data, '\0', len + 1);
				memcpy(data, InputString.c_str(), InputString.size() > len ? len : InputString.size());
				LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, data);
				delete[] data;
				return 1;
			}
			if (Fl::event_key() == FL_Enter || Fl::event_key() == FL_KP_Enter)
			{
				/*获取中文*/
				InputString = value();
				EndInput();
				HandleOperateConfirm();
				/*处理输入次序*/
				if (!model->InputChineseConfig.Sequence.CancelSequenceAfterCompleted)
					FoucsNextSequence();
				return 1;
			}
			if (Fl::event_key() == FL_Escape)	//退出按键
			{
				EndInput();
				return 1;
			}
			if (Fl::event_key() == FL_BackSpace)	//删除按键
			{
				if (!PinYin.empty())
				{
					PinYin.pop_back();
					LocalData::SetString(SYS_PSW_PINYIN_CURVAL, PinYin.c_str());
					if (PinYin.empty())
					{
						HasPinYinpage = false;
						ctrl->Win()->ClosePage(SYS_PINYIN_PAGENUM);
					}
				}
				else
				{
					//char inputchar[32] = { 0 };
					//LocalData::GetBytes(SYS_PSW_INPUTKEY_CURVAL, inputchar);
					//string inputstring = inputchar;
					//if (!UI::CodeFormatUtility::IsStrUtf8(inputstring.c_str()))
					//	UI::IResourceService::GB2312toUtf8(inputstring);
					//value(inputstring.c_str());
					cut(-1);
					InputString = value();
					int len = LocalData::GetLocalVarLen(SYS_PSW_INPUTKEY_CURVAL);
					char *data = new char[len + 1];
					memset(data, '\0', len + 1);
					memcpy(data, InputString.c_str(), InputString.size() > len ? len : InputString.size());
					LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, data);
					delete[] data;
					//LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, inputstring.c_str());
				}
				return 1;
			}
			if (Fl::event_key() == FL_Shift_L || Fl::event_key() == FL_Shift_R)	//Shift按键，切换中英文
			{
				KeyInputUtility::SetChineseState();
				if (HasPinYinpage)
				{
					HasPinYinpage = false;
					ctrl->Win()->ClosePage(SYS_PINYIN_PAGENUM);
					PinYin.clear();
				}
				return 1;
			}
			if (Fl::event_key() == FL_Caps_Lock)								//caps按键，切换大小写
			{
				KeyInputUtility::SetUpperState();
				return 1;
			}
			return 1;

			//return HMIBaseInput::handle(event);
		}
		default:
			return 1;

			//HMIBaseInput::handle(event);
		}
		return 1;
	}
}
