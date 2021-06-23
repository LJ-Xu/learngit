#include "stdafx.h"
#include "DashboardModel.h"
#include <FL/Enumerations.H>
#include "JsonComm.h"
using namespace rapidjson;
namespace UI
{
	void DashboardModel::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
			return;
		}
		DashboardConfig.InitBaseData(json);
		//数据
		if (json.HasMember("DataCtlSign") && json["DataCtlSign"].IsObject())
			DashboardConfig.DataCtlSign.Parse(json["DataCtlSign"]);


		if (json.HasMember("MaxUseAddr"))
			DashboardConfig.MaxUseAddr = json["MaxUseAddr"].GetBool();
		if (json.HasMember("MinUseAddr"))
			DashboardConfig.MinUseAddr = json["MinUseAddr"].GetBool();

		if (json.HasMember("MaxData") && json["MaxData"].IsObject())
			DashboardConfig.MaxData.Parse(json["MaxData"]);
		if (json.HasMember("MinData") && json["MinData"].IsObject())
			DashboardConfig.MinData.Parse(json["MinData"]);

		if (json.HasMember("MaxValue"))
			DashboardConfig.MaxValue = json["MaxValue"].GetDouble();
		if (json.HasMember("MinValue"))
			DashboardConfig.MinValue = json["MinValue"].GetDouble();

		if (json.HasMember("DataColor"))
			DashboardConfig.DataColor = json["DataColor"].GetInt();
		if (json.HasMember("NormalRingWidth"))
			DashboardConfig.NormalRingWidth = json["NormalRingWidth"].GetInt();
		if (json.HasMember("RingToCircleCenterLength"))
			DashboardConfig.RingToCircleCenterLength = json["RingToCircleCenterLength"].GetInt();

		if (json.HasMember("IsUsedAlarmLimit"))
			DashboardConfig.IsUsedAlarmLimit = json["IsUsedAlarmLimit"].GetBool();
		if (json.HasMember("IsUpperWarnValueUsedRegister"))
			DashboardConfig.IsUpperWarnValueUsedRegister = json["IsUpperWarnValueUsedRegister"].GetBool();
		if (json.HasMember("IsLowerWarnValueUsedRegister"))
			DashboardConfig.IsLowerWarnValueUsedRegister = json["IsLowerWarnValueUsedRegister"].GetBool();

		if (json.HasMember("UpperWarnValue"))
			DashboardConfig.UpperWarnValue = json["UpperWarnValue"].GetDouble();
		if (json.HasMember("UpperWarnRegister") && json["UpperWarnRegister"].IsObject())
			DashboardConfig.UpperWarnRegister.Parse(json["UpperWarnRegister"]);

		if (json.HasMember("LowerWarnValue"))
			DashboardConfig.LowerWarnValue = json["LowerWarnValue"].GetDouble();
		if (json.HasMember("LowerWarnRegister") && json["LowerWarnRegister"].IsObject())
			DashboardConfig.LowerWarnRegister.Parse(json["LowerWarnRegister"]);


		if (json.HasMember("UpperWarnColor"))
			DashboardConfig.UpperWarnColor = json["UpperWarnColor"].GetInt();
		if (json.HasMember("LowerWarnColor"))
			DashboardConfig.LowerWarnColor = json["LowerWarnColor"].GetInt();
		if (json.HasMember("IsUsedDangerZone"))
			DashboardConfig.IsUsedDangerZone = json["IsUsedDangerZone"].GetBool();
		if (json.HasMember("IsUpperDangerUsedRegister"))
			DashboardConfig.IsUpperDangerUsedRegister = json["IsUpperDangerUsedRegister"].GetBool();
		if (json.HasMember("IsLowerDangerUsedRegister"))
			DashboardConfig.IsLowerDangerUsedRegister = json["IsLowerDangerUsedRegister"].GetBool();


		if (json.HasMember("UpperDangerValue"))
			DashboardConfig.UpperDangerValue = json["UpperDangerValue"].GetDouble();
		if (json.HasMember("UpperDangerRegister") && json["UpperDangerRegister"].IsObject())
			DashboardConfig.UpperDangerRegister.Parse(json["UpperDangerRegister"]);


		if (json.HasMember("LowerDangerValue"))
			DashboardConfig.LowerDangerValue = json["LowerDangerValue"].GetDouble();
		if (json.HasMember("LowerDangerRegister") && json["LowerDangerRegister"].IsObject())
			DashboardConfig.LowerDangerRegister.Parse(json["LowerDangerRegister"]);


		if (json.HasMember("DangerColor"))
			DashboardConfig.DangerColor = json["DangerColor"].GetInt();

		if (json.HasMember("DashBoardStyle"))
			DashboardConfig.DashBoardStyle.Parse(json["DashBoardStyle"]);

		if (json.HasMember("ArcDirection"))
			DashboardConfig.ArcDirection = json["ArcDirection"].GetInt();
		if (json.HasMember("StartAngle"))
			DashboardConfig.StartAngle = json["StartAngle"].GetInt();
		if (json.HasMember("EndAngle"))
			DashboardConfig.EndAngle = json["EndAngle"].GetInt();
		if (json.HasMember("Transparency"))
			DashboardConfig.Transparency = json["Transparency"].GetInt();

		if (json.HasMember("WatchHandleStyle"))
			DashboardConfig.WatchHandleStyle.Parse(json["WatchHandleStyle"]);

		if (json.HasMember("WatchHandleWidth"))
			DashboardConfig.WatchHandleWidth = json["WatchHandleWidth"].GetInt();
		if (json.HasMember("WatchHandleLength"))
			DashboardConfig.WatchHandleLength = json["WatchHandleLength"].GetInt();
		if (json.HasMember("WatchHandleInnerColor"))
			DashboardConfig.WatchHandleInnerColor = json["WatchHandleInnerColor"].GetInt();
		if (json.HasMember("WatchHandleBorderColor"))
			DashboardConfig.WatchHandleBorderColor = json["WatchHandleBorderColor"].GetInt();
		if (json.HasMember("AxisStyle"))
			DashboardConfig.AxisStyle.Parse(json["AxisStyle"]);
		if (json.HasMember("AxisRadius"))
			DashboardConfig.AxisRadius = json["AxisRadius"].GetInt();
		if (json.HasMember("AxisInnerColor"))
			DashboardConfig.AxisInnerColor = json["AxisInnerColor"].GetInt();
		if (json.HasMember("AxisBorderColor"))
			DashboardConfig.AxisBorderColor = json["AxisBorderColor"].GetInt();

		if (json.HasMember("ShowGraduate"))
			DashboardConfig.ShowGraduate = json["ShowGraduate"].GetBool();

		if (json.HasMember("MainGraduateCount"))
			DashboardConfig.MainGraduateCount = json["MainGraduateCount"].GetInt();
		if (json.HasMember("SecondaryGraduateCount"))
			DashboardConfig.SecondaryGraduateCount = json["SecondaryGraduateCount"].GetInt();
		if (json.HasMember("MainGraduateLen"))
			DashboardConfig.MainGraduateLen = json["MainGraduateLen"].GetInt();
		if (json.HasMember("SecondaryGraduateLen"))
			DashboardConfig.SecondaryGraduateLen = json["SecondaryGraduateLen"].GetInt();
		if (json.HasMember("GraduateColor"))
			DashboardConfig.GraduateColor = json["GraduateColor"].GetInt();
		if (json.HasMember("GraduatePosition"))
			DashboardConfig.GraduatePosition = json["GraduatePosition"].GetInt();
		if (json.HasMember("ShowGraduateSign"))
			DashboardConfig.ShowGraduateSign = json["ShowGraduateSign"].GetInt();


		if (json.HasMember("TypeFaceGraduate") && json["TypeFaceGraduate"].IsObject())
			DashboardConfig.TypeFaceGraduate.Parse(json["TypeFaceGraduate"]);


		if (json.HasMember("DecimalDigit"))
			DashboardConfig.DecimalDigit = json["DecimalDigit"].GetInt();
		if (json.HasMember("IntegerDigit"))
			DashboardConfig.IntegerDigit = json["IntegerDigit"].GetInt();

		if (json.HasMember("CuttedBoardStyle"))
			DashboardConfig.CuttedBoardStyle.Parse(json["CuttedBoardStyle"]);
		if (json.HasMember("IsCuttedBoarder"))
			DashboardConfig.IsCuttedBoarder = json["IsCuttedBoarder"].GetBool();

	}

	void DashboardModel::InitTestData()
	{
		DashboardConfig.X = 0;
		DashboardConfig.Y = 0;
		DashboardConfig.Width = 150;
		DashboardConfig.Height = 150;
		DashboardConfig.IsEnable = false;
		DashboardConfig.CtrlName = "Dashboard0";

		DashboardConfig.MaxValue = 100.0;
		DashboardConfig.MinValue = 0.0;
		DashboardConfig.DataColor = 32768;
		DashboardConfig.NormalRingWidth = 10;
		DashboardConfig.IsUsedAlarmLimit = true;
		DashboardConfig.IsUpperWarnValueUsedRegister = false;
		DashboardConfig.IsLowerWarnValueUsedRegister = false;
		DashboardConfig.UpperWarnValue = 80.0;
		DashboardConfig.LowerWarnValue = 20.0;
		DashboardConfig.UpperWarnColor = 255;
		DashboardConfig.LowerWarnColor = 65535;
		DashboardConfig.IsUsedDangerZone = true;
		DashboardConfig.IsUpperDangerUsedRegister = false;
		DashboardConfig.IsLowerDangerUsedRegister = false;
		DashboardConfig.UpperDangerValue = 60.0;
		DashboardConfig.LowerDangerValue = 40.0;
		DashboardConfig.DangerColor = 16711680;
		DashboardConfig.ArcDirection = 1;
		DashboardConfig.StartAngle = 0;
		DashboardConfig.EndAngle = 100;
		DashboardConfig.Transparency = 50;
		DashboardConfig.WatchHandleWidth = 10;
		DashboardConfig.WatchHandleLength = 50;
		DashboardConfig.WatchHandleInnerColor = 0;
		DashboardConfig.WatchHandleBorderColor = 0;
		DashboardConfig.AxisRadius = 5;
		DashboardConfig.AxisInnerColor = 16777215;
		DashboardConfig.AxisBorderColor = 0;
		DashboardConfig.ShowGraduate = true;
		DashboardConfig.MainGraduateCount = 5;
		DashboardConfig.SecondaryGraduateCount = 3;
		DashboardConfig.MainGraduateLen = 12;
		DashboardConfig.SecondaryGraduateLen = 9;
		DashboardConfig.GraduateColor = 0;
		DashboardConfig.GraduatePosition = 1;
		DashboardConfig.ShowGraduateSign = 1;
		DashboardConfig.TypeFaceGraduate.Colors = 0;
		DashboardConfig.TypeFaceGraduate.Font.Name = "Arial";
		DashboardConfig.TypeFaceGraduate.Font.Size = 12.0;
		DashboardConfig.TypeFaceGraduate.Alpha = 255;
		DashboardConfig.TypeFaceGraduate.Align = 0;
		DashboardConfig.DecimalDigit = 2;
		DashboardConfig.IntegerDigit = 1;

	}


}


