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
		int Color;		//��ɫ
		bool Flash;		//����

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
		float A;	// ���ͱ���ת������
		float B;	// ���ͱ���ת��ƫ��

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(A, B);
		}

	};

	class AccuracyParam
	{
	public:
		int Act;//����������Чλ
		int Bct;//С��������Чλ

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Act, Bct);
		}

	};

	class DataVarOrManual
	{
	public:
		DOUBLE Num;			// ���볣��
		DataVarId DataVarLmt;		// �Ĵ�����ȡ

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
		DataVarId ReadVar;				// ��ȡ�Ĵ��� 
		ImageResId Key;					// ��ȡͼƬ��keyֵ
		StringStyle TextStyle;			// ������ʽ
		string FmtStr;					//��ʼ���ִ�
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ReadVar, Key, TextStyle, FmtStr);
		}
	};

}