/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : VarStringView.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ���ڶ�̬���ֵĻ���ͷ�ļ������ƶ�̬���ֿؼ�
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BasicView.h"
#include "stdafx.h"
#include "VarStringControl.h"
#include "VarStringModel.h"
#include "Point.h"
namespace UI
{
	class VarStringView : public HMIBaseView
	{
	public:
		VarStringView(int X, int Y, int W, int H);
		~VarStringView();
		BasicView BaseView;

		int CurrentStatus = 0;
	protected:
		void draw() override;
		void drawLine(vector<Project::Point> &point);
	};
}
