/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : NoticesAction.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 消息通知类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include <string>
#include <vector>
#include "DataVarInfo.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "CerealCommon.hpp"
namespace Project
{
	enum NotifyBitMode :char { SetON, SetOFF, Rising, Falling };

	struct BitNotices
	{
		DataVarId DataBitVar;
		NotifyBitMode State;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(DataBitVar, State);
		}
	};

	struct WordNotices
	{
		DataVarId DataWordVar;
		double Val;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(DataWordVar, Val);
		}
	};
	struct NoticesAction
	{
		BitNotices Bit;
		WordNotices	Word;

		void Parse(rapidjson::Value& jsonObj);
		static void Parse(std::vector<NoticesAction>& vector, rapidjson::Value& jsonObj);
		
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Bit, Word);
		}
	};

}

