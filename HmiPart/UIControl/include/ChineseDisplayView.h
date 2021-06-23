/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ChineseBtnView.h
 * Author   : qiaodan
 * Date     : 2020-10-15
 * Descript : ����������ʾ�Ļ���ͷ�ļ���������ʾ������֡�����ͼƬ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BasicView.h"
#include <FL/Fl_Input.H>
#include <stdafx.h>
#include "DigitalInputView.h"

namespace UI
{
	class ChineseDisplayView : public InputView
	{
	public:
		ChineseDisplayView(int X, int Y, int W, int H, const char* l);
		~ChineseDisplayView();

		BasicView BaseView;
		string DisplayValue;

		/*��дFl_Input�й����¼���Ӧ������*/
		int handle(int event);
	protected:
		void draw() override;
	};
}