/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GDynPicView.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ���ڶ�̬ͼƬ����ͷ�ļ�
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
		vector<Fl_Image*> PicImages;	//��Ҫ��ʾ��ͼƬ
		void InitDraw();				//��ʼ������
		void SortPic();					//ͼƬ����
		void SortPicKey();				//ͼƬkeyzֵ�������ڷǻ�����ʾ��
		void GetPicRes();				//��ȡͼƬ��Դ

	protected:
		void draw() override;
		int handle(int event);
	};
}

