/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : OperatorChartModel.h
 * Author   : qiaodan
 * Date     : 2021-05-11
 * Descript : //TODO
 * Version  : V0.1.6
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "OperatorChartConfig.h"
#include "Logger.h"
namespace UI
{
	class OperatorChartModel : public BaseModel
	{
	public:
		Project::OperatorChartConfig OperatorConfig;

		void GetRect(UI::Rectangle& rect)
		{
			rect.X = OperatorConfig.X + OperatorConfig.OffX;
			rect.Y = OperatorConfig.Y + OperatorConfig.OffY;
			rect.W = OperatorConfig.Width;
			rect.H = OperatorConfig.Height;
		}
		void* GetTestConfig()
		{
			return &OperatorConfig;
		}
		void InitData(std::string json) override;
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(OperatorConfig)>(data, OperatorConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(OperatorConfig)>(OperatorConfig);
		}
	};
}
