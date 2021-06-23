#include "SysSetGModel.h"
#include "Logger.h"
namespace UI
{
	SysSetGModel::SysSetGModel() {	}
	SysSetGModel::~SysSetGModel() {	}
	void SysSetGModel::GetRect(Rectangle & rect) {	}
	void SysSetGModel::InitTestData() {	}

	// ��ȡ�ؼ�����
	void * SysSetGModel::GetTestConfig() {
		return &SysSetGUnit;
	}

	void SysSetGModel::InitData(string jsonstr) {
		rapidjson::Document json;
		//����json�ַ���
		json.Parse(jsonstr.c_str());
		//��ȡ�������
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