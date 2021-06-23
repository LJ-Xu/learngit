/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : FanView.h
 * Author   : qiaodan
 * Date     : 2021-3-31
 * Descript : ���ڷ��Ȼ���ͷ�ļ�
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "stdafx.h"
#include <BasicView.h>
#include <FL/Fl_Box.H>
namespace UI
{
	class FanView : public HMIBaseView
	{
	public:
		FanView(int X, int Y, int W, int H);
		~FanView();
		BasicView BaseView;
		void InitDraw();				//��ʼ������
		int CurrentPic = 0;
	protected:
		void draw() override;
	};
}