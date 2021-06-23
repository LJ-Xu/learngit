/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BackLightModel.h
 * Author   : qiaodan
 * Date     : 2021-04-03
 * Descript : 关于背景灯内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "BackLightConfig.h"
#include "Logger.h"

namespace UI
{
	class BackLightModel : public BaseModel
	{
	public:
		Project::BackLightConfig BackLightConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = BackLightConfig.X + BackLightConfig.OffX;
			rect.Y = BackLightConfig.Y + BackLightConfig.OffY;
			rect.W = BackLightConfig.Width;
			rect.H = BackLightConfig.Height;
		}

		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &BackLightConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(BackLightConfig)>(data, BackLightConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(BackLightConfig)>(BackLightConfig);
		}
	};
}