#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TimerView.h
 * Author   : qiaodan
 * Date     : 2021-04-01
 * Descript : »æÖÆ¶¨Ê±Æ÷
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include <stdafx.h>
#include <BasicView.h>
#include "UIComm.h"
#include "FL/Fl_Box.H"
namespace UI
{
	class TimerView : public HMIBaseLabel
	{
	public:
		TimerView(int X, int Y, int W, int H, const char *L);
		~TimerView();
		BasicView BaseView;
	protected:
		void draw();
	};
}


