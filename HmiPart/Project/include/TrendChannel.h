/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TrendChannel.h
 * Author   : qiaodan
 * Date     : 2020-12-17
 * Descript : 用于存趋势图通道控件相关参数的类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "DataVarInfo.h"
#include "BaseLine.h"
#include <string>
namespace Project
{
	enum TrendLineType:int {TrendLine,TrendCurve,TrendDot,TrendLineDot};
	class TrendChannel
	{
	public:
		int ChannelNo;				//采样通道
		double TrendMax;				//采样最大值
		double TrendMin;				//采样最小值
		DataVarId TrendMaxVarId;	//指定采样最大值的寄存器
		DataVarId TrendMinVarId;	//指定采样最小值的寄存器
		BaseLine TrendStyle;		//趋势图线条样式
		std::string DataType;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ChannelNo, TrendMax, TrendMin,
				TrendMaxVarId, TrendMinVarId, TrendStyle, DataType);
		}

		void Parse(rapidjson::Value& jsonObj, int gname, int gno);
		static void Parse(std::vector<TrendChannel>& vector, rapidjson::Value& jsonObj, int gname, int gno);
	};
}



