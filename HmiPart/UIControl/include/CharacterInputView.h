/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : CharacterInputView.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : �����ַ�����Ļ���ͷ�ļ���������ʾ������֡�����ͼƬ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BasicView.h"
#include <FL/Fl_Input.H>
#include <stdafx.h>
#include "DigitalInputView.h"
namespace UI
{
	class CharacterInputView : public InputView
	{
	public:
		CharacterInputView(int X, int Y, int W, int H, const char* l);
		~CharacterInputView();
		BasicView BaseView;
		string DisplayValue;
		char *WriteValue;
		void FoucsNextSequence();
		void InitStartInput();			//��ʼ����
		void EndInput();				//��������
		void SetWriteData();
		bool HandleOperatePerm();
		bool HandleOperateConfirm();

		//bool OpenKeypage = false;		//�Ƿ�򿪼���ҳ��
		//size_t MaxSize = 16;
		/*******************************************************************************
		 * Name     : handle
		 * Descript : ��дFl_Input�й����¼���Ӧ������
		 * Input    : event	- �¼����
		 * Output   : 1 - �����¼��ɹ�
		 * Note	    :
		 *******************************************************************************/
		int handle(int event);
		string Inputstring;
	protected:
		void draw() override;

	};
}
