/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DigitalInputModel.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于数值输入框内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "DigitalInputConfig.h"
#include "Logger.h"
namespace UI
{
	class DigitalInputModel : public BaseModel
	{
	public:
		Project::DigitalInputConfig InputNumConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = InputNumConfig.X + InputNumConfig.OffX;
			rect.Y = InputNumConfig.Y + InputNumConfig.OffY;
			rect.W = InputNumConfig.Width;
			rect.H = InputNumConfig.Height;
		}

		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &InputNumConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(InputNumConfig)>(data, InputNumConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(InputNumConfig)>(InputNumConfig);
		}
	};

}
