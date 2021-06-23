#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : SlideInputView.h
 * Author   : qiaodan
 * Date     : 2020-12-17
 * Descript : ���ڻ�������Ļ���ͷ�ļ�
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "BasicView.h"
#include <stdafx.h>
#include <FL/Fl_Box.H>
#include <FL/Fl_Scrollbar.H>
#include "Point.h"
namespace UI
{
	//void scroll_cb(Fl_Widget*, void*);	// hscrollbar callback
	class SlideInputView : public HMIBaseSlider
	{
	public:
		SlideInputView(int X, int Y, int W, int H, const char* l);
		~SlideInputView();

		/*��ʼ������ʹ�õ�һЩ����*/
		void InitDraw();
		/*���ƿ̶ȼ���ע*/
		void DrawScale();
		void DrawXScale();
		void DrawYScale();

		/*ת����ע��ʽ*/
		string ChangeDisplayFormat(double value,int mode);
		/*����ֵд�뵽�Ĵ���*/
		void SetReadVar();
		bool HandleOperatePush();
		int handle(int event);
		BasicView BaseView;
	protected:
		void draw() override;
	private:
		/*����ʹ�ó���*/
		Fl_Color slideRailBackColor_;		//���챳����ɫ
		Fl_Color slideRailFrameColor_;		//����߿���ɫ
		Fl_Color slideBackColor_;			//���鱳����ɫ
		Fl_Color slideFrameColor_;			//����߿���ɫ
		Project::Point axispos_;			//����λ��
		int axislength_;					//���߳���
		Fl_Color axisColor_;				//������ɫ
		int primaryScaleLen_;				//���̶ȳ���
		int secondScaleLen_;				//���̶ȳ���		
		Fl_Font scalefont_;					//��ע����
		Fl_Color scalecolor_;				//��ע��ɫ
		int scalesize_;						//��ע��С
		int slideW_;						//�����/��
	};
}




