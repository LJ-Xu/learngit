/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RecipeTransferModel.h
 * Author   : qiaodan
 * Date     : 2021-03-02
 * Descript : 关于配方上传参数内容的头文件，解析json，存放相关信息
 * Version  : V0.1s
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "RecipeTransferConfig.h"
#include "Logger.h"

namespace UI
{
	class RecipeTransferModel : public BaseModel
	{
	public:
		Project::RecipeTransferConfig TransferRecipeConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = TransferRecipeConfig.X + TransferRecipeConfig.OffX;
			rect.Y = TransferRecipeConfig.Y + TransferRecipeConfig.OffY;
			rect.W = TransferRecipeConfig.Width;
			rect.H = TransferRecipeConfig.Height;
		}
		void InitData(std::string json) override;

		void* GetTestConfig()
		{
			return &TransferRecipeConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(TransferRecipeConfig)>(data, TransferRecipeConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(TransferRecipeConfig)>(TransferRecipeConfig);
		}
	};
}
