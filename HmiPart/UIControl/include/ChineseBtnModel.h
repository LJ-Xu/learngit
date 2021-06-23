/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ChineseBtnModel.h
 * Author   : qiaodan
 * Date     : 2020-10-15
 * Descript : 关于中文按键内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "ChineseBtnConfig.h"
namespace UI
{
	class ChineseBtnModel : public BaseModel
	{
	public:
		Project::ChineseBtnConfig BtnConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = BtnConfig.X + BtnConfig.OffX;
			rect.Y = BtnConfig.Y + BtnConfig.OffY;
			rect.W = BtnConfig.Width;
			rect.H = BtnConfig.Height;
		}
		void InitData(std::string json) override;
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(BtnConfig)>(data, BtnConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(BtnConfig)>(BtnConfig);
		}
	};

}
class ChineseBtnModel
{
};

