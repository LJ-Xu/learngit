/*******************************************************************************
 * Copyright (C) 2011-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * FileName	: ControlFactory.cpp
 * Author	: zhanglinbo
 * Descript	: 创建各种Control
 * Version	: 0.1
 * Modify	:
 *			:
 *******************************************************************************/
#include "stdafx.h"
#include <math.h>
#include <memory>
#include <map>
#include "GHMIPage.h"
#include "PageFactory.h"

namespace UI
{
	typedef std::unique_ptr<HMIPage>(*NewPageFunc)(int pageid, int x, int y, int w, int h, const char* title);
	template <class T>
	static std::unique_ptr<HMIPage> NewPageTmp(int pageid, int x, int y, int w, int h, const char* title)
	{
		std::unique_ptr<HMIPage> ptr(new T(pageid, x, y, w, h, title));
		return ptr;

	}
	static map<short, NewPageFunc> newPageFuncMap = {
		{0, [](int pageid, int x, int y, int w, int h, const char* title) {return NewPageTmp<HMIPage>(pageid,x,y,w,h,title); }  },//普通的PAGE
		{0xff, [](int pageid, int x, int y, int w, int h, const char* title) {return NewPageTmp<GHMIPage>(pageid,x,y,w,h,title); }  },//全局的PAGE

	};
	std::unique_ptr<UI::HMIPage> PageFactory::NewPage(short catogray, int pageid, int x, int y, int w, int h, const char* title)
	{
		if (newPageFuncMap.count(catogray))
			return newPageFuncMap[catogray](pageid, x, y, w, h, title);
		return nullptr;
	}


}
