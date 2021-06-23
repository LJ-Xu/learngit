/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StirrerModel.cpp
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : �������ؼ�����������
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

	// ��ȡ�ؼ���������
	void StirrerModel::GetRect(Rectangle & rect) {
		rect.X = StirrerUnit.X + StirrerUnit.OffX;
		rect.Y = StirrerUnit.Y + StirrerUnit.OffY;
		rect.W = StirrerUnit.Width;
		rect.H = StirrerUnit.Height;
	}

	// ��ʼ����������
	void StirrerModel::InitTestData() {	
		// ��ʼ��ԭʼ����
		StirrerUnit.CtrlName = "StirrerControl";
		StirrerUnit.Height = 90;
		StirrerUnit.Width = 180;
		StirrerUnit.X = 100;
		StirrerUnit.Y = 100;
		// �����߼�
		StirrerUnit.Logic = 0;
		// ���÷���
		StirrerUnit.Dir = 0;
		// �����ٶ�
		StirrerUnit.Speed = 5;
	}

	// ��ȡ�ؼ�����
	void * StirrerModel::GetTestConfig() {
		return &StirrerUnit;
	}

	// ��ʼ���ؼ�����
	void StirrerModel::InitData(string jsonStr) {
		Document json;
		// ����Json�ַ���
		json.Parse(jsonStr.c_str());
		// ��ȡ�������
		if (json.HasParseError()) {
			cout << "Parse Error : ("
				<< json.GetParseError() << " : "
				<< json.GetErrorOffset() << ")" << endl;
		} else {
			StirrerUnit.InitBaseData(json);
			// ��ȡ��ַ
			if (json.HasMember("ReadVar") && !json["ReadVar"].IsNull()) {
				if (json["ReadVar"].HasMember("VarID") &&
					!json["ReadVar"]["VarID"].IsNull() &&
					!json["ReadVar"]["VarID"]["IsNull"].GetBool()) {
					StirrerUnit.ReadVar.Parse(json["ReadVar"]["VarID"]);
				}
			}
			// �������߼�
			if (json.HasMember("Logic")) {
				StirrerUnit.Logic = json["Logic"].GetInt();
			}
			// ��������
			if (json.HasMember("Dir")) {
				StirrerUnit.Dir = json["Dir"].GetInt();
				if (StirrerUnit.Logic == 1)
					StirrerUnit.Dir ^= 1;
			}
			// �����ٶ�
			if (json.HasMember("SpDataVarId") && !json["SpDataVarId"].IsNull()) {
				if (json["SpDataVarId"].HasMember("VarID") &&
					!json["SpDataVarId"]["VarID"].IsNull() &&
					!json["SpDataVarId"]["VarID"]["IsNull"].GetBool()) {
					StirrerUnit.SpVar.Parse(json["SpDataVarId"]["VarID"]);
				} else if (json.HasMember("Speed")) {
					StirrerUnit.Speed = json["Speed"].GetInt();
				}
			}
			// ����ͼƬ
			if (json.HasMember("Key") && !json["Key"].IsNull()) {
				// �����ԴͼƬ�б�
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

	// ��ʼ���ؼ�����
	void StirrerModel::InitData(vector<char> data) {
		return BaseModel::InitData<decltype(StirrerUnit)>(data, StirrerUnit);
	}

	// ��ȡ�ؼ�����
	unique_ptr<vector<char>> StirrerModel::GetData() {
		return BaseModel::GetData<decltype(StirrerUnit)>(StirrerUnit);
	}
}
