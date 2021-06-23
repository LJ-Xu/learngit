/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RecipeInfoRes
 * Author   : qiaodan
 * Date     : 2021-02-03
 * Descript : 配方类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "DataVarInfo.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include <string>
#include <vector>
namespace Project
{
	class RecipeInfoRes: public ColDataTypeInfo
	{
	public:
		int IntegerNum;
		int DecimalNum;
		bool Editable;					//是否可编辑
		//std::vector<std::string> RecipeData;		//当前列数据
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ColTitle, DataType, DataFmt, IntegerNum, DecimalNum,
				RegCount, Editable/*, RecipeData*/);
		}

		void Parse(rapidjson::Value& jsonObj);
		static void Parse(std::vector<RecipeInfoRes>& vector, rapidjson::Value& jsonObj);
	};
}
