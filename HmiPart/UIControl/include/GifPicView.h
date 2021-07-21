#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GifPicView.h
 * Author   : qiaodan
 * Date     : 2020-12-22
 * Descript : ����GIFͼƬ�Ļ���ͷ�ļ���������ʾ������֡�����ͼƬ
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
		
		/*��ʼ������ʹ�õ�һЩ����*/
		void InitDraw();
		BasicView BaseView;

		//vector<Fl_RGB_Image*> PicRes;       //ÿ֡ͼƬ��Դ
		//vector<unsigned> DelayTime;         //֡��
		int PicNum;                         //֡��
		int CurrentPicNo = 0;
		int CurrentFlash = 1;
	protected:
		void draw() override;
		int handle(int event);
	private:
	};
}
