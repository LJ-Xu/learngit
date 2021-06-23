#include "stdafx.h"
#include "RegionFunctionModel.h"
#include <FL/Fl.H>
#include "JsonComm.h"
using namespace rapidjson;
namespace UI
{
	void RegionFunctionModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/KeyBtn.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
			return;
		}
		LOG_INFO_("RegionFunctionModel Parse Json\n");
		FuncRegionConfig.InitBaseData(json);
		if (json.HasMember("ActionMode"))
			FuncRegionConfig.Mode = (Project::ActionMode)json["ActionMode"].GetInt();
		if (json.HasMember("CoilVarIdRef") && json["CoilVarIdRef"].IsObject())
			FuncRegionConfig.CoilVar.Parse(json["CoilVarIdRef"]);
		if (json.HasMember("TriggerCond"))
			FuncRegionConfig.TriggerMode = json["TriggerCond"].GetInt();
		if (json.HasMember("Timing"))
			FuncRegionConfig.Time = (double)json["Timing"].GetInt64();
		if (json.HasMember("CoilLmtVarIdRef") && json["CoilLmtVarIdRef"].IsObject())
			FuncRegionConfig.TimeLmtVar.Parse(json["CoilLmtVarIdRef"]);
		if (json.HasMember("NumericLogicCondVarIdRef") && json["NumericLogicCondVarIdRef"].IsObject())
			FuncRegionConfig.ValueVarId.Parse(json["NumericLogicCondVarIdRef"]);
		if (json.HasMember("NumericCondition"))
			FuncRegionConfig.NumlogicCond = (Project::Conditions)json["NumericCondition"].GetInt();
		if (json.HasMember("NumericLogicValue"))
			FuncRegionConfig.Value = json["NumericLogicValue"].GetDouble();
		if (json.HasMember("CoilLmtCondition"))
			FuncRegionConfig.StopMode = json["CoilLmtCondition"].GetInt();
		if (json.HasMember("TimeDisplayVarIdRef") && json["TimeDisplayVarIdRef"].IsObject())
			FuncRegionConfig.TimeValueVarId.Parse(json["TimeDisplayVarIdRef"]);
		if (json.HasMember("FuncReses") && json["FuncReses"].IsArray())
			Project::BtnFunctionRes::Parse(FuncRegionConfig.Functions, json["FuncReses"]);
		if (json.HasMember("FirstDownLoadVar") && json["FirstDownLoadVar"].IsObject())
			FuncRegionConfig.FirstDownLoadVar.Parse(json["FirstDownLoadVar"]);
		if (json.HasMember("PowerOnVar") && json["PowerOnVar"].IsObject())
			FuncRegionConfig.PowerOnVar.Parse(json["PowerOnVar"]);
	}
}