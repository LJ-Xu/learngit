#include "stdafx.h"
#include "AlarmWinModel.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
using namespace rapidjson;
using namespace std;
namespace UI
{
	void AlarmWinModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/AlarmDisplay.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else {
			if (json.HasMember("OffX"))
				WinAlarmConfig.InitOffXData(json["OffX"]);
			if (json.HasMember("OffY"))
				WinAlarmConfig.InitOffYData(json["OffY"]);
			if (json.HasMember("Width"))
				WinAlarmConfig.Width = json["Width"].GetInt();
			if (json.HasMember("Height"))
				WinAlarmConfig.Height = json["Height"].GetInt();
			if (json.HasMember("X"))
				WinAlarmConfig.X = json["X"].GetInt();
			if (json.HasMember("Y"))
				WinAlarmConfig.Y = json["Y"].GetInt();
			if (json.HasMember("IsEnable"))
				WinAlarmConfig.IsEnable = json["IsEnable"].GetBool();
			if (json.HasMember("CtrlName") && !json["CtrlName"].IsNull())
				WinAlarmConfig.CtrlName = json["CtrlName"].GetString();
			if (json.HasMember("WinNO"))
				WinAlarmConfig.WinNO = json["WinNO"].GetInt();
			if (json.HasMember("WinName") && !json["WinName"].IsNull())
				WinAlarmConfig.WinName = json["WinName"].GetString();
			if (json.HasMember("GroupName"))
				WinAlarmConfig.GroupName = json["GroupName"].GetInt();
			if (json.HasMember("ModePop"))
				WinAlarmConfig.ModePop = (Project::PopMode)json["ModePop"].GetInt();
			if (json.HasMember("CycleTime"))
				WinAlarmConfig.CycleTime = json["CycleTime"].GetInt();
			if (json.HasMember("AutoClose"))
				WinAlarmConfig.AutoClose = json["AutoClose"].GetBool();
			if (json.HasMember("AloneMode"))
				WinAlarmConfig.AloneMode = json["AloneMode"].GetBool();
			if (json.HasMember("CloseBtnMode"))
				WinAlarmConfig.CloseBtnMode = json["CloseBtnMode"].GetBool();
			if (json.HasMember("BgMode"))
				WinAlarmConfig.BgMode = json["BgMode"].GetInt();
			if (json.HasMember("BgColor"))
				WinAlarmConfig.BgColor = json["BgColor"].GetInt();
			if (json.HasMember("BgPicKey") && json["BgPicKey"].IsObject())
				WinAlarmConfig.BgPicKey.Parse(json["BgPicKey"]);
			if (json.HasMember("Perm"))
				WinAlarmConfig.Perm.Parse(json["Perm"]);
		}
	}
}