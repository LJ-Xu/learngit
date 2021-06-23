/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TimerModel.h
 * Author   : qiaodan
 * Date     : 2021-04-01
 * Descript : 关于定时器的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "TimerConfig.h"
#include "Logger.h"
namespace UI
{
	class TimerModel : public BaseModel
	{
	public:
		Project::TimerConfig TimerConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = TimerConfig.X + TimerConfig.OffX;
			rect.Y = TimerConfig.Y + TimerConfig.OffY;
			rect.W = TimerConfig.Width;
			rect.H = TimerConfig.Height;
		}
		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &TimerConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(TimerConfig)>(data, TimerConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(TimerConfig)>(TimerConfig);
		}
	};

}

