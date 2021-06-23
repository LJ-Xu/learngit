/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StirrerModel.cpp
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : ˮ�ÿؼ�����������
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

	// ��ȡ�ؼ���������
	void WaterPumpModel::GetRect(Rectangle& rect) {
		rect.X = WaterPumpUnit.X + WaterPumpUnit.OffX;
		rect.Y = WaterPumpUnit.Y + WaterPumpUnit.OffY;
		rect.W = WaterPumpUnit.Width;
		rect.H = WaterPumpUnit.Height;
	}

	// ��ʼ����������
	void WaterPumpModel::InitTestData() {
		// ��ʼ��ԭʼ����
		WaterPumpUnit.CtrlName = "WaterPumpControl";
		WaterPumpUnit.Height = 150;
		WaterPumpUnit.Width = 90;
		WaterPumpUnit.X = 100;
		WaterPumpUnit.Y = 100;
		// �����߼�
		WaterPumpUnit.Logic = 0;
		// ���÷���
		WaterPumpUnit.Dir = 0;
		// �����ٶ�
		WaterPumpUnit.Speed = 5;
	}

	// ��ȡ�ؼ�����
	void * WaterPumpModel::GetTestConfig() {
		return &WaterPumpUnit;
	}

	// ��ʼ���ؼ�����
	void WaterPumpModel::InitData(string jsonStr) {
		Document json;
		// ����Json�ַ���
		json.Parse(jsonStr.c_str());
		// ��ȡ�������
		if (json.HasParseError()) {
			cout << "Parse Error : ("
				<< json.GetParseError() << " : "
				<< json.GetErrorOffset() << ")" << endl;
		} else {
			WaterPumpUnit.InitBaseData(json);
			// ��ȡ��ַ
			if (json.HasMember("ReadVar") && !json["ReadVar"].IsNull()) {
				if (json["ReadVar"].HasMember("VarID") &&
					!json["ReadVar"]["VarID"].IsNull() &&
					!json["ReadVar"]["VarID"]["IsNull"].GetBool()) {
					WaterPumpUnit.ReadVar.Parse(json["ReadVar"]["VarID"]);
				}
			}
			// �������߼�
			if (json.HasMember("Logic")) {
				WaterPumpUnit.Logic = json["Logic"].GetInt();
			}
			// ��������
			if (json.HasMember("Dir")) {
				WaterPumpUnit.Dir = json["Dir"].GetInt();
				if (WaterPumpUnit.Logic == 1)
					WaterPumpUnit.Dir ^= 1;
			}
			// �����ٶ�
			if (json.HasMember("SpDataVarId") && !json["SpDataVarId"].IsNull()) {
				if (json["SpDataVarId"].HasMember("VarID") &&
					!json["SpDataVarId"]["VarID"].IsNull() &&
					!json["SpDataVarId"]["VarID"]["IsNull"].GetBool()) {
					WaterPumpUnit.SpVar.Parse(json["SpDataVarId"]["VarID"]);
				} else if (json.HasMember("Speed")) {
					WaterPumpUnit.Speed = json["Speed"].GetInt();
				}
			}
			// ����ͼƬ
			if (json.HasMember("Key") && !json["Key"].IsNull()) {
				// �����ԴͼƬ�б�
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

	// ��ʼ���ؼ�����
	void WaterPumpModel::InitData(vector<char> data) {
		return BaseModel::InitData<decltype(WaterPumpUnit)>(data, WaterPumpUnit);
	}

	// ��ȡ�ؼ�����
	unique_ptr<vector<char>> WaterPumpModel::GetData() {
		return BaseModel::GetData<decltype(WaterPumpUnit)>(WaterPumpUnit);
	}
}
