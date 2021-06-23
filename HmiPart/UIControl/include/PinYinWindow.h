/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PinYinWindow.h
 * Author   : qiaodan
 * Date     : 2020-10-15
 * Descript : 输入法窗口
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "ChineseBtnControl.h"
#include "HMIWindow.h"
#include "pinyinime.h"
#include <FL/Fl_Output.H>
namespace UI
{
	struct ChineseMsg
	{
		string chinese;
	};
	class PinYinWindow : public HMIWindow
	{
	public:
		PinYinWindow(int x, int y, int w, int h, const char* title = 0);
		~PinYinWindow();
		vector<Project::WinCtrlID> CharacterBtns;
		Project::WinCtrlID  PrevBtn;
		Project::WinCtrlID NextBtn;
		Fl_Output *PinYinLabel;
		int CurrentPage;
		int ChineseNum = 7;
		string PinYin;
		ChineseMsg CurrentMsg;
		/*******************************************************************************
		 * Name     : OpenPinYinWindow
		 * Descript : 用来设置输入法窗口显示
		 * Input    : winno - 窗口号
		 * Output   :
		 * Note	    :
		 *******************************************************************************/
		void InitPinYinWindow(int winno);
		void SetPinYinChanged(string pinyin);
		void SearchChineseCharacters(const int &currentPage, string pinyin);

		static PinYinWindow* GetPinYinWindow();
	};
}


