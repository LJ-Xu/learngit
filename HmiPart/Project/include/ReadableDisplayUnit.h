/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ReadableDisplayUnit.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : //TODO
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "StringStyle.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "ImageResId.h"
namespace Project
{
	class WarnStyle
	{
	public:
		int Color;		//颜色
		bool Flash;		//闪动

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Color, Flash);
		}
		void Parse(rapidjson::Value& jsonObj)
		{
			if (jsonObj.HasMember("Color"))
				Color = jsonObj["Color"].GetInt();
			if (jsonObj.HasMember("Flash"))
				Flash = jsonObj["Flash"].GetBool();
		}
	};

	class LinearParam
	{
	public:
		float A;	// 线型比例转化比例
		float B;	// 线型比例转化偏移

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(A, B);
		}

	};

	class AccuracyParam
	{
	public:
		int Act;//整数部分有效位
		int Bct;//小数部分有效位

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Act, Bct);
		}

	};

	class DataVarOrManual
	{
	public:
		DOUBLE Num;			// 输入常数
		DataVarId DataVarLmt;		// 寄存器获取

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Num, DataVarLmt);
		}

		void Parse(rapidjson::Value& jsonObj)
		{
			if (jsonObj.HasMember("Num"))
				Num = jsonObj["Num"].GetFloat();
			if (jsonObj.HasMember("VarIdRef") && jsonObj["VarIdRef"].IsObject())
				DataVarLmt.Parse(jsonObj["VarIdRef"]);
		}
	};

	class ReadableDisplayUnit : public MoveableDisplayUnit
	{
	public:
		DataVarId ReadVar;				// 读取寄存器 
		ImageResId Key;					// 获取图片的key值
		StringStyle TextStyle;			// 字体样式
		string FmtStr;					//初始文字串
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ReadVar, Key, TextStyle, FmtStr);
		}
	};

}