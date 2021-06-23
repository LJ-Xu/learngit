/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : MultPilotLampModel.h
 * Author   : qiaodan
 * Date     : 2020-10-27
 * Descript : 关于多状态指示灯绘制参数内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "JsonComm.h"
#include "MultiPilotLampConfig.h"
#include "Logger.h"
namespace UI
{
	class MultiPilotLampModel : public BaseModel
	{
	public:
		Project::MultiPilotLampConfig MPLampconfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = MPLampconfig.X + MPLampconfig.OffX;
			rect.Y = MPLampconfig.Y + MPLampconfig.OffY;
			rect.W = MPLampconfig.Width;
			rect.H = MPLampconfig.Height;
		}
		void* GetTestConfig()
		{
			return &MPLampconfig;
		}
		void InitData(std::string json) override;

		void InitData(std::vector<char> data)
		{
			return BaseModel::InitData<decltype(MPLampconfig)>(data, MPLampconfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(MPLampconfig)>(MPLampconfig);
		}
	};
}
