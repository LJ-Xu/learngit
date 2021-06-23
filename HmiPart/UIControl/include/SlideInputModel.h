/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : SlideInputModel.h
 * Author   : qiaodan
 * Date     : 2020-12-17
 * Descript : 关于滑动输入绘制参数内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "SlideInputConfig.h"
#include "Logger.h"

namespace UI
{
	class SlideInputModel : public BaseModel
	{
	public:
		Project::SlideInputConfig SlideInputConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = SlideInputConfig.X + SlideInputConfig.OffX;
			rect.Y = SlideInputConfig.Y + SlideInputConfig.OffY;
			rect.W = SlideInputConfig.Width;
			rect.H = SlideInputConfig.Height;
		}
		void InitData(std::string json) override;

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(SlideInputConfig)>(data, SlideInputConfig);
		}
		void* GetTestConfig()
		{
			return &SlideInputConfig;
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(SlideInputConfig)>(SlideInputConfig);
		}
	};

}