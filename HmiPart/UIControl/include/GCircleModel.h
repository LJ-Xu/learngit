/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GCircleModel.h
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : 关于圆绘制参数内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "GCircleConfig.h"
namespace UI
{
	class GCircleModel : public BaseModel
	{
	public:
		Project::GCircleConfig CircleConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = CircleConfig.X + CircleConfig.OffX;
			rect.Y = CircleConfig.Y + CircleConfig.OffY;
			rect.W = CircleConfig.Width;
			rect.H = CircleConfig.Height;
		}

		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &CircleConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(CircleConfig)>(data, CircleConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(CircleConfig)>(CircleConfig);
		}
	};

}