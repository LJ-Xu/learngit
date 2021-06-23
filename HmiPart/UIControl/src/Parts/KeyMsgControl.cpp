/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeyMsgControl.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 创建按键消息绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "KeyMsgControl.h"
#include "KeyMsgView.h"
#include "KeyMsgModel.h"
#include "HMIWindow.h"
#include "Message.h"
#include "HMIProject.h"
#include "PinYinWindow.h"
using namespace DataService;
namespace UI
{
	KeyMsgControl::KeyMsgControl(HMIPage* w) : BaseControl(w)
	{
		//创建KeyMsgModel
		mode_ = std::shared_ptr <KeyMsgModel>(new KeyMsgModel());
		InitMVCModel(mode_);
	}

	void KeyMsgControl::CreateView()
	{
		//初始化VIEW
		KeyMsgView* view  =  new KeyMsgView(
			mode_->KeyMsgConfig.X + mode_->KeyMsgConfig.OffX,
			mode_->KeyMsgConfig.Y + mode_->KeyMsgConfig.OffY,
			mode_->KeyMsgConfig.Width,
			mode_->KeyMsgConfig.Height, nullptr);
		InitMVCView(view);
	}

	void KeyMsgControl::AddVarData(vector<Project::DataVarId*> vars)
	{

	}


	void KeyMsgControl::OpenKeyWindow(int lparam)
	{
		UI::HMIWindow *window, *srcWin;
		Project::HMIProject *prj;
		KeyMessage *keymsg = (KeyMessage *)lparam;		//获取消息
		CurrentMsg = *keymsg;
		//prj = Win()->WinMgr()->Project();
 

		/*prj->Pages.Screens[keymsg->DstWinNo].Pos.X = keymsg->KeypadPosx;
		prj->Pages.Screens[keymsg->DstWinNo].Pos.Y = keymsg->KeypadPosy;*/
		//srcWin = Win()->WinMgr()->GetWin(Project::GetWinNo(keymsg->SrcWinNo));
		/*if (srcWin)
		{
			prj->Pages.Screens[keymsg->DstWinNo].Pos.X += srcWin->x();
			prj->Pages.Screens[keymsg->DstWinNo].Pos.Y += srcWin->y();
		}
*/
		//window = Win()->WinMgr()->OpenWindow(keymsg->DstWinNo);

		//window = UI::UIMain::Ins().Test_OpenWindow(2);		//测试使用
		//keymsg->DstCtrlId = window->KeyboardId();		//获取创建窗体的键盘控件
		/*window->set_modal();
		window->show();
		Win()->SendAutoFreeMessage<KeyMessage>((void*)keymsg->DstCtrlId, KEY_EVENT_KEYMSG, keymsg);*/
		//delete keymsg;
		//m_pWindow->SendUserMessage((void*)keymsg->DstCtrlId, KEY_EVENT_KEYMSG, (int)keymsg);	//将消息信息发送给新的键盘控件
	}

	void KeyMsgControl::InitKeyMsg(int lparam)
	{
		//CurrentMsg = new KeyMessage();
		KeyMessage *keymsg = (KeyMessage *)lparam;	//初始化键盘窗口/输入法的keymsg
		CurrentMsg = *keymsg;
		//获取中文值
		//SetCurrentMsg(CurrentMsg ,keymsg);
		//CurrentMsg = (struct KeyMessage *)msg->LParam;		//获取消息
		//PSW::Sys().SetNumberData(LOCAL_SYS_INPUT_UPPERLMT, CurrentMsg.UpperLmt);//设置上限寄存器数值
		//PSW::Sys().SetNumberData(LOCAL_SYS_INPUT_LOWERLMT, CurrentMsg.LowerLmt);//设置下限寄存器数值
		LocalData::SetNumberData(SYS_PSW_INPUT_NUMBER_LOWERLMT, CurrentMsg.UpperLmt);
		LocalData::SetNumberData(SYS_PSW_INPUT_NUMBER_UPPERERLMT, CurrentMsg.UpperLmt);

		//SPSW::Ins().SetNumberData(LOCAL_SYS_INPUT_LOWERLMT, CurrentMsg.LowerLmt);
		//Local_Input_Value_Set(CurrentMsg.Value.c_str());	//设置显示寄存器数值
		if (CurrentMsg.InputType != 2)
			CurrentMsg.Value.clear();						//清除value值
	}

	void KeyMsgControl::HandleKeyChar(int lparam)
	{
		char data[128];
		switch (CurrentMsg.InputType)
		{
		case 0:				//数值
			break;
		case 1:				//字符
			//if (!CurrentMsg.Value.empty())
			//	CurrentMsg.Value = Local_Input_Value_Get(data);
			if (strlen(CurrentMsg.Value.c_str()) < strlen(CurrentMsg.FmtStr.c_str()))		//当字符个数没达到格式限值，可以输入
				CurrentMsg.Value += (char)lparam;				//获取到当前按键输入值
			Win()->SendUserMessage((void*)CurrentMsg.SrcCtrlId, KEY_EVENT_VALUE, (int)&CurrentMsg);	//发送消息给源ctrl
			//Local_Input_Value_Set(CurrentMsg.Value.c_str());	//设置显示寄存器数值
			break;
		case 2:				//中文
			UI::PinYinWindow *win;
			if (CurrentMsg.Value.empty())
			{
				//win = (UI::PinYinWindow *) Win()->WinMgr()->OpenWindow(PinYinWindow::GetPinYinWindow());
 
				/*win->show();*/
				CurrentMsg.Value = (char)lparam;
				//CurrentMsg.PinYinCtrlId = win->KeyboardId();

				Win()->SendAutoFreeMessage<KeyMessage>((void*)CurrentMsg.PinYinCtrlId, KEY_EVENT_KEYMSG, &CurrentMsg);
				Win()->SendAutoFreeMessage<KeyMessage>((void*)CurrentMsg.PinYinCtrlId, KEY_EVENT_PINYIN, &CurrentMsg);
			}
			else if (CurrentMsg.Value.size() < 7)
			{
				UI::HMIWindow *window;
				//window = Win()->WinMgr()->GetWinByCtrID(CurrentMsg.PinYinCtrlId);
				//win = (UI::PinYinWindow *)window;
				CurrentMsg.Value += (char)lparam;
				Win()->SendAutoFreeMessage<KeyMessage>((void*)CurrentMsg.PinYinCtrlId, KEY_EVENT_PINYIN, &CurrentMsg);
			}
			break;
		default:
			break;
		}
	}

	void KeyMsgControl::HandleKeyMessage(int keyValue)
	{
		char data[128];
		switch (keyValue)
		{
		case KEY_DEL:
			switch (CurrentMsg.InputType)
			{
			case 0:
				if (CurrentMsg.Value.length() != 0) {
					CurrentMsg.Value.erase(CurrentMsg.Value.end() - 1);
					Win()->SendUserMessage((void*)CurrentMsg.InputCtrlId, KEY_EVENT_VALUE, (int)&CurrentMsg);
					//Local_Input_Value_Set(CurrentMsg.Value.c_str());	//设置显示寄存器数值
				}
				break;
			case 1:
				if (CurrentMsg.Value.length() != 0) {
					CurrentMsg.Value.erase(CurrentMsg.Value.end() - 1);
					Win()->SendUserMessage((void*)CurrentMsg.InputCtrlId, KEY_EVENT_VALUE, (int)&CurrentMsg);
					//Local_Input_Value_Set(CurrentMsg.Value.c_str());	//设置显示寄存器数值
				}
				break;
			case 2:
				if (CurrentMsg.Value.empty())		//当前拼音为空，则删除字符
				{
					//	CurrentMsg.Chinese.erase(CurrentMsg.Chinese.end() - 3);
					CurrentMsg.Chinese = CurrentMsg.Chinese.substr(0, CurrentMsg.Chinese.size() - 3);
					Win()->SendUserMessage((void*)CurrentMsg.InputCtrlId, KEY_EVENT_VALUE, (int)&CurrentMsg);
					//Local_Input_Value_Set(CurrentMsg.Chinese.c_str());	//设置显示寄存器数值
				}
				else
				{
					UI::PinYinWindow *win;
					UI::HMIWindow *window;
					//window = Win()->WinMgr()->GetWinByCtrID(CurrentMsg.PinYinCtrlId);
					//win = (UI::PinYinWindow *)window;
					CurrentMsg.Value.erase(CurrentMsg.Value.end() - 1);
					if (CurrentMsg.Value.empty())
						//关闭拼音窗口
						;//Win()->WinMgr()->CloseWindow(Project::GetWinNo(CurrentMsg.PinYinCtrlId));
					else
					{
						//win->PinYin = CurrentMsg.Value;
						//win->SetPinYinChanged(CurrentMsg.Value);
						////搜索新的拼音
						//win->SearchChineseCharacters(0, CurrentMsg.Value);
					}
				}
				break;
			default:
				break;
			}
			break;
		case KEY_ESC:
			Win()->SendAutoFreeMessage<KeyMessage>((void*)CurrentMsg.InputCtrlId, KEY_EVENT_ESC, &CurrentMsg);
			//关闭键盘窗口
			//m_pWindow->hide();
			break;
		case KEY_CAPS:
			break;
		case KEY_ENTER:
			//CurrentMsg.Value = Local_Input_Value_Get(data);
			Win()->SendAutoFreeMessage<KeyMessage>((void*)CurrentMsg.InputCtrlId, KEY_EVENT_ENTER, &CurrentMsg);

			
			//m_pWindow->SendUserMessage((void*)currentMsg->inputCtrlId, KEY_EVENT_ENTER, (int)currentMsg);	//发送消息给源ctrl
			break;
		case KEY_MINUS:
			break;
		case KEY_CLR:
			CurrentMsg.Value.clear();							//将数值清除
			//Local_Input_Value_Set(CurrentMsg.Value.c_str());	//设置显示寄存器数值
			Win()->SendUserMessage((void*)CurrentMsg.SrcCtrlId, KEY_EVENT_VALUE, (int)&CurrentMsg);	//发送消息给源ctrl
			break;
		case KEY_DOT:
			//if (!CurrentMsg.Value.empty())
			//	CurrentMsg.Value = Local_Input_Value_Get(data);	//获取当前显示值
			if (CurrentMsg.InputType)	//字符输入
			{
				if (strlen(CurrentMsg.Value.c_str()) < strlen(CurrentMsg.FmtStr.c_str()))		//当字符个数没达到格式限值，可以输入
				{
					CurrentMsg.Value += ".";				//获取到当前按键输入值
					Win()->SendUserMessage((void*)CurrentMsg.SrcCtrlId, KEY_EVENT_VALUE, (int)&CurrentMsg);	//发送消息给源ctrl
					//Local_Input_Value_Set(CurrentMsg.Value.c_str());	//设置显示寄存器数值
				}
			}
			else {	//数值输入
				if (CurrentMsg.Value.find(".") == -1 && CurrentMsg.Accuracy.Bct != 0) {	//判断当前是否含有小数点
					CurrentMsg.Value += ".";				//获取到当前按键输入值
					Win()->SendUserMessage((void*)CurrentMsg.SrcCtrlId, KEY_EVENT_VALUE, (int)&CurrentMsg);	//发送消息给源ctrl
					//Local_Input_Value_Set(CurrentMsg.Value.c_str());	//设置显示寄存器数值
				}
			}
			break;
		default:
			break;
		}
	}
	void KeyMsgControl::HandleKeyNum(int lparam)
	{
		char data[128];
		if (CurrentMsg.InputType)	//字符输入
		{
			if (CurrentMsg.InputType)	//字符输入
			{
	/*			if (!CurrentMsg.Value.empty())
					CurrentMsg.Value = Local_Input_Value_Get(data);*/
				if ((int)strlen(CurrentMsg.Value.c_str()) < strlen(CurrentMsg.FmtStr.c_str()))		//当字符个数没达到格式限值，可以输入
					CurrentMsg.Value += (char)lparam;				//获取到当前按键输入值
				Win()->SendUserMessage((void*)CurrentMsg.SrcCtrlId, KEY_EVENT_VALUE, (int)&CurrentMsg);	//发送消息给源ctrl
				//Local_Input_Value_Set(CurrentMsg.Value.c_str());	//设置显示寄存器数值
			}
		}
		else {		//数值输入
			/*if (!CurrentMsg.Value.empty())
				CurrentMsg.Value = Local_Input_Value_Get(data);*/
			//判断当前是否含有小数点
			if (!CurrentMsg.Value.find("."))
			{
				//判断当前位数是否大于整数位数限制
				if ((int)strlen(CurrentMsg.Value.c_str()) < CurrentMsg.Accuracy.Act)
				{
					CurrentMsg.Value += (char)lparam;				//获取到当前按键输入值
					Win()->SendUserMessage((void*)CurrentMsg.SrcCtrlId, KEY_EVENT_VALUE, (int)&CurrentMsg);	//发送消息给源ctrl
					strcpy(data, CurrentMsg.Value.c_str());
					//Local_Input_Value_Set(CurrentMsg.Value.c_str());	//设置显示寄存器数值
				}
			}
			else
			{
				//判断当前位数是否大于小数位数限制
				if ((int)strlen(CurrentMsg.Value.c_str()) < CurrentMsg.Accuracy.Act + CurrentMsg.Accuracy.Bct + 1)
				{
					CurrentMsg.Value += (char)lparam;				//获取到当前按键输入值
					Win()->SendUserMessage((void*)CurrentMsg.SrcCtrlId, KEY_EVENT_VALUE, (int)&CurrentMsg);	//发送消息给源ctrl
					//Local_Input_Value_Set(CurrentMsg.Value.c_str());	//设置显示寄存器数值			
				}
			}
		}
	}

	void KeyMsgControl::HandleChangePinyin(int lparam)
	{
		UI::PinYinWindow *win;
		win = (PinYinWindow *)Win();
		CurrentMsg = *(KeyMessage *)lparam;
		//设置当前label显示拼音
		win->PinYin = CurrentMsg.Value;
		win->SetPinYinChanged(CurrentMsg.Value);
		win->SearchChineseCharacters(0, CurrentMsg.Value);
	}

	int KeyMsgControl::PeekHMIMessage(Message::Msg* msg)
	{
		Project::DataVarId varId;
		KeyMessage *keymsg;
		switch (msg->GetCode())
		{
		case WM_EVENT_DATEUPDATE:
			break;
		case WM_EVENT_TIMEUP:
			Win()->SendUserMessage((void*)this->CtrlId(), WM_EVENT_TIMETICK, 1000);//1s
			break;
		case WM_EVENT_READY:
			Win()->SendUserMessage((void*)this->CtrlId(), WM_EVENT_TIMETICK, 1000);//1s
			break;
		case WM_EVENT_OPEN:
			OpenKeyWindow(msg->LParam);
			break;
		case KEY_EVENT_KEYMSG:
			InitKeyMsg(msg->LParam);
			break;
		case WM_EVENT_CLOSE:			
			break;
		case KEY_EVENT_CHAR:	//按键输入字符
			HandleKeyChar(msg->LParam);
			break;
		case KEY_EVENT_PINYIN:	//按键输入字符
			HandleChangePinyin(msg->LParam);
			break;
		case KEY_EVENT_NUM:		//按键输入数字 
			HandleKeyNum(msg->LParam);
			break;
		case KEY_EVENT_VALUE:	//获取按键输入值
			Win()->SendUserMessage((void*)CurrentMsg.InputCtrlId, KEY_EVENT_VALUE, (int)msg->LParam);
			break;
		case KEY_EVENT_FUNCTION:
			HandleKeyMessage(msg->LParam);
			break;
		case BTN_SET_CHINESE:
			Win()->SendUserMessage((void*)CurrentMsg.DstCtrlId, KEY_EVENT_CHINESE, (int)msg->LParam);
			break;
		case KEY_EVENT_CHINESE:
			CurrentMsg.Chinese += (char *)msg->LParam;
			CurrentMsg.Value.clear();
			//Win()->WinMgr()->CloseWindow(Project::GetWinNo(CurrentMsg.PinYinCtrlId));
			Win()->SendUserMessage((void*)CurrentMsg.InputCtrlId, KEY_EVENT_VALUE, (int)&CurrentMsg);
			break;
		default:
			break;
		}
		return 1;//handled;
	}
}
