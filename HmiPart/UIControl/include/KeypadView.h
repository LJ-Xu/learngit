/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeypadView.h
 * Author   : qiaodan
 * Date     : 2020-10-22
 * Descript : ���ڰ����Ļ���ͷ�ļ������Ƽ��̰���
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include <stdafx.h>
#include <FL/Fl_Button.H>
#include <BasicView.h>
#include "KeypadModel.h"
#include "KeypadControl.h"

using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

namespace UI
{
	//void BtnshortON_cb(void *t);

	class KeypadView : public HMIBaseButton
	{
		/*******************************************************************************
		 * Name     : time_cb
		 * Descript : ��������Ӧ�¼��ص�����
		 * Input    : t - �ɴ�����������
		 * Output   :
		 * Note	    :
		 *******************************************************************************/
	public:
		KeypadView(int X, int Y, int W, int H, const char *L);
		~KeypadView();

		/*******************************************************************************
		 * Name     : handle
		 * Descript : ���ڴ�������Ӧ�¼�
		 * Input    : event - �¼�����
		 * Output   : 1	- ����ɹ�
		 * Note	    :
		 *******************************************************************************/
		int handle(int event);
		/*******************************************************************************
		 * Name     : HandleClick
		 * Descript : ���ڴ����������ָʾ�ư������õ���Ϊ�����õ�ǰ״̬
		 * Input    :
		 * Output   :
		 * Note	    :
		 *******************************************************************************/
		void HandleClick();
		BasicView BaseView;
		//bool HasShortOnEvent = false;
		bool IsReleased = true;
		bool HaveOperateLimit = false;
		bool HandleOperatePush();
		int CurrentStatus = 0;
	protected:
		void draw() override;

	private:
		//high_resolution_clock::time_point beginTime_;	//��¼��������ʱ��
		//high_resolution_clock::time_point endTime_;		//��¼�����ɿ�ʱ��

	};
}
