#include "stdafx.h"
#include "BarCodeModel.h"
#include <FL/Enumerations.H>
using namespace rapidjson;

namespace UI
{
	void BarCodeModel::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
			return;
		}
		BarcodeConfig.InitBaseData(json);
		if (json.HasMember("Dimension"))
			BarcodeConfig.Dimension = json["Dimension"].GetInt();
		if (json.HasMember("TwoDimensionType"))
			BarcodeConfig.TwoDimensionType = (Project::BarCodeConfig::TwoDimensionCodeType)json["TwoDimensionType"].GetInt();
		if (json.HasMember("OneDimensionType"))
			BarcodeConfig.OneDimensionType = (Project::BarCodeConfig::OneDimensionCodeType)json["OneDimensionType"].GetInt();
		if (json.HasMember("EncodingRule"))
			BarcodeConfig.EncodingRule = json["EncodingRule"].GetInt();
		if (json.HasMember("CorrectErrorLevel"))
			BarcodeConfig.CorrectErrorLevel = json["CorrectErrorLevel"].GetInt();

		if (json.HasMember("UseConstValue"))
			BarcodeConfig.UseConstValue = json["UseConstValue"].GetBool();

		if (json.HasMember("ConstString") && json["ConstString"].IsString())
		{
			BarcodeConfig.ConstString = json["ConstString"].GetString();
		}

		if (json.HasMember("ReadVar") && json["ReadVar"].IsObject())
			BarcodeConfig.ReadVar.Parse(json["ReadVar"]);

	}

	void BarCodeModel::InitTestData()
	{

		BarcodeConfig.X = 50;
		BarcodeConfig.Y = 50;
		BarcodeConfig.Width = 100;
		BarcodeConfig.Height = 100;
		BarcodeConfig.IsEnable = false;
		BarcodeConfig.CtrlName = "BARCODE0";

		BarcodeConfig.Dimension = 1;
		BarcodeConfig.TwoDimensionType = Project::BarCodeConfig::TwoDimensionCodeType::QRCode;
		BarcodeConfig.OneDimensionType = Project::BarCodeConfig::OneDimensionCodeType::EAN13;
		BarcodeConfig.EncodingRule = 0;
		BarcodeConfig.CorrectErrorLevel = 0;
		BarcodeConfig.UseConstValue = true;
		BarcodeConfig.ConstString = "0";


	}
}
