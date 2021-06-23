/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GArcModel.h
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : ����Բ�����Ʋ������ݵ�ͷ�ļ�������json����������Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "GArcConfig.h"
namespace UI
{
	class GArcModel : public BaseModel
	{
	public:
		Project::GArcConfig ArcConfig;

		void InitData(std::string json);
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = ArcConfig.X + ArcConfig.OffX;
			rect.Y = ArcConfig.Y + ArcConfig.OffY;
			rect.W = ArcConfig.Width;
			rect.H = ArcConfig.Height;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(ArcConfig)>(data,ArcConfig);
		}
		void* GetTestConfig()
		{
			return &ArcConfig;
		}
		std::unique_ptr<std::vector<char>> GetData()
		{
			
			return BaseModel::GetData<decltype(ArcConfig)>(ArcConfig);
		}
		
	};

}