/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GFuncModel.h
 * Author   : TangYao
 * Date     : 2021/02/07
 * Descript : 函数画布控件参数解析
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "GFuncConfig.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace rapidjson;

namespace UI 
{
	class GFuncModel : public BaseModel
	{
	public:
		GFuncModel();
		~GFuncModel();

	public:
		void GetRect(Rectangle & rect);
		void InitTestData();
		void * GetTestConfig();
		void InitData(string jsonStr);
		void InitData(vector<char>);
		unique_ptr<vector<char>> GetData();

	public:
		Project::GFuncConfig GFuncUnit;	// 函数画布属性
	};
}
