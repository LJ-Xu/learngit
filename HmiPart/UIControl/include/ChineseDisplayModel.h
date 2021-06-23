/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ChineseBtnModel.h
 * Author   : qiaodan
 * Date     : 2020-10-15
 * Descript : 关于中文显示框内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "ChineseDisplayConfig.h"
#include "Logger.h"
namespace UI
{
	class ChineseDisplayModel : public BaseModel
	{
	public:
		Project::ChineseDisplayConfig DisChineseConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = DisChineseConfig.X + DisChineseConfig.OffX;
			rect.Y = DisChineseConfig.Y + DisChineseConfig.OffY;
			rect.W = DisChineseConfig.Width;
			rect.H = DisChineseConfig.Height;
		}
		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &DisChineseConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(DisChineseConfig)>(data, DisChineseConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(DisChineseConfig)>(DisChineseConfig);
		}
	};
}
