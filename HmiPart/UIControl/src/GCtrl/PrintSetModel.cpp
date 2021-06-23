#include "PrintSetModel.h"
#include "Logger.h"
namespace UI
{

	void PrintSetModel::InitData(string jsonstr) {
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
			LOG_INFO_("PrintSetModel Parse json\n");

			if (json.HasMember("PrintEnable"))
				PrintConfig.PrintEnable = json["PrintEnable"].GetBool();
			if (json.HasMember("PrinterType"))
				PrintConfig.Type = (Project::PrinterType)json["PrinterType"].GetInt();
			if (json.HasMember("PrinterBrand") && !json["PrinterBrand"].IsNull())
				PrintConfig.PrinterBrand = json["PrinterBrand"].GetString();
			if (json.HasMember("PrinterModele") && !json["PrinterModele"].IsNull())
				PrintConfig.PrinterModele = json["PrinterModele"].GetString();
			if (json.HasMember("ComName") && !json["ComName"].IsNull())
				PrintConfig.ComName = json["ComName"].GetString();
			if (json.HasMember("ComType") && !json["ComType"].IsNull())
				PrintConfig.ComType = json["ComType"].GetString();
			if (json.HasMember("ComBaudreate"))
				PrintConfig.ComBaudreate = json["ComBaudreate"].GetInt();
			if (json.HasMember("ComParity"))
				PrintConfig.ComParity = json["ComParity"].GetInt();
			if (json.HasMember("ComDataBit"))
				PrintConfig.ComDataBit = json["ComDataBit"].GetInt();
			if (json.HasMember("ComStopBit"))
				PrintConfig.ComStopBit = json["ComStopBit"].GetInt();
			if (json.HasMember("CutPaperConfig"))
				PrintConfig.CutPaperConfig = (Project::CutPaperStyle)json["CutPaperConfig"].GetInt();
			if (json.HasMember("PaperSize"))
				PrintConfig.PaperSize = (Project::PaperSize)json["PaperSize"].GetInt();
			if (json.HasMember("IpAddress") && json["IpAddress"].IsArray())
			{
				vector<int>().swap(PrintConfig.IpAddress);
				for (size_t i = 0; i < json["IpAddress"].Size(); i++)
					PrintConfig.IpAddress.push_back(json["IpAddress"].GetInt());
			}
			if (json.HasMember("NetPort"))
				PrintConfig.NetPort = json["NetPort"].GetInt();
			if (json.HasMember("PrintDirection"))
				PrintConfig.PrintDirection = json["PrintDirection"].GetInt();
			if (json.HasMember("PrintTrigVar") && json["PrintTrigVar"].IsObject())
				PrintConfig.PrintTrigVar.Parse(json["PrintTrigVar"]);
			if (json.HasMember("PrintSource"))
				PrintConfig.PrintSource = (Project::PrintWin)json["PrintSource"].GetInt();
			if (json.HasMember("PrintSourceVar") && json["PrintSourceVar"].IsObject())
				PrintConfig.PrintSourceVar.Parse(json["PrintSourceVar"]);
			if (json.HasMember("PrintSourceWinNo"))
				PrintConfig.PrintSourceWinNo = json["PrintSourceWinNo"].GetInt();
			if (json.HasMember("PrintZoom"))
				PrintConfig.PrintZoom = json["PrintZoom"].GetDouble();
		}
	}

	void PrintSetModel::InitData(vector<char> data) {
		LOG_INFO_("PrintSetModel InitData\n");
		return BaseModel::InitData<decltype(PrintConfig)>(data, PrintConfig);
	}


}