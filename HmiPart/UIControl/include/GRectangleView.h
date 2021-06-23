/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GRectangleView.h
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : ���ھ��εĻ���ͷ�ļ������ƾ���
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "GraphicDrawHandle.h"
#include "stdafx.h"
#include "BasicView.h"
namespace UI
{
	class GRectangleView : public HMIBaseView
	{
	public:
		GRectangleView(int X, int Y, int W, int H);
		~GRectangleView();
		BasicView BaseView;
	protected:
		void draw() override;
	private:
		void DrawFill();
	private:
		int drawRectRadius;//���Ƶ�ֱ�ǳߴ�
	};
}

