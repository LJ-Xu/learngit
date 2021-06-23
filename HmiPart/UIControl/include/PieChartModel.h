/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PieChartModel.h
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : ��ͼ�ؼ���������ͷ�ļ�
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "PieChartConfig.h"
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;
using namespace rapidjson;

namespace UI
{
	class PieChartModel : public BaseModel
	{
	public:
		PieChartModel();
		~PieChartModel();

	public:
		void GetRect(Rectangle & rect);
		void InitTestData();
		void * GetTestConfig();
		void InitData(string jsonStr);
		void InitData(vector<char>);
		unique_ptr<vector<char>> GetData();

	public:
		Project::PieChartConfig PieChartUnit;
	};
}

