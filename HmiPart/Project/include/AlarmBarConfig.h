#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
#include "StringStyle.h"

namespace Project
{
	class AlarmBarMultiLan;
	class AlarmBarConfig :public MoveableDisplayUnit
	{
	public:
		DataVarId AlarmNoticeVarId;//����¼���ص�ַ,�̶���ֵ
		//��ʾ���Χ
		int GroupScopeFrom;
		int GroupScopeTo;
		//ʹ�ö�����
		bool IsUseMultiLanguage;
		std::vector<AlarmBarMultiLan> MultiLanConfig;
		int DateFormat;//���ڸ�ʽ
		int TimeFormat;//ʱ���ʽ
		StringStyle AlarmBarFont;// ������ʽ
		bool IsBorder;//ѡ���������
		int BorderColor;//�����ɫ
		bool IsFill;//�Ƿ����
		int FillColor;//�����ɫ
		float Transparency;//͸����
		int MoveSpeed;//�����ٶ�

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(X, Y, VOffX, VOffY, Perm, Width, Height, CtrlName, AlarmNoticeVarId, GroupScopeFrom, GroupScopeTo, IsUseMultiLanguage, MultiLanConfig, DateFormat, TimeFormat, AlarmBarFont, IsBorder, BorderColor, IsFill, FillColor, Transparency, MoveSpeed);
		}
	};
	class AlarmBarMultiLan
	{
	public:
		bool  IsSelect;
		std::string Project;
		std::string Describe;
		std::string Setting;

		StringStyle Res;//���ֱ�ǩ
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(IsSelect, Project, Describe, Setting, Res);
		}

		void Parse(rapidjson::Value& jsonObj)
		{
			if (jsonObj.IsObject())
			{
				if (jsonObj.HasMember("IsSelect"))
				{
					this->IsSelect = jsonObj["IsSelect"].GetBool();
				}
				if (jsonObj.HasMember("Project")&& jsonObj["Project"].IsString())
				{
					this->Project = jsonObj["Project"].GetString();
				}
				if (jsonObj.HasMember("Describe") && jsonObj["Describe"].IsString())
				{
					this->Describe = jsonObj["Describe"].GetString();
				}
				if (jsonObj.HasMember("Setting") && jsonObj["Setting"].IsString())
				{
					this->Setting = jsonObj["Setting"].GetString();
				}
				if (jsonObj.HasMember("Res")&&jsonObj["Res"].IsObject())
				{
					this->Res.Parse(jsonObj["Res"]);
				}
			}
		}
	};
}