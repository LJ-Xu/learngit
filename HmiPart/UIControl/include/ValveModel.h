/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ValveModel.h
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : 阀门控件参数解析头文件
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "ValveConfig.h"
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;
using namespace rapidjson;

namespace UI 
{
	class ValveModel : public BaseModel
	{
	public:
		ValveModel();
		~ValveModel();

	public:
		void GetRect(Rectangle & rect);
		void InitTestData();
		void * GetTestConfig();
		void InitData(string jsonStr);
		void InitData(vector<char>);
		unique_ptr<vector<char>> GetData();

	public:
		Project::ValveConfig ValveUnit;	// 阀门属性
	};
}
