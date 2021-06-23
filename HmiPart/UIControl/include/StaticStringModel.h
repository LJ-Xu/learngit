/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StaticStringModel.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于静态文字绘制参数内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "StaticStringConfig.h"
#include "Logger.h"

namespace UI
{
	class StaticStringModel : public BaseModel
	{
	public:
		Project::StaticStringConfig StringConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = StringConfig.X + StringConfig.OffX
					 - StringConfig.Border.BorderSize / 2;
			rect.Y = StringConfig.Y + StringConfig.OffY
					 - StringConfig.Border.BorderSize / 2;
			rect.W = StringConfig.Width + StringConfig.Border.BorderSize;
			rect.H = StringConfig.Height + StringConfig.Border.BorderSize;
		}
		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &StringConfig;
		}

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(StringConfig)>(data, StringConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(StringConfig)>(StringConfig);
		}
	};

}