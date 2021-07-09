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
				if (sysjson.HasMember("IsbanClockSet"))
					SysSetGUnit.Clock.IsbanClockSet = sysjson["IsbanSysSetGUnit.ClockSet"].GetBool();
				if (sysjson.HasMember("ClockViewFormat"))
					SysSetGUnit.Clock.ClockViewFormat = sysjson["ClockViewFormat"].GetInt();
				if (sysjson.HasMember("ClockSource"))
					SysSetGUnit.Clock.ClockSource = sysjson["ClockSource"].GetInt();
				if (sysjson.HasMember("OutDeviceVarId") && sysjson["OutDeviceVarId"].IsArray())
					Project::DataVarId::Parse(SysSetGUnit.Clock.OutDeviceVarId, sysjson["OutDeviceVarId"]);
				//SysSetGUnit.Clock.OutDeviceVarId.Parse(sysjson["OutDeviceVarId"]);
				if (sysjson.HasMember("IsClockWriteinPlc"))
					SysSetGUnit.Clock.IsClockWriteinPlc = sysjson["IsClockWriteinPlc"].GetBool();
				if (sysjson.HasMember("WriteStyle"))
					SysSetGUnit.Clock.WriteMode = (Project::ClockWriteMode)sysjson["WriteStyle"].GetInt();
				if (sysjson.HasMember("TriReadAddrVarId") && sysjson["TriReadAddrVarId"].IsObject())
					SysSetGUnit.Clock.TriReadAddrVarId.Parse(sysjson["TriReadAddrVarId"]);
				if (sysjson.HasMember("TriPattern"))
					SysSetGUnit.Clock.TriPattern = sysjson["TriPattern"].GetInt();
				if (sysjson.HasMember("WriteCycle"))
					SysSetGUnit.Clock.WriteCycle = sysjson["WriteCycle"].GetInt();
				if (sysjson.HasMember("CycleUnit"))
					SysSetGUnit.Clock.CycleUnit = sysjson["CycleUnit"].GetInt();
				if (sysjson.HasMember("CycleVarId") && sysjson["CycleVarId"].IsObject())
					SysSetGUnit.Clock.CycleVarId.Parse(sysjson["CycleVarId"]);
				if (sysjson.HasMember("WriteInPlcDevices") && sysjson["WriteInPlcDevices"].IsArray())
				{
					vector<Project::WritePlcDevicesSet>().swap(SysSetGUnit.Clock.WriteInPlcDevices);
					for (size_t i = 0; i < sysjson["WriteInPlcDevices"].Size(); i++)
					{
						Project::WritePlcDevicesSet tmpvarid;
						if (sysjson["WriteInPlcDevices"][i].HasMember("ClockWriteViewFormat"))
							tmpvarid.ClockWriteViewFormat = sysjson["WriteInPlcDevices"][i]["ClockWriteViewFormat"].GetInt();
						if (sysjson["WriteInPlcDevices"][i].HasMember("DeviceVarId") &&
							sysjson["WriteInPlcDevices"][i]["DeviceVarId"].IsArray())
							Project::DataVarId::Parse(tmpvarid.TimeVids, sysjson["WriteInPlcDevices"][i]["DeviceVarId"]);
						//tmpvarid.Parse(system["WriteInPlcDevices"][i]["DeviceVarId"]);
						SysSetGUnit.Clock.WriteInPlcDevices.push_back(tmpvarid);
					}
				}
				SysSetGUnit.Clock.WriteInPlcDevicesNum = SysSetGUnit.Clock.WriteInPlcDevices.size();
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