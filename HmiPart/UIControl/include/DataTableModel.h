/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DataTableModel.h
 * Author   : qiaodan
 * Date     : 2021-03-26
 * Descript : //TODO
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "DataTableConfig.h"
#include "Logger.h"
namespace UI
{
	class DataTableModel : public BaseModel
	{
	public:
		Project::DataTableConfig DataConfig;

		void GetRect(UI::Rectangle& rect)
		{
			rect.X = DataConfig.X + DataConfig.OffX;
			rect.Y = DataConfig.Y + DataConfig.OffY;
			rect.W = DataConfig.Width;
			rect.H = DataConfig.Height;
		}
		void* GetTestConfig()
		{
			return &DataConfig;
		}
		void InitData(std::string json) override;
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(DataConfig)>(data, DataConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{
			return BaseModel::GetData<decltype(DataConfig)>(DataConfig);
		}
	};
}