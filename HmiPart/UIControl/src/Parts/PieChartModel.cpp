/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StirrerModel.cpp
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : ��ͼ�ؼ�����������
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "PieChartModel.h"
#include "Color.h"

namespace UI
{
	PieChartModel::PieChartModel() {

	}

	PieChartModel::~PieChartModel() {

	}

	// ��ȡ�ؼ�λ������
	void PieChartModel::GetRect(Rectangle & rect) {
		rect.X = PieChartUnit.X + PieChartUnit.OffX;
		rect.Y = PieChartUnit.Y + PieChartUnit.OffY;
		rect.W = PieChartUnit.Width;
		rect.H = PieChartUnit.Height;
	}

	// ��ʼ����������
	void PieChartModel::InitTestData() {
		// ��ʼ��ԭʼ����
		PieChartUnit.CtrlName = "PieChartControl";
		PieChartUnit.Height = 100;
		PieChartUnit.Width = 100;
		PieChartUnit.X = 100;
		PieChartUnit.Y = 100;
	}

	// ��ȡ�ؼ�����
	void * PieChartModel::GetTestConfig() {
		return &PieChartUnit;
	}

	// ��ʼ���ؼ�����
	void PieChartModel::InitData(string jsonStr) {
		Document json;
		// ����json�ַ���
		json.Parse(jsonStr.c_str());
		// ��ȡ�������
		if (json.HasParseError()) {
			cout << "Parse Error : ("
				<< json.GetParseError() << " : "
				<< json.GetErrorOffset() << ")" << endl;
		} else {
			PieChartUnit.InitBaseData(json);
			// ��ȡ��ַ
			if (json.HasMember("ReadVar") && json["ReadVar"].IsArray()) {

				for (int i = 0; i < json["ReadVar"].Size(); ++i)
				{
					Project::DataVarId vid;
					vid.Parse(json["ReadVar"][i]);
					PieChartUnit.ReadVars.push_back(vid);
				}
				/*if (json["ReadVar"].HasMember("VarID") &&
					!json["ReadVar"]["VarID"].IsNull() &&
					!json["ReadVar"]["VarID"]["IsNull"].GetBool()) {
					PieChartUnit.ReadVar.Parse(json["ReadVar"]["VarID"]);
				}*/
			}
			// ��ͼͨ����
			if (json.HasMember("Num")) {
				PieChartUnit.ChannelNum = json["Num"].GetInt();
			}
			// ��ͼ����
			if (json.HasMember("Dir")) {
				PieChartUnit.Dir = json["Dir"].GetInt();
			}
			// ��ʼ�Ƕ�
			if (json.HasMember("StartAngle")) {
				PieChartUnit.StartAngle = json["StartAngle"].GetInt();
			}
			// ��ֹ�Ƕ�
			if (json.HasMember("EndAngle")) {
				PieChartUnit.EndAngle = json["EndAngle"].GetInt();
			}
			// ͸����
			if (json.HasMember("Alpha")) {
				PieChartUnit.Alpha = json["Alpha"].GetInt();
			}
			// Բ���ڲ���ɫ
			if (json.HasMember("Rad")) {
				PieChartUnit.CenterRadiusPecent = (float)json["Rad"].GetInt() / 100.0f;
				PieChartUnit.CenterRadius = (float)PieChartUnit.Width * PieChartUnit.CenterRadiusPecent/2;
			}
			// Բ���ڲ���ɫ
			if (json.HasMember("InnerColor")) {
				PieChartUnit.CenterColor.R = R(json["InnerColor"].GetInt());
				PieChartUnit.CenterColor.G = G(json["InnerColor"].GetInt());
				PieChartUnit.CenterColor.B = B(json["InnerColor"].GetInt());
			}
			// Բ���ڲ��߿�
			if (json.HasMember("CBorderColor")) {
				PieChartUnit.CenterEdgeColor.R = R(json["CBorderColor"].GetInt());
				PieChartUnit.CenterEdgeColor.G = G(json["CBorderColor"].GetInt());
				PieChartUnit.CenterEdgeColor.B = B(json["CBorderColor"].GetInt());
			}
			// ��ͼ�߿���ɫ
			if (json.HasMember("BorderColor")) {
				PieChartUnit.BorderColor.R = R(json["BorderColor"].GetInt());
				PieChartUnit.BorderColor.G = G(json["BorderColor"].GetInt());
				PieChartUnit.BorderColor.B = B(json["BorderColor"].GetInt());
			}
			// ��ͼͨ����ɫ
			if (json.HasMember("Channels") && !json["Channels"].IsNull()) {
				// �����������
				vector<Project::PieChartChannelStyle>().swap(PieChartUnit.Channels);
				PieChartUnit.TotalVal = 0;
				// ��ȡͨ����Ϣ
				for (int i = 0; i < PieChartUnit.ChannelNum; i++) {
					Project::PieChartChannelStyle * style = new Project::PieChartChannelStyle();
					if (json["Channels"][i].HasMember("FontColor") && !json["Channels"][i]["FontColor"].IsNull()) {
						style->FontColor = Project::Color(R(json["Channels"][i]["FontColor"].GetInt()),
														 G(json["Channels"][i]["FontColor"].GetInt()),
														 B(json["Channels"][i]["FontColor"].GetInt()));
					}
					if (json["Channels"][i].HasMember("BackColor") && !json["Channels"][i]["BackColor"].IsNull()) {
						style->BackColor = Project::Color(R(json["Channels"][i]["BackColor"].GetInt()),
														 G(json["Channels"][i]["BackColor"].GetInt()),
														 B(json["Channels"][i]["BackColor"].GetInt()));
					}
					style->Val = static_cast<float>(i + 1);
					PieChartUnit.Channels.push_back(*style);
					PieChartUnit.TotalVal += style->Val;
				}
				float remainPercent = 1.0F;
				for (int i = 0; i < PieChartUnit.ChannelNum; ++i) {
					if (i == PieChartUnit.ChannelNum - 1)
						PieChartUnit.Channels[i].Percent = remainPercent;
					PieChartUnit.Channels[i].Percent = PieChartUnit.Channels[i].Val / PieChartUnit.TotalVal;
					remainPercent -= PieChartUnit.Channels[i].Percent;
				}
			}
			// ������ʾ��ʽ
			if (json.HasMember("DataFormat") && !json["DataFormat"].IsNull()) {
				if (json["DataFormat"].HasMember("Mod")) {
					PieChartUnit.Mode = json["DataFormat"]["Mod"].GetInt();
				}
				if (json["DataFormat"].HasMember("Bct")) {
					PieChartUnit.Bct = json["DataFormat"]["Bct"].GetInt();
				}
				if (json["DataFormat"].HasMember("TextStyle") && !json["DataFormat"]["TextStyle"].IsNull()) {
					PieChartUnit.TextStyle.Parse(json["DataFormat"]["TextStyle"]);
				}
			}
		}
	}

	// ��ʼ���ؼ�����
	void PieChartModel::InitData(vector<char> data) {
		return BaseModel::InitData<decltype(PieChartUnit)>(data, PieChartUnit);
	}

	// ��ȡ�ؼ�����
	unique_ptr<vector<char>> PieChartModel::GetData() {
		return BaseModel::GetData<decltype(PieChartUnit)>(PieChartUnit);
	}
}
