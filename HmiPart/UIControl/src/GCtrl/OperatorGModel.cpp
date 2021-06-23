#include "OperatorGModel.h"
#include "Logger.h"
namespace UI
{
	OperatorGModel::OperatorGModel() {	}
	OperatorGModel::~OperatorGModel() {	}
	void OperatorGModel::GetRect(Rectangle & rect) {	}
	void OperatorGModel::InitTestData() {	}

	// 获取控件配置
	void * OperatorGModel::GetTestConfig() {
		return &OperatorGConfig;
	}

	void OperatorGModel::InitData(string jsonstr) {
		rapidjson::Document json;
		//解析json字符串
		json.Parse(jsonstr.c_str());
		//获取解析结果
		if (json.HasParseError()) {
			cout << "Parse Error : ("
				<< json.GetParseError() << " : "
				<< json.GetErrorOffset() << ")" << endl;
		}
		else {
			LOG_INFO_("OperatorGModel Parse json\n");

			if (json.HasMember("OperationRecordSetUnit") && json["OperationRecordSetUnit"].IsObject())
			{
				rapidjson::Value &recordjson = json["OperationRecordSetUnit"];
				if (recordjson.HasMember("Enable"))
					OperatorGConfig.Enable = recordjson["Enable"].GetBool();
				if (recordjson.HasMember("CmdIdRef") && recordjson["CmdIdRef"].IsObject())
					OperatorGConfig.CmdIdRef.Parse(recordjson["CmdIdRef"]);
				if (recordjson.HasMember("ResultIdRef") && recordjson["ResultIdRef"].IsObject())
					OperatorGConfig.ResultIdRef.Parse(recordjson["ResultIdRef"]);
				if (recordjson.HasMember("ControlIdRef") && recordjson["ControlIdRef"].IsObject())
					OperatorGConfig.ControlIdRef.Parse(recordjson["ControlIdRef"]);

				if (recordjson.HasMember("FileNameMode"))
					OperatorGConfig.SaveLst.FileNameMode = (Project::FlieNameType)recordjson["FileNameMode"].GetInt();
				if (recordjson.HasMember("FileName") && !recordjson["FileName"].IsNull())
					OperatorGConfig.SaveLst.FileName = recordjson["FileName"].GetString();
				if (recordjson.HasMember("StoragePosition"))
					OperatorGConfig.SaveLst.FileStoreSite = (Project::FlieLocation)recordjson["StoragePosition"].GetInt();
				if (recordjson.HasMember("DynamicfileNameIdRef") && recordjson["DynamicfileNameIdRef"].IsObject())
					OperatorGConfig.SaveLst.FileNameDataVar.Parse(recordjson["DynamicfileNameIdRef"]);
				if (recordjson.HasMember("StoragePositionDataVar") && recordjson["StoragePositionDataVar"].IsObject())
					OperatorGConfig.SaveLst.StorePosVarId.Parse(recordjson["StoragePositionDataVar"]);
				if (recordjson.HasMember("SpaceInsufficientAction"))
					OperatorGConfig.SaveLst.StoreSpaceLack = recordjson["SpaceInsufficientAction"].GetInt();
				if (recordjson.HasMember("HmiRecordNumber"))
					OperatorGConfig.SaveLst.UpperLmt = recordjson["HmiRecordNumber"].GetInt();
				if (recordjson.HasMember("SaveItems") && recordjson["SaveItems"].IsArray())
				{
					//bool select;
					std::vector<string>().swap(OperatorGConfig.SaveLst.Contents);
					for (unsigned i = 0; i < recordjson["SaveItems"].Size(); i++)
					{
						/*if (recordjson["SaveItems"][i].HasMember("IsSelect"))
							select = recordjson["SaveItems"][i]["IsSelect"].GetBool();*/
						/*if (select && recordjson["SaveItems"][i].HasMember("Describe") &&
							!recordjson["SaveItems"][i]["Describe"].IsNull())
							OperatorGConfig.SaveLst.Contents.push_back(recordjson["SaveItems"][i]["Describe"].GetString());*/
						OperatorGConfig.SaveLst.Contents.push_back(recordjson["SaveItems"][i].GetString());
					}
				}
			}
			if (json.HasMember("OperationRecordSetPrint") && json["OperationRecordSetPrint"].IsObject())
				OperatorGConfig.PrintSet.Parse(json["OperationRecordSetPrint"]);
		}
	}

	void OperatorGModel::InitData(vector<char> data) {
		LOG_INFO_("OperatorGModel InitData\n");
		return BaseModel::InitData<decltype(OperatorGConfig)>(data, OperatorGConfig);
	}

	unique_ptr<vector<char>> OperatorGModel::GetData() {
		return BaseModel::GetData<decltype(OperatorGConfig)>(OperatorGConfig);
	}
}