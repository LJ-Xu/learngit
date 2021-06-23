/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TrendChartModel.h
 * Author   : qiaodan
 * Date     : 2020-12-03
 * Descript : 关于趋势图绘制参数内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "TrendChartConfig.h"
#include "Logger.h"

namespace UI
{
	class TrendChartModel : public BaseModel
	{
	public:
		Project::TrendChartConfig ChartTrendConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = ChartTrendConfig.X + ChartTrendConfig.OffX;
			rect.Y = ChartTrendConfig.Y + ChartTrendConfig.OffY;
			rect.W = ChartTrendConfig.Width;
			rect.H = ChartTrendConfig.Height;
		}
		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &ChartTrendConfig;
		}

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(ChartTrendConfig)>(data, ChartTrendConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(ChartTrendConfig)>(ChartTrendConfig);
		}
	};

}

