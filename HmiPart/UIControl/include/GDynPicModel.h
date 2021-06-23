/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GDynPicModel.h
 * Author   : qiaodan
 * Date     : 2020-10-19
 * Descript : 关于动态图片内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "GDynPicConfig.h"
namespace UI
{
	class GDynPicModel : public BaseModel
	{
	public:
		Project::GDynPicConfig PicDynConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = PicDynConfig.X + PicDynConfig.OffX;
			rect.Y = PicDynConfig.Y + PicDynConfig.OffY;
			rect.W = PicDynConfig.Width;
			rect.H = PicDynConfig.Height;
		}

		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &PicDynConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(PicDynConfig)>(data, PicDynConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(PicDynConfig)>(PicDynConfig);
		}
	};
}