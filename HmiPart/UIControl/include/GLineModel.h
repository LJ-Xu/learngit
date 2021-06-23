/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GLineModel.h
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : ����ֱ���Ʋ������ݵ�ͷ�ļ�������json����������Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "GLineConfig.h"
namespace UI
{
	class GLineModel : public BaseModel
	{
	public:
		Project::GLineConfig LineConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = LineConfig.X + LineConfig.OffX;
			rect.Y = LineConfig.Y + LineConfig.OffY;
			rect.W = LineConfig.Width;
			rect.H = LineConfig.Height;
		}
		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &LineConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(LineConfig)>(data, LineConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(LineConfig)>(LineConfig);
		}
	};
}
