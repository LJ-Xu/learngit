#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ChineseInputView.h
 * Author   : qiaodan
 * Date     : 2020-10-19
 * Descript : 关于中文输入的绘制头文件，绘制显示框和文字、背景图片
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
	class ChineseInputView : public InputView
	{
	public:
		ChineseInputView(int X, int Y, int W, int H, const char* l);
		~ChineseInputView();
		void FoucsNextSequence();
		void InitStartInput();			//开始输入
		void EndInput();				//结束输入
		bool HandleOperatePerm();
		bool HandleOperateConfirm();
		void EnterValue();

		BasicView BaseView;
		string DisplayValue;
		string InputString;
		bool HasPinYinpage = false;		//是否打开拼音页面
		string PinYin;
		int MaxSize = 16;

		int handle(int event);
	protected:
		void draw() override;

	};
}