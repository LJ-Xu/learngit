/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeyMsgControl.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ����������Ϣ���ƣ����ݴ�����Ϣ
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
		//����KeyMsgModel
		mode_ = std::shared_ptr <KeyMsgModel>(new KeyMsgModel());
		InitMVCModel(mode_);
	}

	void KeyMsgControl::CreateView()
	{
		//��ʼ��VIEW
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
		KeyMessage *keymsg = (KeyMessage *)lparam;		//��ȡ��Ϣ
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

		//window = UI::UIMain::Ins().Test_OpenWindow(2);		//����ʹ��
		//keymsg->DstCtrlId = window->KeyboardId();		//��ȡ��������ļ��̿ؼ�
		/*window->set_modal();
		window->show();
		Win()->SendAutoFreeMessage<KeyMessage>((void*)keymsg->DstCtrlId, KEY_EVENT_KEYMSG, keymsg);*/
		//delete keymsg;
		//m_pWindow->SendUserMessage((void*)keymsg->DstCtrlId, KEY_EVENT_KEYMSG, (int)keymsg);	//����Ϣ��Ϣ���͸��µļ��̿ؼ�
	}

	void KeyMsgControl::InitKeyMsg(int lparam)
	{
		//CurrentMsg = new KeyMessage();
		KeyMessage *keymsg = (KeyMessage *)lparam;	//��ʼ�����̴���/���뷨��keymsg
		CurrentMsg = *keymsg;
		//��ȡ����ֵ
		//SetCurrentMsg(CurrentMsg ,keymsg);
		//CurrentMsg = (struct KeyMessage *)msg->LParam;		//��ȡ��Ϣ
		//PSW::Sys().SetNumberData(LOCAL_SYS_INPUT_UPPERLMT, CurrentMsg.UpperLmt);//�������޼Ĵ�����ֵ
		//PSW::Sys().SetNumberData(LOCAL_SYS_INPUT_LOWERLMT, CurrentMsg.LowerLmt);//�������޼Ĵ�����ֵ
		LocalData::SetNumberData(SYS_PSW_INPUT_NUMBER_LOWERLMT, CurrentMsg.UpperLmt);
		LocalData::SetNumberData(SYS_PSW_INPUT_NUMBER_UPPERERLMT, CurrentMsg.UpperLmt);

		//SPSW::Ins().SetNumberData(LOCAL_SYS_INPUT_LOWERLMT, CurrentMsg.LowerLmt);
		//Local_Input_Value_Set(CurrentMsg.Value.c_str());	//������ʾ�Ĵ�����ֵ
		if (CurrentMsg.InputType != 2)
			CurrentMsg.Value.clear();						//���valueֵ
	}

	void KeyMsgControl::HandleKeyChar(int lparam)
	{
		char data[128];
		switch (CurrentMsg.InputType)
		{
		case 0:				//��ֵ
			break;
		case 1:				//�ַ�
			//if (!CurrentMsg.Value.empty())
			//	CurrentMsg.Value = Local_Input_Value_Get(data);
			if (strlen(CurrentMsg.Value.c_str()) < strlen(CurrentMsg.FmtStr.c_str()))		//���ַ�����û�ﵽ��ʽ��ֵ����������
				CurrentMsg.Value += (char)lparam;				//��ȡ����ǰ��������ֵ
			Win()->SendUserMessage((void*)CurrentMsg.SrcCtrlId, KEY_EVENT_VALUE, (int)&CurrentMsg);	//������Ϣ��Դctrl
			//Local_Input_Value_Set(CurrentMsg.Value.c_str());	//������ʾ�Ĵ�����ֵ
			break;
		case 2:				//����
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
					//Local_Input_Value_Set(CurrentMsg.Value.c_str());	//������ʾ�Ĵ�����ֵ
				}
				break;
			case 1:
				if (CurrentMsg.Value.length() != 0) {
					CurrentMsg.Value.erase(CurrentMsg.Value.end() - 1);
					Win()->SendUserMessage((void*)CurrentMsg.InputCtrlId, KEY_EVENT_VALUE, (int)&CurrentMsg);
					//Local_Input_Value_Set(CurrentMsg.Value.c_str());	//������ʾ�Ĵ�����ֵ
				}
				break;
			case 2:
				if (CurrentMsg.Value.empty())		//��ǰƴ��Ϊ�գ���ɾ���ַ�
				{
					//	CurrentMsg.Chinese.erase(CurrentMsg.Chinese.end() - 3);
					CurrentMsg.Chinese = CurrentMsg.Chinese.substr(0, CurrentMsg.Chinese.size() - 3);
					Win()->SendUserMessage((void*)CurrentMsg.InputCtrlId, KEY_EVENT_VALUE, (int)&CurrentMsg);
					//Local_Input_Value_Set(CurrentMsg.Chinese.c_str());	//������ʾ�Ĵ�����ֵ
				}
				else
				{
					UI::PinYinWindow *win;
					UI::HMIWindow *window;
					//window = Win()->WinMgr()->GetWinByCtrID(CurrentMsg.PinYinCtrlId);
					//win = (UI::PinYinWindow *)window;
					CurrentMsg.Value.erase(CurrentMsg.Value.end() - 1);
					if (CurrentMsg.Value.empty())
						//�ر�ƴ������
						;//Win()->WinMgr()->CloseWindow(Project::GetWinNo(CurrentMsg.PinYinCtrlId));
					else
					{
						//win->PinYin = CurrentMsg.Value;
						//win->SetPinYinChanged(CurrentMsg.Value);
						////�����µ�ƴ��
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
			//�رռ��̴���
			//m_pWindow->hide();
			break;
		case KEY_CAPS:
			break;
		case KEY_ENTER:
			//CurrentMsg.Value = Local_Input_Value_Get(data);
			Win()->SendAutoFreeMessage<KeyMessage>((void*)CurrentMsg.InputCtrlId, KEY_EVENT_ENTER, &CurrentMsg);

			
			//m_pWindow->SendUserMessage((void*)currentMsg->inputCtrlId, KEY_EVENT_ENTER, (int)currentMsg);	//������Ϣ��Դctrl
			break;
		case KEY_MINUS:
			break;
		case KEY_CLR:
			CurrentMsg.Value.clear();							//����ֵ���
			//Local_Input_Value_Set(CurrentMsg.Value.c_str());	//������ʾ�Ĵ�����ֵ
			Win()->SendUserMessage((void*)CurrentMsg.SrcCtrlId, KEY_EVENT_VALUE, (int)&CurrentMsg);	//������Ϣ��Դctrl
			break;
		case KEY_DOT:
			//if (!CurrentMsg.Value.empty())
			//	CurrentMsg.Value = Local_Input_Value_Get(data);	//��ȡ��ǰ��ʾֵ
			if (CurrentMsg.InputType)	//�ַ�����
			{
				if (strlen(CurrentMsg.Value.c_str()) < strlen(CurrentMsg.FmtStr.c_str()))		//���ַ�����û�ﵽ��ʽ��ֵ����������
				{
					CurrentMsg.Value += ".";				//��ȡ����ǰ��������ֵ
					Win()->SendUserMessage((void*)CurrentMsg.SrcCtrlId, KEY_EVENT_VALUE, (int)&CurrentMsg);	//������Ϣ��Դctrl
					//Local_Input_Value_Set(CurrentMsg.Value.c_str());	//������ʾ�Ĵ�����ֵ
				}
			}
			else {	//��ֵ����
				if (CurrentMsg.Value.find(".") == -1 && CurrentMsg.Accuracy.Bct != 0) {	//�жϵ�ǰ�Ƿ���С����
					CurrentMsg.Value += ".";				//��ȡ����ǰ��������ֵ
					Win()->SendUserMessage((void*)CurrentMsg.SrcCtrlId, KEY_EVENT_VALUE, (int)&CurrentMsg);	//������Ϣ��Դctrl
					//Local_Input_Value_Set(CurrentMsg.Value.c_str());	//������ʾ�Ĵ�����ֵ
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
		if (CurrentMsg.InputType)	//�ַ�����
		{
			if (CurrentMsg.InputType)	//�ַ�����
			{
	/*			if (!CurrentMsg.Value.empty())
					CurrentMsg.Value = Local_Input_Value_Get(data);*/
				if ((int)strlen(CurrentMsg.Value.c_str()) < strlen(CurrentMsg.FmtStr.c_str()))		//���ַ�����û�ﵽ��ʽ��ֵ����������
					CurrentMsg.Value += (char)lparam;				//��ȡ����ǰ��������ֵ
				Win()->SendUserMessage((void*)CurrentMsg.SrcCtrlId, KEY_EVENT_VALUE, (int)&CurrentMsg);	//������Ϣ��Դctrl
				//Local_Input_Value_Set(CurrentMsg.Value.c_str());	//������ʾ�Ĵ�����ֵ
			}
		}
		else {		//��ֵ����
			/*if (!CurrentMsg.Value.empty())
				CurrentMsg.Value = Local_Input_Value_Get(data);*/
			//�жϵ�ǰ�Ƿ���С����
			if (!CurrentMsg.Value.find("."))
			{
				//�жϵ�ǰλ���Ƿ��������λ������
				if ((int)strlen(CurrentMsg.Value.c_str()) < CurrentMsg.Accuracy.Act)
				{
					CurrentMsg.Value += (char)lparam;				//��ȡ����ǰ��������ֵ
					Win()->SendUserMessage((void*)CurrentMsg.SrcCtrlId, KEY_EVENT_VALUE, (int)&CurrentMsg);	//������Ϣ��Դctrl
					strcpy(data, CurrentMsg.Value.c_str());
					//Local_Input_Value_Set(CurrentMsg.Value.c_str());	//������ʾ�Ĵ�����ֵ
				}
			}
			else
			{
				//�жϵ�ǰλ���Ƿ����С��λ������
				if ((int)strlen(CurrentMsg.Value.c_str()) < CurrentMsg.Accuracy.Act + CurrentMsg.Accuracy.Bct + 1)
				{
					CurrentMsg.Value += (char)lparam;				//��ȡ����ǰ��������ֵ
					Win()->SendUserMessage((void*)CurrentMsg.SrcCtrlId, KEY_EVENT_VALUE, (int)&CurrentMsg);	//������Ϣ��Դctrl
					//Local_Input_Value_Set(CurrentMsg.Value.c_str());	//������ʾ�Ĵ�����ֵ			
				}
			}
		}
	}

	void KeyMsgControl::HandleChangePinyin(int lparam)
	{
		UI::PinYinWindow *win;
		win = (PinYinWindow *)Win();
		CurrentMsg = *(KeyMessage *)lparam;
		//���õ�ǰlabel��ʾƴ��
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
		case KEY_EVENT_CHAR:	//���������ַ�
			HandleKeyChar(msg->LParam);
			break;
		case KEY_EVENT_PINYIN:	//���������ַ�
			HandleChangePinyin(msg->LParam);
			break;
		case KEY_EVENT_NUM:		//������������ 
			HandleKeyNum(msg->LParam);
			break;
		case KEY_EVENT_VALUE:	//��ȡ��������ֵ
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
