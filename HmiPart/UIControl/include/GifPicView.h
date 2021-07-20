#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GifPicView.h
 * Author   : qiaodan
 * Date     : 2020-12-22
 * Descript : 关于GIF图片的绘制头文件，绘制显示框和文字、背景图片
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "BasicView.h"
#include <stdafx.h>
#include <FL/Fl_Box.H>
namespace UI
{
	class GifPicView : public HMIBaseView
	{
	public:
		GifPicView(int X, int Y, int W, int H, const char* l);
		~GifPicView();
		
		/*初始化绘制使用的一些参数*/
		void InitDraw();
		BasicView BaseView;

		//vector<Fl_RGB_Image*> PicRes;       //每帧图片资源
		//vector<unsigned> DelayTime;         //帧长
		int PicNum;                         //帧数
		int CurrentPicNo = 0;
		int CurrentFlash = 1;
	protected:
		void draw() override;
		int handle(int event);
	private:
	};
}
