/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : MultiPilotLampView.h
 * Author   : qiaodan
 * Date     : 2020-10-22
 * Descript : ���ڶ�״ָ̬ʾ�ƵĻ���ͷ�ļ������ƶ�״ָ̬ʾ��
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include <stdafx.h>
#include <FL/Fl_Box.H>
#include <BasicView.h>
#include "MultiPilotLampModel.h"
namespace UI
{
	class MultiPilotLampView : public HMIBaseLabel
	{
	public:
		MultiPilotLampView(int X, int Y, int W, int H);
		~MultiPilotLampView();
		BasicView BaseView;
		int CurrentStatus = 0;		//0:OFF״̬	1:ON״̬
		int CurrentFlash = 1;		//0:����	1:��ʾ
	protected:
		void draw() override;
	};

}

