/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BtnFunctionView.h
 * Author   : qiaodan
 * Date     : 2020-10-28
 * Descript : ���ڹ��ܰ����Ļ���ͷ�ļ������Ƽ��̰���
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include <stdafx.h>
#include <FL/Fl_Button.H>
#include <BasicView.h>
#include "BtnFunctionModel.h"
#include "BtnFunctionControl.h"

using namespace std;
namespace UI
{
	enum FunctionAct { PRESS, RELEASE };
	class BtnFunctionView : public HMIBaseButton
	{
	public:
		BtnFunctionView(int X, int Y, int W, int H, const char *L);
		~BtnFunctionView();

		/*******************************************************************************
		 * Name     : handle
		 * Descript : ���ڴ�������Ӧ�¼�
		 * Input    : event - �¼�����
		 * Output   : 1	- ����ɹ�
		 * Note	    :
		 *******************************************************************************/
		int handle(int event);
		BasicView BaseView;
		bool IsReleased = true;
		bool HandleOperatePush();
		bool HaveOperateLimit = false;
		int CurrentStatus = 0;

	protected:
		void draw() override;

	};
}


