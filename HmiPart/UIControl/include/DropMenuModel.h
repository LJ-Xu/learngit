/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DropMenuModel.h
 * Author   : qiaodan
 * Date     : 2020-10-19
 * Descript : 关于下拉菜单内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "DropMenuConfig.h"
#include "Logger.h"
namespace UI
{
	class DropMenuModel : public BaseModel
	{
	public:
		Project::DropMenuConfig MenuConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = MenuConfig.X + MenuConfig.OffX;
			rect.Y = MenuConfig.Y + MenuConfig.OffY;
			rect.W = MenuConfig.Width;
			rect.H = MenuConfig.Height;
		}

		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &MenuConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(MenuConfig)>(data, MenuConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(MenuConfig)>(MenuConfig);
		}
	};
}
