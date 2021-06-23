/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GRectangleModel.h
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : 关于矩形绘制参数内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "GRectangleConfig.h"
namespace UI
{
	class GRectangleModel : public BaseModel
	{
	public:
		Project::GRectangleConfig RectangleConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = RectangleConfig.X + RectangleConfig.OffX;
			rect.Y = RectangleConfig.Y + RectangleConfig.OffY;
			rect.W = RectangleConfig.Width;
			rect.H = RectangleConfig.Height;
		}

		void InitData(std::string jstr) override;
		void* GetTestConfig()
		{
			return &RectangleConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(RectangleConfig)>(data, RectangleConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(RectangleConfig)>(RectangleConfig);
		}
	};
}
