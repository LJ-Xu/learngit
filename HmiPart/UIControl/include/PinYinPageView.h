#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PinYinPageView.h
 * Author   : qiaodan
 * Date     : 2021-01-21
 * Descript : 关于拼音界面的绘制头文件，绘制拼音界面
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "BasicView.h"
#include <stdafx.h>
namespace UI
{
	class PinYinPageView : public HMIBaseGroup
	{
	public:
		PinYinPageView(int X, int Y, int W, int H, const char* l);
		~PinYinPageView();

		int handle(int event);
		BasicView BaseView;

		int SelectedBtn;					//被选中的按键
		string PinYin;						//拼音字符串
		vector<string> ChineseString;		//中文字符串

		bool EnablePreBtn = true;
		bool EnableNextBtn = true;
		
		static void SetIns(PinYinPageView *view);
		static PinYinPageView *Ins();
	protected:
		void draw() override;
	private:
		int pinyinwidth_ = 0;
		static PinYinPageView *staticview_;
	};
}
