/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : FluidUnitModel.h
 * Author   : qiaodan
 * Date     : 2020-10-19
 * Descript : 关于流体内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "FluidUnitConfig.h"
#include "Logger.h"
namespace UI
{
	class FluidUnitModel : public BaseModel
	{
	public:
		Project::FluidUnitConfig FluidConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = FluidConfig.X + FluidConfig.OffX;
			rect.Y = FluidConfig.Y + FluidConfig.OffY;
			rect.W = FluidConfig.Width;
			rect.H = FluidConfig.Height;
		}

		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &FluidConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(FluidConfig)>(data, FluidConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(FluidConfig)>(FluidConfig);
		}
	};
}
