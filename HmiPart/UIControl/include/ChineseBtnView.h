/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ChineseBtnView.h
 * Author   : qiaodan
 * Date     : 2020-10-15
 * Descript : �������İ����Ļ���ͷ�ļ���������ʾ������֡�����ͼƬ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/

#pragma once
#include <BasicView.h>
#include <FL/Fl_Button.H>
using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

#define BTN_PREV	0
#define BTN_NEXT	1
#define BTN_VALUE	3
namespace UI
{
	class ChineseBtnView : public HMIBaseButton
	{
	public:
		ChineseBtnView(int X, int Y, int W, int H, const char *L);
		~ChineseBtnView();
		BasicView BaseView;
		int handle(int event);
		bool achieveTime = false;

	protected:
		void draw() override;
		high_resolution_clock::time_point beginTime;
		high_resolution_clock::time_point endTime;

	};

}
