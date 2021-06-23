#include "SysSetGModel.h"
#include "Logger.h"
namespace UI
{
	SysSetGModel::SysSetGModel() {	}
	SysSetGModel::~SysSetGModel() {	}
	void SysSetGModel::GetRect(Rectangle & rect) {	}
	void SysSetGModel::InitTestData() {	}

	// 获取控件配置
	void * SysSetGModel::GetTestConfig() {
		return &SysSetGUnit;
	}

	void SysSetGModel::InitData(string jsonstr) {
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
			LOG_INFO_("SysSetGModel Parse Json\n");

			if (json.HasMember("SysSetUnit") && json["SysSetUnit"].IsObject())
			{
				rapidjson::Value &sysjson = json["SysSetUnit"];
				if (sysjson.HasMember("IsCtrlPageExchange"))
					SysSetGUnit.Exchange.IsCtrlPageExchange = sysjson["IsCtrlPageExchange"].GetBool();
				if (sysjson.HasMember("CtrlPageExchangeVarId") && sysjson["CtrlPageExchangeVarId"].IsObject())
					SysSetGUnit.Exchange.CtrlPageExchangeVarId.Parse(sysjson["CtrlPageExchangeVarId"]);
				if (sysjson.HasMember("IsReportCurPageNo"))
					SysSetGUnit.Exchange.IsReportCurPageNo = sysjson["IsReportCurPageNo"].GetBool();
				if (sysjson.HasMember("ReportCurPageNoVarId") && sysjson["ReportCurPageNoVarId"].IsObject())
					SysSetGUnit.Exchange.ReportCurPageNoVarId.Parse(sysjson["ReportCurPageNoVarId"]);
			}
		}
	}

	void SysSetGModel::InitData(vector<char> data) {
		LOG_INFO_("SysSetGModel InitData\n");
		return BaseModel::InitData<decltype(SysSetGUnit)>(data, SysSetGUnit);
	}

	unique_ptr<vector<char>> SysSetGModel::GetData() {
		return BaseModel::GetData<decltype(SysSetGUnit)>(SysSetGUnit);
	}
}