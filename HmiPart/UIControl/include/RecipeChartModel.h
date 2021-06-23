/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RecipeChartModel.h
 * Author   : qiaodan
 * Date     : 2021-02-04
 * Descript : 关于配方表内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "Logger.h"
#include "RecipeChartConfig.h"
namespace UI
{
	class RecipeChartModel : public BaseModel
	{
	public:
		Project::RecipeChartConfig RecipeConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = RecipeConfig.X + RecipeConfig.OffX;
			rect.Y = RecipeConfig.Y + RecipeConfig.OffY;
			rect.W = RecipeConfig.Width;
			rect.H = RecipeConfig.Height;
		}
		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &RecipeConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(RecipeConfig)>(data, RecipeConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(RecipeConfig)>(RecipeConfig);
		}
	};
}

