#include "stdafx.h"
#include "BtnFunctionModel.h"
#include <FL/Fl.H>
#include "JsonComm.h"
using namespace rapidjson;
namespace UI
{
	void BtnFunctionModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/KeyBtn.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		//if (json.HasParseError())	//判断解析是否正确
		//{
		//	printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		//}
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else
		{
			LOG_INFO("BtnFunctionModel Parse Json\n");
			FuncBtnConfig.InitBaseData(json);

			if (json.HasMember("Txt") && json["Txt"].IsObject())
				FuncBtnConfig.Txt.Parse(json["Txt"]);
			if (json.HasMember("Csses") && json["Csses"].IsArray())
				Project::StringStyle::Parse(FuncBtnConfig.StrStyles, json["Csses"]);
			if (json.HasMember("Keys") && json["Keys"].IsArray())
				Project::ImageResId::Parse(FuncBtnConfig.PicKey, json["Keys"]);

			if (json.HasMember("PressStates") && json["PressStates"].IsArray())
				Project::BtnFunctionRes::Parse(FuncBtnConfig.Press, json["PressStates"]);
			if (json.HasMember("ReleaseStates") && json["ReleaseStates"].IsArray())
				Project::BtnFunctionRes::Parse(FuncBtnConfig.Release, json["ReleaseStates"]);
		}
	}
}