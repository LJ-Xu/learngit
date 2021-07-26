/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GFuncModel.cpp
 * Author   : TangYao
 * Date     : 2021/02/07
 * Descript : ���������ؼ�����
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

	// ��ȡ�ؼ���������
	void GFuncModel::GetRect(Rectangle & rect) {
		rect.X = GFuncUnit.X + GFuncUnit.OffX;
		rect.Y = GFuncUnit.Y + GFuncUnit.OffY;
		rect.W = GFuncUnit.Width;
		rect.H = GFuncUnit.Height;
	}

	// ��ʼ����������
	void GFuncModel::InitTestData() {
		// ��ʼ��ԭʼ����
		GFuncUnit.CtrlName = "ValveControl";
		GFuncUnit.Height = 200;
		GFuncUnit.Width = 200;
		GFuncUnit.X = 100;
		GFuncUnit.Y = 100;
	}

	// ��ȡ�ؼ�����
	void * GFuncModel::GetTestConfig() {
		return &GFuncUnit;
	}

	// ��ʼ���ؼ�����
	void GFuncModel::InitData(string jstr) {
		Document json;
		// ����Json�ַ���
		json.Parse(jstr.c_str());
		// ��ȡ�������
		if (json.HasParseError()) {
			cout << "Parse Error : ("
				<< json.GetParseError() << " : "
				<< json.GetErrorOffset() << ")" << endl;
		}
		else {

			// ��ȫ����
			if (json.HasMember("Perm") && !json["Perm"].IsNull()) {
				GFuncUnit.Perm.Parse(json["Perm"]);
			}
			// �ؼ�ʹ��
			if (json.HasMember("IsEnable")) {
				GFuncUnit.IsEnable = json["IsEnable"].GetBool();
			}
			// �ؼ�����
			if (json.HasMember("CtrlName") && !json["CtrlName"].IsNull()) {
				GFuncUnit.CtrlName = json["CtrlName"].GetString();
			}
			// �ؼ���������
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

	// ��ʼ���ؼ�����
	void GFuncModel::InitData(vector<char> data) {
		return BaseModel::InitData<decltype(GFuncUnit)>(data, GFuncUnit);
	}

	// ��ȡ�ؼ�����
	unique_ptr<vector<char>> GFuncModel::GetData() {
		return BaseModel::GetData<decltype(GFuncUnit)>(GFuncUnit);
	}
}