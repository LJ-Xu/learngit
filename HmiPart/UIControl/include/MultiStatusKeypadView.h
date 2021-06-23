/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : MultiPilotLampView.h
 * Author   : qiaodan
 * Date     : 2020-10-22
 * Descript : ���ڶ�״̬�����Ļ���ͷ�ļ������ƶ�״̬����
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include <stdafx.h>
#include <FL/Fl_Button.H>
#include <BasicView.h>
#include "MultiStatusKeypadModel.h"
#include "MultiStatusKeypadControl.h"
using namespace std;
namespace UI
{
	class MultiStatusKeypadView : public HMIBaseButton
	{
	public:
		MultiStatusKeypadView(int X, int Y, int W, int H, const char *L);
		~MultiStatusKeypadView();
		BasicView BaseView;

		/*�¼�������*/
		int handle(int event);

		/*���ڴ����������ָʾ�ư������õ���Ϊ�����õ�ǰ״̬*/
		void HandleClick();
		bool HandleOperatePush();

		/*������������õ�ǰ״ִ̬�еĶ���*/
		void SetStatusAction();
		int CurrentStatus = 0;
		//int CurrentBtnStatus = 0;
		bool IsReleased = true;
		bool IsAchieveSafeTime = false;
	protected:
		void draw() override;

	};

}

