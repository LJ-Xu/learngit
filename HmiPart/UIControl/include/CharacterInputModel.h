/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : CharacterInputModel.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于字符输入框内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "CharacterInputConfig.h"
#include "Logger.h"
namespace UI
{
	class CharacterInputModel : public BaseModel
	{
	public:
		Project::CharacterInputConfig InputStrConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = InputStrConfig.X + InputStrConfig.OffX;
			rect.Y = InputStrConfig.Y + InputStrConfig.OffY;
			rect.W = InputStrConfig.Width;
			rect.H = InputStrConfig.Height;
		}

		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &InputStrConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(InputStrConfig)>(data, InputStrConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(InputStrConfig)>(InputStrConfig);
		}
	};
}