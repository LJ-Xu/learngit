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
		DataVarId AlarmNoticeVarId;//报警录入监控地址,固定数值
		//显示组别范围
		int GroupScopeFrom;
		int GroupScopeTo;
		//使用多语言
		bool IsUseMultiLanguage;
		std::vector<AlarmBarMultiLan> MultiLanConfig;
		int DateFormat;//日期格式
		int TimeFormat;//时间格式
		StringStyle AlarmBarFont;// 字体样式
		bool IsBorder;//选择外框设置
		int BorderColor;//外框颜色
		bool IsFill;//是否填充
		int FillColor;//填充颜色
		float Transparency;//透明度
		int MoveSpeed;//流动速度

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

		StringStyle Res;//文字标签
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