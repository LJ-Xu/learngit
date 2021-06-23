/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : FanModel.h
 * Author   : qiaodan
 * Date     : 2020-10-19
 * Descript : 关于风扇内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "FanConfig.h"
#include "Logger.h"

namespace UI
{
	class FanModel : public BaseModel
	{
	public:
		Project::FanConfig FanUnitConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = FanUnitConfig.X + FanUnitConfig.OffX;
			rect.Y = FanUnitConfig.Y + FanUnitConfig.OffY;
			rect.W = FanUnitConfig.Width;
			rect.H = FanUnitConfig.Height;
		}

		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &FanUnitConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(FanUnitConfig)>(data, FanUnitConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(FanUnitConfig)>(FanUnitConfig);
		}
	};
}
