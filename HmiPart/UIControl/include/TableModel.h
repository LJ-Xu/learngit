/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TableModel.h
 * Author   : qiaodan
 * Date     : 2020-12-03
 * Descript : 关于表格绘制参数内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "TableConfig.h"
#include "Logger.h"

namespace UI
{
	class TableModel : public BaseModel
	{
	public:
		Project::TableConfig TableConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = TableConfig.X + TableConfig.OffX;
			rect.Y = TableConfig.Y + TableConfig.OffY;
			rect.W = TableConfig.Width;
			rect.H = TableConfig.Height;
		}
		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &TableConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(TableConfig)>(data, TableConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(TableConfig)>(TableConfig);
		}
	};

}
