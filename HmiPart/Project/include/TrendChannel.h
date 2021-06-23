/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TrendChannel.h
 * Author   : qiaodan
 * Date     : 2020-12-17
 * Descript : ���ڴ�����ͼͨ���ؼ���ز�������
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
		int ChannelNo;				//����ͨ��
		double TrendMax;				//�������ֵ
		double TrendMin;				//������Сֵ
		DataVarId TrendMaxVarId;	//ָ���������ֵ�ļĴ���
		DataVarId TrendMinVarId;	//ָ��������Сֵ�ļĴ���
		BaseLine TrendStyle;		//����ͼ������ʽ
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



