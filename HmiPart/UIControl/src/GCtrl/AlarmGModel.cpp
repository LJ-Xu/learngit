#include "AlarmGModel.h"

namespace UI
{
	AlarmGModel::AlarmGModel() {

	}

	AlarmGModel::~AlarmGModel() {

	}

	void AlarmGModel::GetRect(Rectangle & rect) {

	}

	void AlarmGModel::InitTestData() {

	}

	// 获取控件配置
	void * AlarmGModel::GetTestConfig() {
		return &AlarmGUnit;
	}

	void AlarmGModel::InitData(string jsonstr) {
		rapidjson::Document json;
		 //解析json字符串
		json.Parse(jsonstr.c_str());
		 //获取解析结果
		if (json.HasParseError()) {
			cout << "Parse Error : ("
				<< json.GetParseError() << " : "
				<< json.GetErrorOffset() << ")" << endl;
		} else {
			if (json.HasMember("AlarmLogin") && json["AlarmLogin"].IsObject()
				&& json["AlarmLogin"].HasMember("AlarmConfigProperty") && 
				json["AlarmLogin"]["AlarmConfigProperty"].IsObject())
			{
				rapidjson::Value &alarmjson = json["AlarmLogin"]["AlarmConfigProperty"];

				if (alarmjson.HasMember("IsHisEvent") && alarmjson["IsHisEvent"].IsBool())
					AlarmGUnit.IsSave = alarmjson["IsHisEvent"].GetBool();
				ParseAlarmSaveLst(AlarmGUnit.SaveLst, alarmjson);
				if (alarmjson.HasMember("ConfigInfoLst") && alarmjson["ConfigInfoLst"].IsArray())
					Project::AlarmInfoRes::Parse(AlarmGUnit.InfoLst, alarmjson["ConfigInfoLst"]);
			}
		}
	}

	void AlarmGModel::InitData(vector<char> data) {
		return BaseModel::InitData<decltype(AlarmGUnit)>(data, AlarmGUnit);
	}

	void AlarmGModel::ParseAlarmSaveLst(Project::SaveFileRes & svrs, rapidjson::Value & alarmjson)
	{
		if (alarmjson.HasMember("StoreSite"))
			svrs.StoreLocation = (Project::StoreLocat)alarmjson["StoreSite"].GetInt();
		if (alarmjson.HasMember("StorePosVarId") && alarmjson["StorePosVarId"].IsObject())
			svrs.StorePosVarId.Parse(alarmjson["StorePosVarId"]);
		if (alarmjson.HasMember("HisInfoLst") && alarmjson["HisInfoLst"].IsArray())
		{
			rapidjson::Value & jsonLst = alarmjson["HisInfoLst"];
			vector<string>().swap(svrs.Contents);
			memset(svrs.SaveFormat, 0, 10);
			for (unsigned i = 0; i < jsonLst.Size(); i++)
			{
				bool select;
				if (jsonLst[i].HasMember("IsSelect"))
					select = jsonLst[i]["IsSelect"].GetBool();
				if (!select)
					continue;
				if (jsonLst[i].HasMember("Describe")
					&& !jsonLst[i]["Describe"].IsNull())
				{
					string strTemp = jsonLst[i]["Describe"].GetString();
					if (!strTemp.compare("序号"))
					{
						svrs.SaveFormat[i] = SAVE_ID;
					}
					else if (!strTemp.compare("报警触发日期"))
					{
						svrs.SaveFormat[i] = SAVE_DATE;
					}
					else if (!strTemp.compare("报警触发时间"))
					{
						svrs.SaveFormat[i] = SAVE_TIME;
					}
					else if (!strTemp.compare("报警信息"))
					{
						svrs.SaveFormat[i] = SAVE_INFO;
					}
					else if (!strTemp.compare("报警次数"))
					{
						svrs.SaveFormat[i] = SAVE_COUNT;
					}
					else if (!strTemp.compare("确认时间"))
					{
						svrs.SaveFormat[i] = SAVE_CONFIRM;
					}
					else
					{
						svrs.SaveFormat[i] = SAVE_RECOVERYTIME;
					}
					svrs.Contents.push_back(strTemp);
				}
			}
		}
		if (alarmjson.HasMember("FileName") && !alarmjson["FileName"].IsNull())
			svrs.FileName = alarmjson["FileName"].GetString();
		if (alarmjson.HasMember("FileNameMode"))
			svrs.FileNameMode = (Project::FlieNameType)alarmjson["FileNameMode"].GetInt();
		if (alarmjson.HasMember("FileNameVarId") && alarmjson["FileNameVarId"].IsObject())
			svrs.FileNameDataVar.Parse(alarmjson["FileNameVarId"]);
		if (alarmjson.HasMember("LackLocationMode"))
			svrs.StoreSpaceLack = alarmjson["LackLocationMode"].GetInt();
		if (alarmjson.HasMember("IsFileSaveLimit"))
			svrs.IsFileSaveTimeLimit = alarmjson["IsFileSaveLimit"].GetBool();
		if (alarmjson.HasMember("SaveDays"))
			svrs.SaveDays = alarmjson["SaveDays"].GetInt();


	}

	unique_ptr<vector<char>> AlarmGModel::GetData() {
		return BaseModel::GetData<decltype(AlarmGUnit)>(AlarmGUnit);
	}
}