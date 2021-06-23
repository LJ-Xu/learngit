/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GBtnPilotLampConfig.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于指示灯按键内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "GBtnPilotLampConfig.h"
#include "Logger.h"

namespace UI
{
	class BtnPilotLampModel : public BaseModel
	{
	public:
		Project::GBtnPilotLampConfig BtnPilotLampConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = BtnPilotLampConfig.X + BtnPilotLampConfig.OffX;
			rect.Y = BtnPilotLampConfig.Y + BtnPilotLampConfig.OffY;
			rect.W = BtnPilotLampConfig.Width;
			rect.H = BtnPilotLampConfig.Height;
		}
		void InitData(std::string json) override;

		void* GetTestConfig()
		{
			return &BtnPilotLampConfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(BtnPilotLampConfig)>(data, BtnPilotLampConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(BtnPilotLampConfig)>(BtnPilotLampConfig);
		}
	};

}

