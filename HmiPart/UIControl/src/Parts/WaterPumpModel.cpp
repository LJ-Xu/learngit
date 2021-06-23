/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StirrerModel.cpp
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : 水泵控件参数解析类
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "WaterPumpModel.h"

namespace UI
{
	WaterPumpModel::WaterPumpModel() {

	}

	WaterPumpModel::~WaterPumpModel() {

	}

	// 获取控件基础属性
	void WaterPumpModel::GetRect(Rectangle& rect) {
		rect.X = WaterPumpUnit.X + WaterPumpUnit.OffX;
		rect.Y = WaterPumpUnit.Y + WaterPumpUnit.OffY;
		rect.W = WaterPumpUnit.Width;
		rect.H = WaterPumpUnit.Height;
	}

	// 初始化测试数据
	void WaterPumpModel::InitTestData() {
		// 初始化原始属性
		WaterPumpUnit.CtrlName = "WaterPumpControl";
		WaterPumpUnit.Height = 150;
		WaterPumpUnit.Width = 90;
		WaterPumpUnit.X = 100;
		WaterPumpUnit.Y = 100;
		// 设置逻辑
		WaterPumpUnit.Logic = 0;
		// 设置方向
		WaterPumpUnit.Dir = 0;
		// 设置速度
		WaterPumpUnit.Speed = 5;
	}

	// 获取控件配置
	void * WaterPumpModel::GetTestConfig() {
		return &WaterPumpUnit;
	}

	// 初始化控件数据
	void WaterPumpModel::InitData(string jsonStr) {
		Document json;
		// 解析Json字符串
		json.Parse(jsonStr.c_str());
		// 获取解析结果
		if (json.HasParseError()) {
			cout << "Parse Error : ("
				<< json.GetParseError() << " : "
				<< json.GetErrorOffset() << ")" << endl;
		} else {
			WaterPumpUnit.InitBaseData(json);
			// 读取地址
			if (json.HasMember("ReadVar") && !json["ReadVar"].IsNull()) {
				if (json["ReadVar"].HasMember("VarID") &&
					!json["ReadVar"]["VarID"].IsNull() &&
					!json["ReadVar"]["VarID"]["IsNull"].GetBool()) {
					WaterPumpUnit.ReadVar.Parse(json["ReadVar"]["VarID"]);
				}
			}
			// 搅拌器逻辑
			if (json.HasMember("Logic")) {
				WaterPumpUnit.Logic = json["Logic"].GetInt();
			}
			// 流动方向
			if (json.HasMember("Dir")) {
				WaterPumpUnit.Dir = json["Dir"].GetInt();
				if (WaterPumpUnit.Logic == 1)
					WaterPumpUnit.Dir ^= 1;
			}
			// 流动速度
			if (json.HasMember("SpDataVarId") && !json["SpDataVarId"].IsNull()) {
				if (json["SpDataVarId"].HasMember("VarID") &&
					!json["SpDataVarId"]["VarID"].IsNull() &&
					!json["SpDataVarId"]["VarID"]["IsNull"].GetBool()) {
					WaterPumpUnit.SpVar.Parse(json["SpDataVarId"]["VarID"]);
				} else if (json.HasMember("Speed")) {
					WaterPumpUnit.Speed = json["Speed"].GetInt();
				}
			}
			// 背景图片
			if (json.HasMember("Key") && !json["Key"].IsNull()) {
				// 清空资源图片列表
				vector<Project::ImageResId>().swap(WaterPumpUnit.Keys);
				unique_ptr<Project::ImageResId> pResId;
				size_t size = json["Key"].Size();
				for (size_t i = 0; i < size; ++i) {
					pResId.reset(new Project::ImageResId());
					if (size == 1) {
						pResId->Parse(json["Key"]);
					} else {
						pResId->Parse(json["Key"][i]);
					}
					WaterPumpUnit.Keys.push_back(*pResId);
				}
				pResId.reset();
			}
			if (json.HasMember("InnerRectX")) {
				WaterPumpUnit.InnerRectX = json["InnerRectX"].GetInt();
			}
			if (json.HasMember("InnerRectY")) {
				WaterPumpUnit.InnerRectY = json["InnerRectY"].GetInt();
			}
			if (json.HasMember("InnerRectW")) {
				WaterPumpUnit.InnerRectW = json["InnerRectW"].GetInt();
			}
			if (json.HasMember("InnerRectH")) {
				WaterPumpUnit.InnerRectH = json["InnerRectH"].GetInt();
			}
		}
	}

	// 初始化控件数据
	void WaterPumpModel::InitData(vector<char> data) {
		return BaseModel::InitData<decltype(WaterPumpUnit)>(data, WaterPumpUnit);
	}

	// 获取控件数据
	unique_ptr<vector<char>> WaterPumpModel::GetData() {
		return BaseModel::GetData<decltype(WaterPumpUnit)>(WaterPumpUnit);
	}
}
