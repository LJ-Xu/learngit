#include "ProjectSystemSet.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
using namespace rapidjson;
namespace Project
{
	void ProjectSystemSet::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else
		{
			if (json.HasMember("SystemSetProperty") && json["SystemSetProperty"].IsObject())
			{
				Value &system = json["SystemSetProperty"];
				if (system.HasMember("StartPage"))
					Param.StartPage = system["StartPage"].GetInt();
				if (system.HasMember("WaitTime"))
					Param.WaitTime = system["WaitTime"].GetInt64();
				if (system.HasMember("IsOffBgLed"))
					Param.IsOffBgLed = system["IsOffBgLed"].GetBool();
				if (system.HasMember("IsViewPage"))
					Param.IsViewPage = system["IsViewPage"].GetBool();
				if (system.HasMember("ViewPageIndex"))
					Param.ViewPageIndex = system["ViewPageIndex"].GetInt();
				if (system.HasMember("IsHideCursor"))
					Param.IsHideCursor = system["IsHideCursor"].GetBool();
				if (system.HasMember("CursorSize"))
					Param.CSize = (CursorSize)system["CursorSize"].GetInt();
				if (system.HasMember("IsBuzzer"))
					Param.DisableBuzzer = system["IsBuzzer"].GetBool();

				if (system.HasMember("IsVertical"))
					Monitor.IsVertical = system["IsVertical"].GetBool();
				if (system.HasMember("ScreenModel") && !system["ScreenModel"].IsNull())
					Monitor.ScreenModel = system["ScreenModel"].GetString();
				if (system.HasMember("PartRatioW"))
					Monitor.PartRatioW = system["PartRatioW"].GetDouble();
				if (system.HasMember("PartRatioH"))
					Monitor.PartRatioH = system["PartRatioH"].GetDouble();
				if (system.HasMember("HScreenRatio"))
					Monitor.HScreenRatio = system["HScreenRatio"].GetDouble();
				if (system.HasMember("WScreenRatio"))
					Monitor.WScreenRatio = system["WScreenRatio"].GetDouble();
				if (system.HasMember("ZoomMode"))
					Monitor.ZoomMode = system["ZoomMode"].GetInt();

				if (system.HasMember("IsHmiCurIp"))
					Display.IsHmiCurIp = system["IsHmiCurIp"].GetBool();
				if (system.HasMember("IsConnectDevice"))
					Display.IsConnectDevice = system["IsConnectDevice"].GetBool();
				if (system.HasMember("IsClock"))
					Display.IsClock = system["IsClock"].GetBool();
				if (system.HasMember("IsRemoteComm"))
					Display.IsRemoteComm = system["IsRemoteComm"].GetBool();
				if (system.HasMember("IsRemoteMonitor"))
					Display.IsRemoteMonitor = system["IsRemoteMonitor"].GetBool();
				if (system.HasMember("Iscalibrate"))
					Display.Iscalibrate = system["Iscalibrate"].GetBool();
				if (system.HasMember("IsFindUDisk"))
					Display.IsFindUDisk = system["IsFindUDisk"].GetBool();

				if (system.HasMember("PwdLogin"))
					VNC.PwdLogin = system["PwdLogin"].GetInt();
				if (system.HasMember("VNCPwd") && !system["VNCPwd"].IsNull())
					VNC.VNCPwd = system["VNCPwd"].GetString();
				if (system.HasMember("IsMonitorMode"))
					VNC.IsMonitorMode = system["IsMonitorMode"].GetBool();
				if (system.HasMember("IsRemoteCommFlag"))
					VNC.IsRemoteCommFlag = system["IsRemoteCommFlag"].GetBool();
				if (system.HasMember("RemoteCommPort"))
					VNC.RemoteCommPort = system["RemoteCommPort"].GetInt();
				if (system.HasMember("SafePattern") && !system["SafePattern"].IsNull())
					VNC.SafePattern = system["SafePattern"].GetString();
				if (system.HasMember("Serverdomain") && !system["Serverdomain"].IsNull())
					VNC.Serverdomain = system["Serverdomain"].GetString();
				if (system.HasMember("ServerIp") && system["ServerIp"].IsArray())
				{
					vector<int>().swap(VNC.ServerIp);
					for (size_t i = 0; i < system["ServerIp"].Size(); i++)
						VNC.ServerIp.push_back(system["ServerIp"][i].GetInt());
				}
				if (system.HasMember("DevicePwd") && !system["DevicePwd"].IsNull())
					VNC.DevicePwd = system["DevicePwd"].GetString();

				if (system.HasMember("IsbanClockSet"))
					Clock.IsbanClockSet = system["IsbanClockSet"].GetBool();
				if (system.HasMember("ClockViewFormat"))
					Clock.ClockViewFormat = system["ClockViewFormat"].GetInt();
				if (system.HasMember("ClockSource"))
					Clock.ClockSource = system["ClockSource"].GetInt();
				if (system.HasMember("OutDeviceVarId") && system["OutDeviceVarId"].IsArray())
					Project::DataVarId::Parse(Clock.OutDeviceVarId, system["OutDeviceVarId"]);
					//Clock.OutDeviceVarId.Parse(system["OutDeviceVarId"]);
				if (system.HasMember("IsClockWriteinPlc"))
					Clock.IsClockWriteinPlc = system["IsClockWriteinPlc"].GetBool();
				if (system.HasMember("WriteStyle"))
					Clock.WriteMode = (ClockWriteMode)system["WriteStyle"].GetInt();
				if (system.HasMember("TriReadAddrVarId") && system["TriReadAddrVarId"].IsObject())
					Clock.TriReadAddrVarId.Parse(system["TriReadAddrVarId"]);
				if (system.HasMember("TriPattern"))
					Clock.TriPattern = system["TriPattern"].GetInt();
				if (system.HasMember("WriteCycle"))
					Clock.WriteCycle = system["WriteCycle"].GetInt();
				if (system.HasMember("CycleUnit"))
					Clock.CycleUnit = system["CycleUnit"].GetInt();
				if (system.HasMember("CycleVarId") && system["CycleVarId"].IsObject())
					Clock.CycleVarId.Parse(system["CycleVarId"]);
				if (system.HasMember("WriteInPlcDevices") && system["WriteInPlcDevices"].IsArray())
				{
					vector<Project::WritePlcDevicesSet>().swap(Clock.WriteInPlcDevices);
					for (size_t i = 0; i < system["WriteInPlcDevices"].Size(); i++)
					{
						Project::WritePlcDevicesSet tmpvarid;
						if (system["WriteInPlcDevices"][i].HasMember("ClockWriteViewFormat"))
							tmpvarid.ClockWriteViewFormat = system["WriteInPlcDevices"][i]["ClockWriteViewFormat"].GetInt();
						if (system["WriteInPlcDevices"][i].HasMember("DeviceVarId") &&
							system["WriteInPlcDevices"][i]["DeviceVarId"].IsArray())
							Project::DataVarId::Parse(tmpvarid.TimeVids, system["WriteInPlcDevices"][i]["DeviceVarId"]);
							//tmpvarid.Parse(system["WriteInPlcDevices"][i]["DeviceVarId"]);
						Clock.WriteInPlcDevices.push_back(tmpvarid);
					}
				}
				Clock.WriteInPlcDevicesNum = Clock.WriteInPlcDevices.size();

				if (system.HasMember("IsCtrlPageExchange"))
					Exchange.IsCtrlPageExchange = system["IsCtrlPageExchange"].GetBool();
				if (system.HasMember("CtrlPageExchangeVarId") && system["CtrlPageExchangeVarId"].IsObject())
					Exchange.CtrlPageExchangeVarId.Parse(system["CtrlPageExchangeVarId"]);
				if (system.HasMember("IsReportCurPageNo"))
					Exchange.IsReportCurPageNo = system["IsReportCurPageNo"].GetBool();
				if (system.HasMember("ReportCurPageNoVarId") && system["ReportCurPageNoVarId"].IsObject())
					Exchange.ReportCurPageNoVarId.Parse(system["ReportCurPageNoVarId"]);

			}
		}
	}
}
