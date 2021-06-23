/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ChineseInputModel.h
 * Author   : qiaodan
 * Date     : 2020-10-19
 * Descript : 关于中文输入框内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "ChineseInputConfig.h"
#include "Logger.h"
namespace UI
{
	class ChineseInputModel : public BaseModel
	{
	public:
		Project::ChineseInputConfig InputChineseConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = InputChineseConfig.X + InputChineseConfig.OffX;
			rect.Y = InputChineseConfig.Y + InputChineseConfig.OffY;
			rect.W = InputChineseConfig.Width;
			rect.H = InputChineseConfig.Height;
		}

		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &InputChineseConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(InputChineseConfig)>(data, InputChineseConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(InputChineseConfig)>(InputChineseConfig);
		}
	};
}


