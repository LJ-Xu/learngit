/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DropMenuControl.cpp
 * Author   : qiaodan
 * Date     : 2020-12-03
 * Descript : 下拉菜单的控制源码，创建下拉菜单绘制，传递处理消息
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
		if (iter != model->InputNumConfig.SequenceGroup.end())			//存在数据
		{
			//if (!model->InputNumConfig.Sequence.CancelSequenceAfterCompleted )
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
	bool DigitalInputView::HandleOperatePerm()
	{
		shared_ptr<DigitalInputModel> model = BaseView.GetModel<DigitalInputModel>();
		shared_ptr<DigitalInputControl> ctrl = BaseView.GetControl<DigitalInputControl>();
		if ((model->InputNumConfig.Perm.RequireRegion != 0)
			&& (!UI::PermUtility::HasUserPerm(model->InputNumConfig.Perm.RequireRegion)))		//用户设置了安全权限并且没有权限
		{
			if (model->InputNumConfig.Perm.IsHideElement)			//隐藏原件
			{
				model->InputNumConfig.Perm.HasShowPerm = false;
				hide();
				redraw();
				return false;
			}
			if (model->InputNumConfig.Perm.IsPopwin)				//弹出提示窗口
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
	void DigitalInputView::EnterValue()			//确认后写入
	{
		shared_ptr<DigitalInputModel> model = BaseView.GetModel<DigitalInputModel>();
		shared_ptr<DigitalInputControl> ctrl = BaseView.GetControl<DigitalInputControl>();

		if (model->InputNumConfig.RwDiffAddrUsed)  //读写使用不同寄存器
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
		if (model->InputNumConfig.RwDiffAddrUsed)  //读写使用不同寄存器
			dataflag = DataApi::GetDataFlag(model->InputNumConfig.WriteVar);
		else
			dataflag = DataApi::GetDataFlag(model->InputNumConfig.ReadVar);
		if (dataflag == 0 || !HandleOperatePerm())		//没有权限则返回
			return;	
		/*写入前通知*/
		NoticesUtility::NoticeWrite(model->InputNumConfig.ResBef);
		/*不需要处理focus事件*/
		NeedHandleFocus = false;
		OpenKeypage = true;

		/*打开键盘窗口*/
		if (model->InputNumConfig.KeyMethod.KeypadSrc == 0)	//使用弹出键盘
			ctrl->Win()->OpenDialogPage(model->InputNumConfig.KeyMethod.Keypad.KeypadIdx, nullptr,
				model->InputNumConfig.KeyMethod.Keypad.KeypadPos.X,
				model->InputNumConfig.KeyMethod.Keypad.KeypadPos.Y);
		//ctrl->Win()->focus(this);
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
		//LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, value());
		/*设置限值*/
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

		/*关闭键盘*/
		if (model->InputNumConfig.KeyMethod.KeypadSrc == 0)	//使用弹出键盘
			ctrl->Win()->ClosePage(model->InputNumConfig.KeyMethod.Keypad.KeypadIdx);
		OpenKeypage = false;
		/*不在需要处理focus事件*/
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
			if (NeedHandleFocus)							//当前控件是否需要处理focus事件
			{
				if (model->InputNumConfig.KeyMethod.Method)	//位控制模式
				{
					/*如果未使能当前输入*/
					if (!UI::UIData::Bit(model->InputNumConfig.KeyMethod.KeypadVar) /*||
						!model->InputNumConfig.KeyMethod.KeypadVar.Cmp(BitVid)*/)
					{
						FoucsNextSequence();				//使下一个输入次序部件获取焦点
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
			/*判断模式*/
			SysSetApi::TriggerBeep();
			if (model->InputNumConfig.KeyMethod.Method)			//位控制模式
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
			if (model->InputNumConfig.RwDiffAddrUsed)  //读写使用不同寄存器
				tp = UI::UIDataService::Ins().GetDataType(model->InputNumConfig.WriteVar);	
			else
				tp = UI::UIDataService::Ins().GetDataType(model->InputNumConfig.ReadVar);
			/*处理键值*/
			int curmark = mark();
			string perValue = value();				//保留当前值
			char ascii = Fl::event_text()[0];		//获取输入键值
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
						if (perValue.find(".") == -1 && model->InputNumConfig.Accuracy.Bct != 0) 	//判断当前是否含有小数点
							replace(position(), mark(), &ascii, 1);
					}
					else
					{
						replace(position(), mark(), &ascii, 1);
					}
					string currentValue = value();
					/*获取整数位和小数位*/
					int pos = currentValue.find(".");
					int act, bct;
					if (pos == 0)	//小数点在头部
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
					/*是否符合限值*/
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
				/*获取用户输入数值*/
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
				/*设置输入框写寄存器*/
				//如果读写寄存器相同，则ctrl接受消息改变显示值
				//如果读写寄存器不相同，则直接显示原来的字符串
				/*写入后通知*/
				HandleOperateConfirm();
					
				/*处理输入次序*/
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
			if (Fl::event_key() == FL_Escape)	//退出按键
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
			if (Fl::event_key() == FL_BackSpace)	//删除按键
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
		//绘制框体
		Fl_Image *rgbImage = IResourceService::Ins()->GetImage(model->InputNumConfig.Key.KeyVal);
		if (!rgbImage) {
			LOG_INFO_("Digital Input Pic is NULL\n");
		}
		else {
			image(rgbImage);
			align(FL_ALIGN_IMAGE_BACKDROP);		//设置背景图片
			box(FL_NO_BOX);
		}
		/*当用户打开键盘时，实时显示用户输入值*/
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
			/*设置字体*/
			textfont(UI::IResourceService::GetFontIdx(model->InputNumConfig.TextStyle.Font.Name));
			textsize(model->InputNumConfig.TextStyle.Font.Size);
			textcolor(fl_rgb_color(RGBColor(model->InputNumConfig.TextStyle.Colors)));
			drawtext(model->InputNumConfig.X + model->InputNumConfig.OffX,
				model->InputNumConfig.Y + model->InputNumConfig.OffY,
				model->InputNumConfig.Width, model->InputNumConfig.Height,
				model->InputNumConfig.TextStyle.Align, rgbImage, model->InputNumConfig.PswMode);
		}
		else   /*用户关闭键盘后，显示数值由ReadVar获取*/
		{
			///*设置密码内容*/
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
			/*设置字体*/
			textfont(UI::IResourceService::GetFontIdx(model->InputNumConfig.TextStyle.Font.Name));
			textsize(model->InputNumConfig.TextStyle.Font.Size);
			/*设置字体颜色和文字*/
			Fl_Color tcolor;
			value(DisplayValue.c_str());
			if (model->InputNumConfig.IsWarnStyleUsed)
			{
				if (IsExceedMaxLmt)		//超出最大值
					tcolor = fl_rgb_color(RGBColor(model->InputNumConfig.WarnStyleUpper.Color));
				else if (IsExceedMinLmt)	//低于最小值
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
