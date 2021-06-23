/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeypadModel.h
 * Author   : qiaodan
 * Date     : 2020-10-22
 * Descript : 关于按键绘制参数内容的头文件，解析json，存放相关信息
 * Version  : V0.1s
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "KeypadConfig.h"
#include "Logger.h"
namespace UI
{
	class KeypadModel : public BaseModel
	{
	public:
		Project::KeypadConfig KeypadConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = KeypadConfig.X + KeypadConfig.OffX;
			rect.Y = KeypadConfig.Y + KeypadConfig.OffY;
			rect.W = KeypadConfig.Width;
			rect.H = KeypadConfig.Height;
		}
		void InitData(std::string json) override;

		void* GetTestConfig()
		{
			return &KeypadConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(KeypadConfig)>(data, KeypadConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(KeypadConfig)>(KeypadConfig);
		}
	};
}
