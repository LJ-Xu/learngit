/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DigitalDisplayModel.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于数值显示框内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#pragma once
#include "BaseModel.h"
#include "DigitalDisplayConfig.h"
#include "Logger.h"
namespace UI
{
	class DigitalDisplayModel : public BaseModel
	{
	public:
		Project::DigitalDisplayConfig DisNumConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = DisNumConfig.X + DisNumConfig.OffX;
			rect.Y = DisNumConfig.Y + DisNumConfig.OffY;
			rect.W = DisNumConfig.Width;
			rect.H = DisNumConfig.Height;
		}
		void InitData(std::string json) override;

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(DisNumConfig)>(data, DisNumConfig);
		}
		void* GetTestConfig()
		{
			return &DisNumConfig;
		}
		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(DisNumConfig)>(DisNumConfig);
		}
	};

}

class DigitalDisplayModel
{
};

