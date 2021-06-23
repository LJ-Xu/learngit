/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeyBtnModel.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于键盘按键绘制参数内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "KeyBtnConfig.h"
#include "Logger.h"

namespace UI
{
	class KeyBtnModel : public BaseModel
	{
	public:
		Project::KeyBtnConfig KeyBtnConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = KeyBtnConfig.X + KeyBtnConfig.OffX;
			rect.Y = KeyBtnConfig.Y + KeyBtnConfig.OffY;
			rect.W = KeyBtnConfig.Width;
			rect.H = KeyBtnConfig.Height;
		}
		
		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &KeyBtnConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(KeyBtnConfig)>(data, KeyBtnConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(KeyBtnConfig)>(KeyBtnConfig);
		}
	};
}


