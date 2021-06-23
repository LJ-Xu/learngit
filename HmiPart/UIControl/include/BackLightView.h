#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BackLightView.h
 * Author   : qiaodan
 * Date     : 2021-04-02
 * Descript : »æÖÆ±³¾°µÆ
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
	class BackLightView : public HMIBaseWindow
	{
	public:
		BackLightView(int X, int Y, int W, int H);
		~BackLightView();
		BasicView BaseView;
		int handle(int event);

		bool IsPutOutStatus = false;		//ÊÇ·ñÊÇÏ¨Ãð×´Ì¬
	protected:
		void draw();
	};
}

