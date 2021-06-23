/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : CharacterDisplayModel.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于字符显示框内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "CharacterDisplayConfig.h"
#include "Logger.h"
namespace UI
{
	class CharacterDisplayModel : public BaseModel
	{
	public:
		Project::CharacterDisplayConfig DisStrConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = DisStrConfig.X + DisStrConfig.OffX;
			rect.Y = DisStrConfig.Y + DisStrConfig.OffY;
			rect.W = DisStrConfig.Width;
			rect.H = DisStrConfig.Height;
		}
		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &DisStrConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(DisStrConfig)>(data, DisStrConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(DisStrConfig)>(DisStrConfig);
		}
	};
}

