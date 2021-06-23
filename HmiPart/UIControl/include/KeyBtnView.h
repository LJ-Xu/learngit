/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeyBtnView.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ���ڼ��̰����Ļ���ͷ�ļ������Ƽ��̰���
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include <stdafx.h>
#include <FL/Fl_Button.H>
#include <BasicView.h>
#include "KeyBtnModel.h"
#include "KeyBtnControl.h"

using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

namespace UI
{
	class KeyBtnView : public HMIBaseButton
	{
	public:
		KeyBtnView(int X, int Y, int W, int H, const char *L);
		~KeyBtnView();

		/*******************************************************************************
		 * Name     : handle
		 * Descript : ���ڴ�������Ӧ�¼�
		 * Input    : event - �¼�����
		 * Output   : 1	- ����ɹ�
		 * Note	    :
		 *******************************************************************************/
		int handle(int event);
		BasicView BaseView;
		bool IsReleased = true;
		int CurrentStatus = 0;
		void HandleKeyMsg();
		bool HandleOperatePush();

	protected:
		void draw() override;
	};
}
