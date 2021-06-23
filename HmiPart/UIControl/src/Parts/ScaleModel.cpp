#include "stdafx.h"
#include "ScaleModel.h"
#include <FL/Enumerations.H>
#include "JsonComm.h"

using namespace rapidjson;

namespace UI
{
	void ScaleModel::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
			return;
		}
		ScaleConfig.InitBaseData(json);

		if (json.HasMember("ScaleType"))
		{
			ScaleConfig.ScaleTp = (Project::ScaleConfig::ScaleType)json["ScaleType"].GetInt();
			if (ScaleConfig.ScaleTp == Project::ScaleConfig::ScaleType::UDCir)
			{
				if (json.HasMember("CircularStartAngle"))
					ScaleConfig.CircularStartAngle = json["CircularStartAngle"].GetInt(); 
				if (json.HasMember("CircularEndAngle"))
					ScaleConfig.CircularEndAngle = json["CircularEndAngle"].GetInt();
			}
		}
			
		if (json.HasMember("LineColor"))
			ScaleConfig.LineColor = json["LineColor"].GetInt();
		if (json.HasMember("LineStyle"))
			ScaleConfig.LineStyle = json["LineStyle"].GetInt();
		if (json.HasMember("LineWidth"))
			ScaleConfig.LineWidth = json["LineWidth"].GetInt();
		if (json.HasMember("MajorScaleCutCount"))
			ScaleConfig.MainGraduateCount = json["MajorScaleCutCount"].GetInt();
		if (json.HasMember("MajorScaleLineLength"))
			ScaleConfig.MajorScaleLineLength = json["MajorScaleLineLength"].GetInt();
		if (json.HasMember("UseSecondaryScale"))
			ScaleConfig.UseSecondaryScale = json["UseSecondaryScale"].GetBool();
		if (json.HasMember("SecondaryScaleCutCount"))
			ScaleConfig.SecondaryScaleCutCount = json["SecondaryScaleCutCount"].GetInt();
		if (json.HasMember("SecondaryScaleLength"))
			ScaleConfig.SecondaryScaleLength = json["SecondaryScaleLength"].GetInt();
		if (json.HasMember("IsDisplayMark"))
			ScaleConfig.IsDisplayMark = json["IsDisplayMark"].GetBool();
		if (json.HasMember("MarkIntegerNumber"))
			ScaleConfig.MarkIntegerNumber = json["MarkIntegerNumber"].GetInt();
		if (json.HasMember("MarkDecimalNumber"))
			ScaleConfig.MarkDecimalNumber = json["MarkDecimalNumber"].GetInt();
		//字体
		if (json.HasMember("MarkFontSytle") && json["MarkFontSytle"].IsObject())
			ScaleConfig.MarkFontSytle.Parse(json["MarkFontSytle"]);
		if (json.HasMember("MarkMaxValue"))
			ScaleConfig.MarkMaxValue = json["MarkMaxValue"].GetInt();
		if (json.HasMember("IsMarkMaxValueUseRegister"))
			ScaleConfig.IsMarkMaxValueUseRegister = json["IsMarkMaxValueUseRegister"].GetBool();
		if (json.HasMember("MarkMaxValueVarIdRef") && json["MarkMaxValueVarIdRef"].IsObject())
			ScaleConfig.MarkMaxValueVarIdRef.Parse(json["MarkMaxValueVarIdRef"]);
		if (json.HasMember("MarkMinValue"))
			ScaleConfig.MarkMinValue = json["MarkMinValue"].GetInt();
		if (json.HasMember("IsMarkMinValueUseRegister"))
			ScaleConfig.IsMarkMinValueUseRegister = json["IsMarkMinValueUseRegister"].GetBool();
		if (json.HasMember("MarkMinValueVarIdRef") && json["MarkMinValueVarIdRef"].IsObject())
			ScaleConfig.MarkMinValueVarIdRef.Parse(json["MarkMinValueVarIdRef"]);
		if (json.HasMember("IsReserveMark"))
			ScaleConfig.IsReserveMark = json["IsReserveMark"].GetBool();
		if (json.HasMember("MarkPlace"))
			ScaleConfig.MarkPlace = json["MarkPlace"].GetInt();
		if (json.HasMember("IsDisplayAxis"))
			ScaleConfig.IsDisplayAxis = json["IsDisplayAxis"].GetBool();
	}

	void ScaleModel::InitTestData()
	{
		ScaleConfig.X = 50;
		ScaleConfig.Y = 50;
		ScaleConfig.Width = 100;
		ScaleConfig.Height = 100;
		ScaleConfig.IsEnable = false;
		ScaleConfig.CtrlName = "SCALE0";

		ScaleConfig.ScaleTp = ScaleConfig.Horizontal;
		ScaleConfig.LineColor = 0;
		ScaleConfig.LineStyle = 0;
		ScaleConfig.LineWidth = 1;
		ScaleConfig.MainGraduateCount = 5;
		ScaleConfig.MajorScaleLineLength = 10;
		ScaleConfig.UseSecondaryScale = true;
		ScaleConfig.SecondaryScaleCutCount = 3;
		ScaleConfig.SecondaryScaleLength = 8;
		ScaleConfig.IsDisplayMark = true;
		ScaleConfig.MarkIntegerNumber = 3;
		ScaleConfig.MarkDecimalNumber = 2;
		ScaleConfig.MarkFontSytle.Colors = 0;
		ScaleConfig.MarkFontSytle.Font.Name = "Arial";
		ScaleConfig.MarkFontSytle.Font.Size = 12.0;
		ScaleConfig.MarkFontSytle.Alpha = 255;
		ScaleConfig.MarkFontSytle.Align = 0;
		ScaleConfig.MarkMaxValue = 100;
		ScaleConfig.IsMarkMaxValueUseRegister = false;
		ScaleConfig.MarkMinValue = -100;
		ScaleConfig.IsMarkMinValueUseRegister = false;
		ScaleConfig.IsReserveMark = false;
		ScaleConfig.MarkPlace = 0;
	}
}
