/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : MultPilotLampModel.h
 * Author   : qiaodan
 * Date     : 2020-10-27
 * Descript : 关于多状态按键绘制参数内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "MultiStatusKeypadConfig.h"
#include "Logger.h"
namespace UI
{
	class MultiStatusKeypadModel : public BaseModel
	{
	public:
		Project::MultiStatusKeypadConfig MultiStatusKeypadConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = MultiStatusKeypadConfig.X + MultiStatusKeypadConfig.OffX;
			rect.Y = MultiStatusKeypadConfig.Y + MultiStatusKeypadConfig.OffY;
			rect.W = MultiStatusKeypadConfig.Width;
			rect.H = MultiStatusKeypadConfig.Height;
		}
		void* GetTestConfig()
		{
			return &MultiStatusKeypadConfig;
		}
		void InitData(std::string json) override;

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(MultiStatusKeypadConfig)>(data, MultiStatusKeypadConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(MultiStatusKeypadConfig)>(MultiStatusKeypadConfig);
		}
	};
}

