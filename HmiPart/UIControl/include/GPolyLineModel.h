/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GPolyLineModel.h
 * Author   : qiaodan
 * Date     : 2020-10-19
 * Descript : 关于折线内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "GPolyLineConfig.h"
namespace UI
{
	class GPolyLineModel : public BaseModel
	{
	public:
		Project::GPolyLineConfig PolyLineConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = PolyLineConfig.X + PolyLineConfig.OffX;
			rect.Y = PolyLineConfig.Y + PolyLineConfig.OffY;
			rect.W = PolyLineConfig.Width;
			rect.H = PolyLineConfig.Height;
		}

		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &PolyLineConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(PolyLineConfig)>(data, PolyLineConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(PolyLineConfig)>(PolyLineConfig);
		}
	};

}