/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : InputNumShowUnit.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : //TODO
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "ReadableDisplayUnit.h"
#include "KeyboardRes.h"
#include "MacroRes.h"
namespace Project
{
	
	class InputMethod
	{
	public:
		int Method;				//�������뷽ʽ ���� : 0 ͨ��λ���� : 1
		DataVarId KeypadVar;	// λ���ƼĴ���
		int KeypadSrc;			//���̵���Դ ���� : 1 USB���룬ϵͳ����̵� : 0
		KeyboardRes Keypad;		//ʹ�õ�������

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Method, KeypadVar, KeypadSrc, Keypad);
		}

		void Parse(rapidjson::Value& keyMethod)
		{
			if (keyMethod.HasMember("Method"))
				Method = keyMethod["Method"].GetInt();
			if (keyMethod.HasMember("KeypadVar") && keyMethod["KeypadVar"].IsObject())
				KeypadVar.Parse(keyMethod["KeypadVar"]);
			if (keyMethod.HasMember("KeypadSrc"))
				KeypadSrc = keyMethod["KeypadSrc"].GetInt();
			if (keyMethod.HasMember("KeypadRes") && keyMethod["KeypadRes"].IsObject())
			{
				if (keyMethod["KeypadRes"].HasMember("KeyPadIdx"))
					Keypad.KeypadIdx = keyMethod["KeypadRes"]["KeyPadIdx"].GetInt();
				if (keyMethod["KeypadRes"].HasMember("KeyPadPos"))
				{
					rapidjson::Value &keypadPos = keyMethod["KeypadRes"]["KeyPadPos"];
					if (!keypadPos.IsNull()) {
						if (keypadPos.HasMember("X"))
							Keypad.KeypadPos.X = keypadPos["X"].GetInt();
						if (keypadPos.HasMember("Y"))
							Keypad.KeypadPos.Y = keypadPos["Y"].GetInt();
					}
				}

			}
		}
	};

	class InputSequence
	{
	public:
		bool CancelSequenceAfterCompleted;		//������ٰ���������
		int	 SequenceId;		//�������
		int  GroupId;			//Ⱥ�����

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(CancelSequenceAfterCompleted, SequenceId, GroupId);
		}

	};

	class Notify
	{
	public:
		bool NotifiedUsed;	// ����֪ͨ�б�
		DataVarId Data;		// ֪ͨ������
		int Val;			// ��ȡ����
		int Mode;			// д��ǰִ��: 0   д���ִ��: 1
		int State;			// ����״̬ ��OFF : 0 ��ON : 1

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(NotifiedUsed, Data, Val, Mode, State);
		}

	};

	class InputNumShowUnit:public ReadableDisplayUnit
	{
	public:
		int PswMode;					//������ʽ
		bool IsFillZero;				//�Ƿ�ʹ��ǰ��0
		AccuracyParam Accuracy;			//С��������λ

		bool IsWarnUpperUsed;			//�Ƿ�ʹ�ñ�������
		DataVarOrManual WarnUpper;		//������������
		bool IsWarnLowerUsed;			//�Ƿ�ʹ�ñ�������
		DataVarOrManual WarnLower;		//������������
		bool IsWarnStyleUsed;			//�Ƿ�ʹ�ñ�����ʽ
		WarnStyle WarnStyleUpper;		//����������ʽ
		WarnStyle WarnStyleLower;		//����������ʽ

		bool IsConvertUsed;				//�Ƿ�ʹ�ñ���ת��
		DataVarOrManual NovUpperSrc;	//����ת������Դ����
		DataVarOrManual NovLowerSrc;	//����ת������Դ����
		DataVarOrManual NovUpperRes;	//����ת��ת��ֵ����
		DataVarOrManual NovLowerRes;	//����ת��ת��ֵ����
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(PswMode, IsFillZero, Accuracy, IsWarnUpperUsed, WarnUpper,
				IsWarnLowerUsed, WarnLower, IsWarnStyleUsed, WarnStyleUpper, WarnStyleLower,
				IsConvertUsed, NovUpperSrc, NovLowerSrc, NovUpperRes, NovLowerRes);
		}
	};
}