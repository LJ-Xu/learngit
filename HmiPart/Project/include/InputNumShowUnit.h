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
		int Method;				//键盘输入方式 触控 : 0 通过位控制 : 1
		DataVarId KeypadVar;	// 位控制寄存器
		int KeypadSrc;			//键盘的来源 弹出 : 1 USB接入，系统软键盘等 : 0
		KeyboardRes Keypad;		//使用弹出键盘

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
		bool CancelSequenceAfterCompleted;		//输入后不再按次序输入
		int	 SequenceId;		//输入次序
		int  GroupId;			//群组序号

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(CancelSequenceAfterCompleted, SequenceId, GroupId);
		}

	};

	class Notify
	{
	public:
		bool NotifiedUsed;	// 启用通知列表
		DataVarId Data;		// 通知的数据
		int Val;			// 获取输入
		int Mode;			// 写入前执行: 0   写入后执行: 1
		int State;			// 设置状态 设OFF : 0 设ON : 1

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(NotifiedUsed, Data, Val, Mode, State);
		}

	};

	class InputNumShowUnit:public ReadableDisplayUnit
	{
	public:
		int PswMode;					//密码样式
		bool IsFillZero;				//是否使用前导0
		AccuracyParam Accuracy;			//小数及整数位

		bool IsWarnUpperUsed;			//是否使用报警上限
		DataVarOrManual WarnUpper;		//报警上限设置
		bool IsWarnLowerUsed;			//是否使用报警下限
		DataVarOrManual WarnLower;		//报警下限设置
		bool IsWarnStyleUsed;			//是否使用报警样式
		WarnStyle WarnStyleUpper;		//报警上限样式
		WarnStyle WarnStyleLower;		//报警下限样式

		bool IsConvertUsed;				//是否使用比例转换
		DataVarOrManual NovUpperSrc;	//比例转换数据源上限
		DataVarOrManual NovLowerSrc;	//比例转换数据源下限
		DataVarOrManual NovUpperRes;	//比例转换转换值上限
		DataVarOrManual NovLowerRes;	//比例转换转换值下限
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(PswMode, IsFillZero, Accuracy, IsWarnUpperUsed, WarnUpper,
				IsWarnLowerUsed, WarnLower, IsWarnStyleUsed, WarnStyleUpper, WarnStyleLower,
				IsConvertUsed, NovUpperSrc, NovLowerSrc, NovUpperRes, NovLowerRes);
		}
	};
}