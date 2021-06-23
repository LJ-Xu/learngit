#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : AlarmWinView.h
 * Author   : qiaodan
 * Date     : 2020-11-10
 * Descript : ���ڱ��������Ļ���ͷ�ļ������Ʊ�������
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include <stdafx.h>
#include <FL/Fl_Button.H>
#include <BasicView.h>
#include "AlarmWinModel.h"
#include "AlarmWinControl.h"
#include "AlarmInfoRes.h"
using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

namespace UI
{
	class AlarmWinView : public HMIBaseWindow
	{
	public:
		AlarmWinView(int X, int Y, int W, int H, const char *L);
		~AlarmWinView();

		/*******************************************************************************
		 * Name     : handle
		 * Descript : ���ڴ�������Ӧ�¼�
		 * Input    : event - �¼�����
		 * Output   : 1	- ����ɹ�
		 * Note	    :
		 *******************************************************************************/
		int handle(int event);
		BasicView BaseView;
		Project::AlarmContent AlarmInfo;
		bool GetCloseMode(int &mode,int &time);
		
	protected:
		void draw() override;

	private:
		high_resolution_clock::time_point beginTime_;	//��¼��������ʱ��
		high_resolution_clock::time_point endTime_;		//��¼�����ɿ�ʱ��

	};
}
