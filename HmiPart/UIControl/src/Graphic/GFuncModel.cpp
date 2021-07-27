/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GFuncModel.cpp
 * Author   : TangYao
 * Date     : 2021/02/07
 * Descript : 函数画布控件参数
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "GFuncModel.h"

namespace UI
{
	GFuncModel::GFuncModel() {

	}

	GFuncModel::~GFuncModel() {

	}

	// 获取控件基础属性
	void GFuncModel::GetRect(Rectangle & rect) {
		rect.X = GFuncUnit.X + GFuncUnit.OffX;
		rect.Y = GFuncUnit.Y + GFuncUnit.OffY;
		rect.W = GFuncUnit.Width;
		rect.H = GFuncUnit.Height;
	}

	// 初始化测试数据
	void GFuncModel::InitTestData() {
		// 初始化原始属性
		GFuncUnit.CtrlName = "ValveControl";
		GFuncUnit.Height = 200;
		GFuncUnit.Width = 200;
		GFuncUnit.X = 100;
		GFuncUnit.Y = 100;
	}

	// 获取控件配置
	void * GFuncModel::GetTestConfig() {
		return &GFuncUnit;
	}

	// 初始化控件数据
	void GFuncModel::InitData(string jstr) {
		Document json;
		// 解析Json字符串
		json.Parse(jstr.c_str());
		// 获取解析结果
		if (json.HasParseError()) {
			cout << "Parse Error : ("
				<< json.GetParseError() << " : "
				<< json.GetErrorOffset() << ")" << endl;
		}
		else {

			// 安全属性
			if (json.HasMember("Perm") && !json["Perm"].IsNull()) {
				GFuncUnit.Perm.Parse(json["Perm"]);
			}
			// 控件使能
			if (json.HasMember("IsEnable")) {
				GFuncUnit.IsEnable = json["IsEnable"].GetBool();
			}
			// 控件名称
			if (json.HasMember("CtrlName") && !json["CtrlName"].IsNull()) {
				GFuncUnit.CtrlName = json["CtrlName"].GetString();
			}
			// 控件基础属性
			if (json.HasMember("OffX")) {
				GFuncUnit.InitOffXData(json["OffX"]);
			}
			if (json.HasMember("OffY")) {
				GFuncUnit.InitOffYData(json["OffY"]);
			}
			if (json.HasMember("Width")) {
				GFuncUnit.Width = json["Width"].GetInt();
			}
			if (json.HasMember("Height")) {
				GFuncUnit.Height = json["Height"].GetInt();
			}
			if (json.HasMember("X")) {
				GFuncUnit.X = json["X"].GetInt();
			}
			if (json.HasMember("Y")) {
				GFuncUnit.Y = json["Y"].GetInt();
			}
			if (json.HasMember("MapBgColor"))
			{
				GFuncUnit.BackColor = json["MapBgColor"].GetInt();
			}
		}
	}

	// 初始化控件数据
	void GFuncModel::InitData(vector<char> data) {
		return BaseModel::InitData<decltype(GFuncUnit)>(data, GFuncUnit);
	}

	// 获取控件数据
	unique_ptr<vector<char>> GFuncModel::GetData() {
		return BaseModel::GetData<decltype(GFuncUnit)>(GFuncUnit);
	}
}