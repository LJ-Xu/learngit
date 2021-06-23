/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
#include <FL/Fl_Shared_Image.H>
 * Name     : DigitalInputView.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : �����ַ������Ļ���ͷ�ļ���������ʾ������֡�����ͼƬ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BasicView.h"
#include <stdafx.h>
#include "DigitalDisplayView.h"
namespace UI
{
	
	class DigitalInputView : public InputView
	{
	public:
		DigitalInputView(int X, int Y, int W, int H, const char* l);
		~DigitalInputView();
		void FoucsNextSequence();
		void InitStartInput();			//��ʼ����
		void EndInput();				//��������
		bool HandleOperatePerm(); 
		bool HandleOperateConfirm();
		void EnterValue();

		/*******************************************************************************
		 * Name     : handle
		 * Descript : ��дFl_Input�й����¼���Ӧ������
		 * Input    : event	- �¼����
		 * Output   : 1 - �����¼��ɹ�
		 * Note	    :
		 *******************************************************************************/
		int handle(int event);

		BasicView BaseView;
		string DisplayValue;
		bool IsExceedMaxLmt = false;	//�������ֵ
		bool IsExceedMinLmt = false;	//������Сֵ
		bool IsHideText = false;		//������ֵ��˸

		string OldValue;				//������Ű�������ǰ����
		double MaxValue;
		double MinValue;
		double InputValue = 0;
	protected:
		void draw() override;
	};
}
