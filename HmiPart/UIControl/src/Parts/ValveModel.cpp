/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ValveModel.cpp
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : ���ſؼ�����������
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

	// ��ȡ�ؼ���������
	void ValveModel::GetRect(Rectangle & rect) {
		rect.X = ValveUnit.X + ValveUnit.OffX;
		rect.Y = ValveUnit.Y + ValveUnit.OffY;
		rect.W = ValveUnit.Width;
		rect.H = ValveUnit.Height;
	}

	// ��ʼ����������
	void ValveModel::InitTestData() {
		// ��ʼ��ԭʼ����
		ValveUnit.CtrlName = "ValveControl";
		ValveUnit.Height = 110;
		ValveUnit.Width = 100;
		ValveUnit.X = 100;
		ValveUnit.Y = 100;
		// ���ö���
		ValveUnit.Act = Project::BtnAction(0);
		// �����߼�
		ValveUnit.Logic = 0;
		// ���÷���
		ValveUnit.Dir = 0;
		// �����ٶ�
		ValveUnit.Speed = 5;
	}

	// ��ȡ�ؼ�����
	void * ValveModel::GetTestConfig() {
		return &ValveUnit;
	}

	// ��ʼ���ؼ�����
	void ValveModel::InitData(string jstr) {
		Document json;
		// ����Json�ַ���
		json.Parse(jstr.c_str());
		// ��ȡ�������
		if (json.HasParseError()) {
			cout << "Parse Error : ("
				<< json.GetParseError() << " : "
				<< json.GetErrorOffset() << ")" << endl;
		} else {
			// ��ȡ��ַ
			if (json.HasMember("ReadVar") && !json["ReadVar"].IsNull()) {
				if (json["ReadVar"].HasMember("VarID") && 
					!json["ReadVar"]["VarID"].IsNull() && 
					!json["ReadVar"]["VarID"]["IsNull"].GetBool()) {
					ValveUnit.ReadVar.Parse(json["ReadVar"]["VarID"]);
				}
			}
			// д���ַ
			if (json.HasMember("WriteVar") && !json["WriteVar"].IsNull()) {
				if (json["WriteVar"].HasMember("VarID") && 
					!json["WriteVar"]["VarID"].IsNull() && 
					!json["WriteVar"]["VarID"]["IsNull"].GetBool()) {
					ValveUnit.WriteVar.Parse(json["WriteVar"]["VarID"]);
				}
			}
			//��λ����û��IsRWDiff������ʱ��������߼�
#if TEMPRWDIF
			//if (ValveUnit.WriteVar.IsNegative())
			//	ValveUnit.WriteVar.Vid = ValveUnit.ReadVar.Vid;
#else

			if (json.HasMember("IsRWDiff"))
				ValveUnit.IsRWDiff = json["IsRWDiff"].GetBool();
#endif
			// ���Ŷ���
			if (json.HasMember("Act")) {
				ValveUnit.Act = Project::BtnAction(json["Act"].GetInt());
			}
			// �����߼�
			if (json.HasMember("Logic")) {
				ValveUnit.Logic = json["Logic"].GetInt();
			}
			// ��������
			if (json.HasMember("Dir")) {
				ValveUnit.Dir = json["Dir"].GetInt();
			}
			// �����ٶ�
			if (json.HasMember("SpDataVarId") && !json["SpDataVarId"].IsNull()) {
				if (json["SpDataVarId"].HasMember("VarID") && 
					!json["SpDataVarId"]["VarID"].IsNull() && 
					!json["SpDataVarId"]["VarID"]["IsNull"].GetBool()) {
					// ʹ�üĴ�����ȡ����
					ValveUnit.SpVar.Parse(json["SpDataVarId"]["VarID"]);
				} else if (json.HasMember("Speed")) {
					// ��ȡ�����ٶ�
					ValveUnit.Speed = json["Speed"].GetInt();
				}
			}
			// ����ͼƬ
			if (json.HasMember("Key") && !json["Key"].IsNull()) {
				// �����ԴͼƬ�б�
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
				// �ͷ���Դ
				pResId.reset();
			}
			// ��ȫ����
			if (json.HasMember("Perm") && !json["Perm"].IsNull()) {
				ValveUnit.Perm.Parse(json["Perm"]);
			}
			// �ؼ�ʹ��
			if (json.HasMember("IsEnable")) {
				ValveUnit.IsEnable = json["IsEnable"].GetBool();
			}
			// �ؼ�����
			if (json.HasMember("CtrlName") && !json["CtrlName"].IsNull()) {
				ValveUnit.CtrlName = json["CtrlName"].GetString();
			}
			// �ؼ���������
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

	// ��ʼ���ؼ�����
	void ValveModel::InitData(vector<char> data) {
		return BaseModel::InitData<decltype(ValveUnit)>(data, ValveUnit);
	}

	// ��ȡ�ؼ�����
	unique_ptr<vector<char>> ValveModel::GetData() {
		return BaseModel::GetData<decltype(ValveUnit)>(ValveUnit);
	}
}