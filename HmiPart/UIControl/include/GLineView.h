/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GLineView.h
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : ����ֱ�ߵĻ���ͷ�ļ�������ֱ��
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "GraphicDrawHandle.h"
#include "stdafx.h"
#include "BasicView.h"
#include "GLineControl.h"
#include "GLineModel.h"
namespace UI
{
	class GLineView : public HMIBaseView
	{
	public:
		GLineView(int X, int Y, int W, int H);
		~GLineView();
		BasicView BaseView;
	protected:
		void draw() override;
		int handle(int event);
	private:
		/*�����߶β��֣�������Arrow_Rotate�������Ƽ�ͷ����*/
		void DrawLine();
		/*�����߶μ�ͷ����*/
		void DrawStartArrow(double angle, int& startx, int& starty);
		void DrawEndArrow(double angle, int& endx, int& endy);
		void CalculateAngle(double &startangle, double &endangle);
	};
}
