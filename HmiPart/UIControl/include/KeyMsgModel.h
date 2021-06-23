/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeyMsgModel.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于按键消息绘制参数内容的头文件，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseModel.h"
#include "KeyMsgConfig.h"
namespace UI
{
	class KeyMsgModel : public BaseModel
	{
	public:
		Project::KeyMsgConfig KeyMsgConfig;
		void GetRect(UI::Rectangle& rect)
		{
			rect.X = KeyMsgConfig.X + KeyMsgConfig.OffX;
			rect.Y = KeyMsgConfig.Y + KeyMsgConfig.OffY;
			rect.W = KeyMsgConfig.Width;
			rect.H = KeyMsgConfig.Height;
		}
		void InitData(std::string json) override;

		void InitData(std::vector<char> data)
		{
			//return BaseModel::InitData<decltype(m_KeyMsgConfig)>(data, m_KeyMsgConfig);
		}

		std::unique_ptr<std::vector<char>> GetData()
		{

			return BaseModel::GetData<decltype(KeyMsgConfig)>(KeyMsgConfig);
		}
	};
}
