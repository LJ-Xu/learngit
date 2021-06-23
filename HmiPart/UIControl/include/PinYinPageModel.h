/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PinYinPageModel.h
 * Author   : qiaodan
 * Date     : 2021-01-21
 * Descript : 关于拼音界面内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "PinYinPageConfig.h"
namespace UI
{
	class PinYinPageModel : public BaseModel
	{
	public:
		Project::PinYinPageConfig PinYinConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = PinYinConfig.X + PinYinConfig.OffX;
			rect.Y = PinYinConfig.Y + PinYinConfig.OffY;
			rect.W = PinYinConfig.Width;
			rect.H = PinYinConfig.Height;
		}
		void InitData(std::string json) override;
		void InitPinYinData(int x = 0, int y = 0);
		void* GetTestConfig()
		{
			return &PinYinConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(PinYinConfig)>(data, PinYinConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(PinYinConfig)>(PinYinConfig);
		}
	};
}

