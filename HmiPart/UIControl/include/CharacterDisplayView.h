/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : CharacterDisplayView.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : �����ַ���ʾ��Ļ���ͷ�ļ���������ʾ������֡�����ͼƬ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BasicView.h"
#include <FL/Fl_Input.H>
#include <stdafx.h>
#include "DigitalDisplayView.h"

namespace UI
{
	class CharacterDisplayView : public InputView
	{
	public:
		CharacterDisplayView(int X, int Y, int W, int H, const char* l);
		~CharacterDisplayView();

		BasicView BaseView;
		string DisplayValue;

		/*******************************************************************************
		 * Name     : handle
		 * Descript : ��дFl_Input�й����¼���Ӧ������
		 * Input    : event	- �¼����
		 * Output   : 1 - �����¼��ɹ�
		 * Note	    :
		 *******************************************************************************/
		int handle(int event);
	protected:
		void draw() override;
	};
}
