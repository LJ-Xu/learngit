/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : VarStringModel.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于动态文字绘制参数内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "VarStringConfig.h"
#include "Logger.h"
namespace UI
{
	class VarStringModel : public BaseModel
	{
	public:
		Project::VarStringConfig VarStringConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = VarStringConfig.X + VarStringConfig.OffX;
			rect.Y = VarStringConfig.Y + VarStringConfig.OffY;
			rect.W = VarStringConfig.Width;
			rect.H = VarStringConfig.Height;
		}
		void* GetTestConfig()
		{
			return &VarStringConfig;
		}

		void InitData(std::string json) override;

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(VarStringConfig)>(data, VarStringConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{
			return BaseModel::GetData<decltype(VarStringConfig)>(VarStringConfig);
		}
	};
}


