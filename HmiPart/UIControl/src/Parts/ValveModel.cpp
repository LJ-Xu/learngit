/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ValveModel.cpp
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : 阀门控件参数解析类
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "stdafx.h"
#include "ValveModel.h"

namespace UI 
{
	ValveModel::ValveModel() {

	}

	ValveModel::~ValveModel() {

	}

	// 获取控件基础属性
	void ValveModel::GetRect(Rectangle & rect) {
		rect.X = ValveUnit.X + ValveUnit.OffX;
		rect.Y = ValveUnit.Y + ValveUnit.OffY;
		rect.W = ValveUnit.Width;
		rect.H = ValveUnit.Height;
	}

	// 初始化测试数据
	void ValveModel::InitTestData() {
		// 初始化原始属性
		ValveUnit.CtrlName = "ValveControl";
		ValveUnit.Height = 110;
		ValveUnit.Width = 100;
		ValveUnit.X = 100;
		ValveUnit.Y = 100;
		// 设置动作
		ValveUnit.Act = Project::BtnAction(0);
		// 设置逻辑
		ValveUnit.Logic = 0;
		// 设置方向
		ValveUnit.Dir = 0;
		// 设置速度
		ValveUnit.Speed = 5;
	}

	// 获取控件配置
	void * ValveModel::GetTestConfig() {
		return &ValveUnit;
	}

	// 初始化控件数据
	void ValveModel::InitData(string jstr) {
		Document json;
		// 解析Json字符串
		json.Parse(jstr.c_str());
		// 获取解析结果
		if (json.HasParseError()) {
			cout << "Parse Error : ("
				<< json.GetParseError() << " : "
				<< json.GetErrorOffset() << ")" << endl;
		} else {
			// 读取地址
			if (json.HasMember("ReadVar") && !json["ReadVar"].IsNull()) {
				if (json["ReadVar"].HasMember("VarID") && 
					!json["ReadVar"]["VarID"].IsNull() && 
					!json["ReadVar"]["VarID"]["IsNull"].GetBool()) {
					ValveUnit.ReadVar.Parse(json["ReadVar"]["VarID"]);
				}
			}
			// 写入地址
			if (json.HasMember("WriteVar") && !json["WriteVar"].IsNull()) {
				if (json["WriteVar"].HasMember("VarID") && 
					!json["WriteVar"]["VarID"].IsNull() && 
					!json["WriteVar"]["VarID"]["IsNull"].GetBool()) {
					ValveUnit.WriteVar.Parse(json["WriteVar"]["VarID"]);
				}
			}
			//上位机还没把IsRWDiff传过来时先用这个逻辑
#if TEMPRWDIF
			//if (ValveUnit.WriteVar.IsNegative())
			//	ValveUnit.WriteVar.Vid = ValveUnit.ReadVar.Vid;
#else

			if (json.HasMember("IsRWDiff"))
				ValveUnit.IsRWDiff = json["IsRWDiff"].GetBool();
#endif
			// 阀门动作
			if (json.HasMember("Act")) {
				ValveUnit.Act = Project::BtnAction(json["Act"].GetInt());
			}
			// 阀门逻辑
			if (json.HasMember("Logic")) {
				ValveUnit.Logic = json["Logic"].GetInt();
			}
			// 流动方向
			if (json.HasMember("Dir")) {
				ValveUnit.Dir = json["Dir"].GetInt();
			}
			// 流动速度
			if (json.HasMember("SpDataVarId") && !json["SpDataVarId"].IsNull()) {
				if (json["SpDataVarId"].HasMember("VarID") && 
					!json["SpDataVarId"]["VarID"].IsNull() && 
					!json["SpDataVarId"]["VarID"]["IsNull"].GetBool()) {
					// 使用寄存器获取流速
					ValveUnit.SpVar.Parse(json["SpDataVarId"]["VarID"]);
				} else if (json.HasMember("Speed")) {
					// 获取流动速度
					ValveUnit.Speed = json["Speed"].GetInt();
				}
			}
			// 背景图片
			if (json.HasMember("Key") && !json["Key"].IsNull()) {
				// 清空资源图片列表
				vector<Project::ImageResId>().swap(ValveUnit.PicKey);
				unique_ptr<Project::ImageResId> pResId;
				size_t size = json["Key"].Size();
				for (size_t i = 0; i < size; ++i) {
					pResId.reset(new Project::ImageResId());
					if (size == 1) {
						pResId->Parse(json["Key"]);
					} else {
						pResId->Parse(json["Key"][i]);
					}	
					ValveUnit.PicKey.push_back(*pResId);
				}
				// 释放资源
				pResId.reset();
			}
			// 安全属性
			if (json.HasMember("Perm") && !json["Perm"].IsNull()) {
				ValveUnit.Perm.Parse(json["Perm"]);
			}
			// 控件使能
			if (json.HasMember("IsEnable")) {
				ValveUnit.IsEnable = json["IsEnable"].GetBool();
			}
			// 控件名称
			if (json.HasMember("CtrlName") && !json["CtrlName"].IsNull()) {
				ValveUnit.CtrlName = json["CtrlName"].GetString();
			}
			// 控件基础属性
			if (json.HasMember("OffX")) {
				ValveUnit.InitOffXData(json["OffX"]);
			}
			if (json.HasMember("OffY")) {
				ValveUnit.InitOffYData(json["OffY"]);
			}
			if (json.HasMember("Width")) {
				ValveUnit.Width = json["Width"].GetInt();
			}
			if (json.HasMember("Height")) {
				ValveUnit.Height = json["Height"].GetInt();
			}
			if (json.HasMember("X")) {
				ValveUnit.X = json["X"].GetInt();
			}
			if (json.HasMember("Y")) {
				ValveUnit.Y = json["Y"].GetInt();
			}

			if (json.HasMember("InnerRectX")) {
				ValveUnit.InnerRectX = json["InnerRectX"].GetInt();
			}
			if (json.HasMember("InnerRectY")) {
				ValveUnit.InnerRectY = json["InnerRectY"].GetInt();
			}
			if (json.HasMember("InnerRectW")) {
				ValveUnit.InnerRectW = json["InnerRectW"].GetInt();
			}
			if (json.HasMember("InnerRectH")) {
				ValveUnit.InnerRectH = json["InnerRectH"].GetInt();
			}
		}
	}

	// 初始化控件数据
	void ValveModel::InitData(vector<char> data) {
		return BaseModel::InitData<decltype(ValveUnit)>(data, ValveUnit);
	}

	// 获取控件数据
	unique_ptr<vector<char>> ValveModel::GetData() {
		return BaseModel::GetData<decltype(ValveUnit)>(ValveUnit);
	}
}