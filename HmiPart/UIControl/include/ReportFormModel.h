/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PilotLampModel.h
 * Author   : qiaodan
 * Date     : 2020-11-27
 * Descript : //TODO
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "ReportFormConfig.h"
#include "Logger.h"

namespace UI
{
	class ReportFormModel : public BaseModel
	{
	public:
		Project::ReportFormConfig ReportConfig;

		void GetRect(UI::Rectangle& rect)
		{
			rect.X = ReportConfig.X + ReportConfig.OffX;
			rect.Y = ReportConfig.Y + ReportConfig.OffY;
			rect.W = ReportConfig.Width;
			rect.H = ReportConfig.Height;
		}
		void* GetTestConfig()
		{
			return &ReportConfig;
		}
		void InitData(std::string json) override;
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(ReportConfig)>(data, ReportConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(ReportConfig)>(ReportConfig);
		}
	};
}

