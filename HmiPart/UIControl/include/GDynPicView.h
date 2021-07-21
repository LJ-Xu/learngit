/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GDynPicView.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于动态图片绘制头文件
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
	class GDynPicView : public HMIBaseView
	{
	public:
		GDynPicView(int X, int Y, int W, int H);
		~GDynPicView();
		BasicView BaseView;

		vector<int> SortNum;
		vector<Fl_Image*> PicImages;	//需要显示的图片
		void InitDraw();				//初始化绘制
		void SortPic();					//图片排序
		void SortPicKey();				//图片keyz值排序（用于非缓存显示）
		void GetPicRes();				//获取图片资源

	protected:
		void draw() override;
		int handle(int event);
	};
}

