/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StaticStringView.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ���ھ�̬���ֵĻ���ͷ�ļ������ƾ�̬���ֿؼ�
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BasicView.h"
#include "stdafx.h"
#include "StaticStringControl.h"
#include "StaticStringModel.h"
namespace UI
{
	class StaticStringView : public HMIBaseView
	{
	public:
		StaticStringView(int X, int Y, int W, int H);
		~StaticStringView();
		BasicView BaseView;
	protected:
		void draw() override;
		void drawUnderline();
		void drawStrikeout();
	};
}
