/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PilotLampModel.h
 * Author   : qiaodan
 * Date     : 2020-10-27
 * Descript : 关于指示灯绘制参数内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "JsonComm.h"
#include "PilotLampConfig.h"
#include "Logger.h"
namespace UI
{
	class PilotLampModel : public BaseModel
	{
	public:
		Project::PilotLampConfig PLampconfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = PLampconfig.X + PLampconfig.OffX;
			rect.Y = PLampconfig.Y + PLampconfig.OffY;
			rect.W = PLampconfig.Width;
			rect.H = PLampconfig.Height;
		}

		void InitData(std::string json) override;
		void* GetTestConfig()
		{
			return &PLampconfig;
		}
		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(PLampconfig)>(data, PLampconfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(PLampconfig)>(PLampconfig);
		}
	};
}


