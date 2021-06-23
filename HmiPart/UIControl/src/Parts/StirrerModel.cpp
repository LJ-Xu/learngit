/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StirrerModel.cpp
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : 搅拌器控件参数解析类
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "StirrerModel.h"

namespace UI
{
	StirrerModel::StirrerModel() {

	}

	StirrerModel::~StirrerModel() {

	}

	// 获取控件基础属性
	void StirrerModel::GetRect(Rectangle & rect) {
		rect.X = StirrerUnit.X + StirrerUnit.OffX;
		rect.Y = StirrerUnit.Y + StirrerUnit.OffY;
		rect.W = StirrerUnit.Width;
		rect.H = StirrerUnit.Height;
	}

	// 初始化测试数据
	void StirrerModel::InitTestData() {	
		// 初始化原始属性
		StirrerUnit.CtrlName = "StirrerControl";
		StirrerUnit.Height = 90;
		StirrerUnit.Width = 180;
		StirrerUnit.X = 100;
		StirrerUnit.Y = 100;
		// 设置逻辑
		StirrerUnit.Logic = 0;
		// 设置方向
		StirrerUnit.Dir = 0;
		// 设置速度
		StirrerUnit.Speed = 5;
	}

	// 获取控件配置
	void * StirrerModel::GetTestConfig() {
		return &StirrerUnit;
	}

	// 初始化控件数据
	void StirrerModel::InitData(string jsonStr) {
		Document json;
		// 解析Json字符串
		json.Parse(jsonStr.c_str());
		// 获取解析结果
		if (json.HasParseError()) {
			cout << "Parse Error : ("
				<< json.GetParseError() << " : "
				<< json.GetErrorOffset() << ")" << endl;
		} else {
			StirrerUnit.InitBaseData(json);
			// 读取地址
			if (json.HasMember("ReadVar") && !json["ReadVar"].IsNull()) {
				if (json["ReadVar"].HasMember("VarID") &&
					!json["ReadVar"]["VarID"].IsNull() &&
					!json["ReadVar"]["VarID"]["IsNull"].GetBool()) {
					StirrerUnit.ReadVar.Parse(json["ReadVar"]["VarID"]);
				}
			}
			// 搅拌器逻辑
			if (json.HasMember("Logic")) {
				StirrerUnit.Logic = json["Logic"].GetInt();
			}
			// 流动方向
			if (json.HasMember("Dir")) {
				StirrerUnit.Dir = json["Dir"].GetInt();
				if (StirrerUnit.Logic == 1)
					StirrerUnit.Dir ^= 1;
			}
			// 流动速度
			if (json.HasMember("SpDataVarId") && !json["SpDataVarId"].IsNull()) {
				if (json["SpDataVarId"].HasMember("VarID") &&
					!json["SpDataVarId"]["VarID"].IsNull() &&
					!json["SpDataVarId"]["VarID"]["IsNull"].GetBool()) {
					StirrerUnit.SpVar.Parse(json["SpDataVarId"]["VarID"]);
				} else if (json.HasMember("Speed")) {
					StirrerUnit.Speed = json["Speed"].GetInt();
				}
			}
			// 背景图片
			if (json.HasMember("Key") && !json["Key"].IsNull()) {
				// 清空资源图片列表
				vector<Project::ImageResId>().swap(StirrerUnit.Keys);
				unique_ptr<Project::ImageResId> pResId;
				size_t size = json["Key"].Size();
				for (size_t i = 0; i < size; ++i) {
					pResId.reset(new Project::ImageResId());
					if (size == 1) {
						pResId->Parse(json["Key"]);
					} else {
						pResId->Parse(json["Key"][i]);
					}
					StirrerUnit.Keys.push_back(*pResId);
				}
				pResId.reset();
			}
		}
	}

	// 初始化控件数据
	void StirrerModel::InitData(vector<char> data) {
		return BaseModel::InitData<decltype(StirrerUnit)>(data, StirrerUnit);
	}

	// 获取控件数据
	unique_ptr<vector<char>> StirrerModel::GetData() {
		return BaseModel::GetData<decltype(StirrerUnit)>(StirrerUnit);
	}
}
