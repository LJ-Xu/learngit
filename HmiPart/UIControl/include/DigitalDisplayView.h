/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DigitalDisplayView.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : �����ַ���ʾ��Ļ���ͷ�ļ���������ʾ������֡�����ͼƬ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BasicView.h"
#include <stdafx.h>
#include <FL/Fl_Input.H>

//#include "DigitalInputView.h"
namespace UI
{
	class InputView : public HMIBaseInput
	{
	public:
		InputView(int X, int Y, int W, int H, const char* l) :HMIBaseInput(X, Y, W, H, l) {};
		~InputView() {};

		/*******************************************************************************
		 * Name     : drawtext
		 * Descript : ���ڻ������������򱳾�������
		 * Input    : X		-	 ��������Ͻ�x����
		 *			  Y		-	 ��������Ͻ�y����
		 *			  W		-	 �������
		 *			  H		-	 ����򳤶�
		 *			  align	-	 �����������ʾ���뷽ʽ����ֵ����Fl_Align
		 *			  img	-	 ����򱳾�ͼƬ��Դ
		 * Output   :
		 * Note	    :
		 *******************************************************************************/
		void drawtext(int X, int Y, int W, int H, int align, Fl_Image *img, bool usepassword = false, bool hidetext = false);
		const char* expand(const char*, char*, char *) const;
		double expandpos(const char*, const char*, const char*, int*) const;
		bool OpenKeypage = false;				//�Ƿ�򿪼���ҳ��
		bool NeedHandleFocus = false;					//�Ƿ���Ҫ������
//		DataVarId BitVid = DataVarId::NullId;
		//bool IsSearchFirst = false;						//�Ƿ��ڽ�����Ѱ
	};

	class DigitalDisplayView: public InputView
	{
	public:
		DigitalDisplayView(int X, int Y, int W, int H, const char* l);
		~DigitalDisplayView();
		BasicView BaseView;
		string DisplayValue;
		bool IsExceedMaxLmt = false;	//�������ֵ
		bool IsExceedMinLmt = false;	//������Сֵ
		bool IsHideText = false;		//������ֵ��˸
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
