/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BtnPilotLampView.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ����ָʾ�ư����Ļ���ͷ�ļ�
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include <stdafx.h>
#include <FL/Fl_Button.H>
#include <BasicView.h>
#include "BtnPilotLampModel.h"
using namespace std;
namespace UI
{
	class BtnPilotLampView : public HMIBaseButton
	{
	public:
		BtnPilotLampView(int X, int Y, int W, int H, const char *L);
		~BtnPilotLampView();
		BasicView BaseView;

	    /*******************************************************************************
		 * Name     : handle
		 * Descript : ���ڴ�������Ӧ�¼�
		 * Input    : event - �¼�����
		 * Output   : 1	- ����ɹ�
		 * Note	    :
		 *******************************************************************************/
		int handle(int event);
		/*���ݰ�������ģʽ��������*/
		void HandleClick();
		int CurrentStatus = 0;		//0:OFF״̬	1:ON״̬
		int CurrentFlash = 1;		//0:����	1:��ʾ
		bool IsReleased = true;
		bool HaveOperateLimit = false;
		bool HandleOperatePush();
	protected:
		void draw() override;
	};

}
