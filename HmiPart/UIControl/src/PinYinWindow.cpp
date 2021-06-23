/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PinYinWindow.cpp
 * Author   : qiaodan
 * Date     : 2020-10-15
 * Descript : 绘制输入法显示窗口
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "PinYinWindow.h"
#include "ControlFactory.h"
#include "FL/Fl_Output.H"
using namespace ime_pinyin;

namespace UI
{
	PinYinWindow::PinYinWindow(int x, int y, int w, int h, const char* title) :HMIWindow(x, y, w, h, title) {}
	PinYinWindow::~PinYinWindow(){}


	void PinYinWindow::InitPinYinWindow(int winno)
	{
#if 0
		//Init(winno, nullptr);
		int ctrno = 0;
		int x = 0;
		char jstr[200];
		for (int i = 0; i < 7; i++)
		{
			shared_ptr<BaseControl> ctrl(ControlFactory::GetNewControlByNameEx("ChineseBtnControl"));
			if (ctrl)
			{
				x += 50;
				sprintf(jstr,"{\"X\":%d,\"Y\":40,\"Width\":50,\"Height\":50}", x); 
				ctrl->SetHMIWindow(this);
				ctrl->CreateView(jstr);
			}
			ctrl->CtrlId(Project::WinCtrlID(CTR_ID(winno, ctrno))); ctrno++;
			//AddCtrl(ctrl);
			CharacterBtns.push_back(ctrl->CtrlId());
		}
		shared_ptr<BaseControl> prevCtrl(ControlFactory::GetNewControlByNameEx("ChineseBtnControl"));
		if (prevCtrl)
		{
			prevCtrl->SetHMIWindow(this);
			prevCtrl->CreateView("{\"X\":0,\"Y\":40,\"Width\":50,\"Height\":50,\"Text\":\"@4->\"}");
		}
		prevCtrl->CtrlId(Project::WinCtrlID(CTR_ID(winno, ctrno))); ctrno++;
		//AddCtrl(prevCtrl);
		PrevBtn = prevCtrl->CtrlId();

		shared_ptr<BaseControl> nextCtrl(ControlFactory::GetNewControlByNameEx("ChineseBtnControl"));
		if (nextCtrl)
		{
			nextCtrl->SetHMIWindow(this);
			nextCtrl->CreateView("{\"X\":400,\"Y\":40,\"Width\":50,\"Height\":50,\"Text\":\"@6->\"}");
		}
		nextCtrl->CtrlId(Project::WinCtrlID(CTR_ID(winno, ctrno))); ctrno++;
		//AddCtrl(nextCtrl);
		NextBtn = nextCtrl->CtrlId();

		shared_ptr<BaseControl> keyCtrl(ControlFactory::GetNewControlByNameEx("KeyMsgControl"));
		if (keyCtrl)
		{
			keyCtrl->SetHMIWindow(this);
			keyCtrl->CreateView("");
		}
		/*SetKeyboardId(Project::WinCtrlID(CTR_ID(winno, ctrno)));ctrno++;
		AddCtrl(keyCtrl);*/
		//Ctrls.push_back(keyCtrl);
		//new Fl_Input(0, 0, 80, 40, "Normal:");
		PinYinLabel = new Fl_Output(0,0,80,40);
		PinYinLabel->box((Fl_Boxtype)0);
#endif
	}

	void PinYinWindow::SetPinYinChanged(string pinyin)
	{
		//获取当前拼音
		PinYinLabel->hide();
		const char * label = pinyin.c_str();
		PinYinLabel->value(label);
		PinYinLabel->redraw();
		PinYinLabel->show();
		//从库中获取拼音符合的汉字
		//设置ChineseBtnControl
	}

	void PinYinWindow::SearchChineseCharacters(const int &currentPage, string pinyin)
	{
		const int max_spelling_length = 32;
		const int max_decoded_length = 32;
		const int max_single_hanzi = 20;
		static unsigned int page_change_times = 0;
		const char* syspath = "C:\\Users\\PC\\source\\repos\\pd-hmi-cpp\\HmiComm\\googlepinyin\\dict\\dict_pinyin.dat";
		const char* usrpath = "C:\\Users\\PC\\source\\repos\\pd-hmi-cpp\\HmiComm\\googlepinyin\\dict\\dict_pinyin_user.dat";
		//QString app_dir(qApp->applicationDirPath() + "/dict");
		im_open_decoder(syspath,usrpath);
		im_set_max_lens(max_spelling_length, max_decoded_length);
		im_reset_search();
		size_t cand_num = im_search(pinyin.c_str(), pinyin.size());

		size_t decode_len;
		im_get_sps_str(&decode_len);

		switch (currentPage)
		{
		case 1:
			if (cand_num > ChineseNum && page_change_times  <= cand_num / ChineseNum)
				page_change_times++;
			break;
		case -1:
			if (page_change_times > 0) page_change_times--;
			break;
		default:
			page_change_times = 0;
			break;
		}
		if (0 == page_change_times)
			SendUserMessage((void *)PrevBtn, BTN_SET_ENABLE,0);
		else
			SendUserMessage((void *)PrevBtn, BTN_SET_ENABLE, 1);

		if (page_change_times == cand_num / ChineseNum)
			SendUserMessage((void *)NextBtn, BTN_SET_ENABLE, 0);
		else
			SendUserMessage((void *)NextBtn, BTN_SET_ENABLE, 1);
		unsigned i,k;
		char16 *cand_buf = new char16[max_decoded_length];
		char16 *cand;
		char *cand_char = new char[max_decoded_length];
		memset(cand_char, 0, max_decoded_length);
		string cand_str;
		if (CharacterBtns.size() == 0)
			return;
		std::unique_ptr<Project::WinCtrlID[]> btnIdx{ new Project::WinCtrlID[CharacterBtns.size()] };

		vector<Project::WinCtrlID>::iterator btn;
		for (i = 0, btn = CharacterBtns.begin(); btn != CharacterBtns.end(); btn++, i++)
			btnIdx.get()[i] = (*btn);
		//i = 0;
		for (i = 0; i < ChineseNum; i++)
		{
			if (page_change_times * ChineseNum + i <= cand_num)
			{
				cand = im_get_candidate(page_change_times * ChineseNum + i, cand_buf, max_decoded_length);
				if (cand)
				{
					CurrentMsg.chinese.erase();
					for (int j = 0; j < strlen((char *)cand_buf) / 2; j++)
					{
						fl_utf8encode(cand_buf[j], cand_char);	//转换为utf8编码
						CurrentMsg.chinese += cand_char;
					}
					 SendAutoFreeMessage<ChineseMsg>((void *)btnIdx.get()[i], BTN_GET_CHINESE, &CurrentMsg);
					//SendUserMessage((void *)btnIdx.get()[i], WM_CHINESE,(int)CurrentMsg);
					if (i == 0) cand_str.erase(0, im_get_fixed_len());
				}
				else
				{
					cand_str = "";
				}
			}
		}
		delete cand_buf;
	}





	PinYinWindow* PinYinWindow::GetPinYinWindow()
	{
		UI::PinYinWindow *win = new UI::PinYinWindow(0, 0, 450, 90);
		win->InitPinYinWindow(2);
		 
		return win;
	}
}
