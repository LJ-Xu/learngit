/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : XYLineChartModel.cpp
 * Author   : qiaodan
 * Date     : 2020-12-07
 * Descript : 解析json，关于XY折线图图参数内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "XYLineChartModel.h"
#include <FL/Fl.H>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "XYLineChartViewUnit.h"
using namespace rapidjson;
namespace UI
{
	void XYLineChartModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/XYLineChart.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else
		{
			LOG_INFO_("XYLineChartModel Parse Json\n");
			ChartXYLineConfig.InitBaseData(json);
			if (json.HasMember("Type"))
				ChartXYLineConfig.RefreshMode = json["Type"].GetInt();
			if (json.HasMember("SamplingCycle"))
				ChartXYLineConfig.SamplingCycle = json["SamplingCycle"].GetInt();
			if (json.HasMember("TriggerTypeVar") && json["TriggerTypeVar"].IsObject())
				ChartXYLineConfig.TriggerTypeVar.Parse(json["TriggerTypeVar"]);
			if (json.HasMember("TriggerCondition"))
				ChartXYLineConfig.TriggerCond = json["TriggerCondition"].GetInt();
			if (json.HasMember("PauseControlVar") && json["PauseControlVar"].IsObject())
				ChartXYLineConfig.PauseControlVar.Parse(json["PauseControlVar"]);
			if (json.HasMember("PauseControlCondition"))
				ChartXYLineConfig.PauseControlCondition = json["PauseControlCondition"].GetInt();
			if (json.HasMember("ClearControlCondition"))
				ChartXYLineConfig.ClearControlCondition = json["ClearControlCondition"].GetInt();
			if (json.HasMember("ClearControlVar") && json["ClearControlVar"].IsObject())
				ChartXYLineConfig.ClearControlVar.Parse(json["ClearControlVar"]);
			if (json.HasMember("UseZoom"))
				ChartXYLineConfig.UseZoom = json["UseZoom"].GetBool();
			if (json.HasMember("ZoomPercent"))
				ChartXYLineConfig.ZoomPercent = json["ZoomPercent"].GetInt();
			if (json.HasMember("PageTurningControlStyle"))
				ChartXYLineConfig.PageControlStyle = json["PageTurningControlStyle"].GetInt();
			if (json.HasMember("TurningControlStyleVar") && json["TurningControlStyleVar"].IsObject())
				ChartXYLineConfig.TurnControlVar.Parse(json["TurningControlStyleVar"]);
			if (json.HasMember("SamplingPoints"))
				ChartXYLineConfig.SamplePoints = json["SamplingPoints"].GetInt();
			if (json.HasMember("SamplingPointsVar") && json["SamplingPointsVar"].IsObject())
				ChartXYLineConfig.SamplePointsVar.Parse(json["SamplingPointsVar"]);
			if (json.HasMember("PointsPerScreen"))
				ChartXYLineConfig.PointsPerScreen = json["PointsPerScreen"].GetInt();
			if (json.HasMember("ChanelNum"))
				ChartXYLineConfig.ChannelNum = json["ChanelNum"].GetInt();
			if (json.HasMember("DataChanels") && json["DataChanels"].IsArray())
				Project::XYLineChannel::Parse(ChartXYLineConfig.DataChanel, json["DataChanels"]);

			if (json.HasMember("ReferenceLineNum"))
				ChartXYLineConfig.ReferenceLineNum = json["ReferenceLineNum"].GetInt();
			if (json.HasMember("ReferenceLines") && json["ReferenceLines"].IsArray())
			{
				vector<Project::ReferenceLine>().swap(ChartXYLineConfig.ReferenceLines);
				for (unsigned i = 0; i < json["ReferenceLines"].Size(); i++)
				{
					Project::ReferenceLine tmpline;
					if (json["ReferenceLines"][i].HasMember("LineColor"))
						tmpline.LineColor = json["ReferenceLines"][i]["LineColor"].GetInt();
					if (json["ReferenceLines"][i].HasMember("LineValue"))
						tmpline.LineValue = json["ReferenceLines"][i]["LineValue"].GetDouble();
					if (json["ReferenceLines"][i].HasMember("LineValueVar") &&
						json["ReferenceLines"][i]["LineValueVar"].IsObject())
						tmpline.LineValueVar.Parse(json["ReferenceLines"][i]["LineValueVar"]);
					ChartXYLineConfig.ReferenceLines.push_back(tmpline);
				}
			}
			if (json.HasMember("LineChartBackColor"))
				ChartXYLineConfig.LineChartBackColor = json["LineChartBackColor"].GetInt();
			if (json.HasMember("ScaleAreaBackColor"))
				ChartXYLineConfig.ScaleAreaBackColor = json["ScaleAreaBackColor"].GetInt();
			if (json.HasMember("UseGrid"))
				ChartXYLineConfig.UseGrid = json["UseGrid"].GetBool();
			if (json.HasMember("XGridEqualNum"))
				ChartXYLineConfig.XGridEqualNum = json["XGridEqualNum"].GetInt();
			if (json.HasMember("YGridEqualNum"))
				ChartXYLineConfig.YGridEqualNum = json["YGridEqualNum"].GetInt();
			if (json.HasMember("GridLineStyle"))
				ChartXYLineConfig.GridStyle.Type = json["GridLineStyle"].GetInt();
			if (json.HasMember("GridLineColor"))
				ChartXYLineConfig.GridStyle.Color = json["GridLineColor"].GetInt();
			if (json.HasMember("DrawPosition") && json["DrawPosition"].IsObject())
			{
				Value &value = json["DrawPosition"];
				if (value.HasMember("OriginPos") && value["OriginPos"].IsObject())
				{
					if (value["OriginPos"].HasMember("x"))
						ChartXYLineConfig.OriginPos.X = value["OriginPos"]["x"].GetInt();
					if (value["OriginPos"].HasMember("y"))
						ChartXYLineConfig.OriginPos.Y = value["OriginPos"]["y"].GetInt();
				}
				if (value.HasMember("XaxisLen"))
					ChartXYLineConfig.XaxisLen = value["XaxisLen"].GetInt();
				if (value.HasMember("YaxisLen"))
					ChartXYLineConfig.YaxisLen = value["YaxisLen"].GetInt();
				if (value.HasMember("XscaleInfo") && value["XscaleInfo"].IsArray())
					Project::ScaleInfo::Parse(ChartXYLineConfig.XScaleInfo, value["XscaleInfo"]);
				if (value.HasMember("YscaleInfo") && value["YscaleInfo"].IsArray())
					Project::ScaleInfo::Parse(ChartXYLineConfig.YScaleInfo, value["YscaleInfo"]);
				if (value.HasMember("ScrollHeight"))
					ChartXYLineConfig.ScrollHeight = value["ScrollHeight"].GetInt();
			}

			if (json.HasMember("UseXScale"))
				ChartXYLineConfig.UseXScale = json["UseXScale"].GetBool();
			if (json.HasMember("XUpperLimit"))
				ChartXYLineConfig.XScaleSet.UpperLimit = json["XUpperLimit"].GetDouble();
			if (json.HasMember("XUpperLimitVar") && json["XUpperLimitVar"].IsObject())
				ChartXYLineConfig.XScaleSet.UpperLimitVar.Parse(json["XUpperLimitVar"]);
			if (json.HasMember("XLowerLimit"))
				ChartXYLineConfig.XScaleSet.LowerLimit = json["XLowerLimit"].GetDouble();
			if (json.HasMember("XLowerLimitVar") && json["XLowerLimitVar"].IsObject())
				ChartXYLineConfig.XScaleSet.LowerLimitVar.Parse(json["XLowerLimitVar"]);
			if (json.HasMember("XScaleColor"))
				ChartXYLineConfig.XScaleSet.ScaleColor = json["XScaleColor"].GetInt();
			if (json.HasMember("XPrimaryScaleNum"))
				ChartXYLineConfig.XScaleSet.PrimaryScaleNum = json["XPrimaryScaleNum"].GetInt();
			if (json.HasMember("XPrimaryScaleLen"))
				ChartXYLineConfig.XScaleSet.PrimaryScaleLen = json["XPrimaryScaleLen"].GetInt();
			if (json.HasMember("UseXSecondaryScaleNum"))
				ChartXYLineConfig.XScaleSet.UseSecondaryScaleNum = json["UseXSecondaryScaleNum"].GetBool();
			if (json.HasMember("XSecondaryScaleNum"))
				ChartXYLineConfig.XScaleSet.SecondaryScaleNum = json["XSecondaryScaleNum"].GetInt();
			if (json.HasMember("XSecondaryScaleLen"))
				ChartXYLineConfig.XScaleSet.SecondaryScaleLen = json["XSecondaryScaleLen"].GetInt();
			if (json.HasMember("XScaleMarkShowNumber"))
				ChartXYLineConfig.XScaleSet.MarkType = (Project::ScaleMarkType)json["XScaleMarkShowNumber"].GetInt();
			if (json.HasMember("XScaleMarkFont") && !json["XScaleMarkFont"].IsNull())
				ChartXYLineConfig.XScaleSet.ScaleMarkFont = json["XScaleMarkFont"].GetString();
			if (json.HasMember("XScaleMarkSize"))
				ChartXYLineConfig.XScaleSet.ScaleMarkSize = json["XScaleMarkSize"].GetInt();
			if (json.HasMember("XScaleMarkColor"))
				ChartXYLineConfig.XScaleSet.ScaleMarkColor = json["XScaleMarkColor"].GetInt();
			if (json.HasMember("XScaleMarkAct"))
				ChartXYLineConfig.XScaleSet.IntegerNum = json["XScaleMarkAct"].GetInt();
			if (json.HasMember("XScaleMarkBct"))
				ChartXYLineConfig.XScaleSet.DecimalnNum = json["XScaleMarkBct"].GetInt();

			if (json.HasMember("UseYScale"))
				ChartXYLineConfig.UseYScale = json["UseYScale"].GetBool();
			if (json.HasMember("YUpperLimit"))
				ChartXYLineConfig.YScaleSet.UpperLimit = json["YUpperLimit"].GetDouble();
			if (json.HasMember("YUpperLimitVar") && json["YUpperLimitVar"].IsObject())
				ChartXYLineConfig.YScaleSet.UpperLimitVar.Parse(json["YUpperLimitVar"]);
			if (json.HasMember("YLowerLimit"))
				ChartXYLineConfig.YScaleSet.LowerLimit = json["YLowerLimit"].GetDouble();
			if (json.HasMember("YLowerLimitVar") && json["YLowerLimitVar"].IsObject())
				ChartXYLineConfig.YScaleSet.LowerLimitVar.Parse(json["YLowerLimitVar"]);
			if (json.HasMember("YScaleColor"))
				ChartXYLineConfig.YScaleSet.ScaleColor = json["YScaleColor"].GetInt();
			if (json.HasMember("YPrimaryScaleNum"))
				ChartXYLineConfig.YScaleSet.PrimaryScaleNum = json["YPrimaryScaleNum"].GetInt();
			if (json.HasMember("YPrimaryScaleLen"))
				ChartXYLineConfig.YScaleSet.PrimaryScaleLen = json["YPrimaryScaleLen"].GetInt();
			if (json.HasMember("UseYSecondaryScaleNum"))
				ChartXYLineConfig.YScaleSet.UseSecondaryScaleNum = json["UseYSecondaryScaleNum"].GetBool();
			if (json.HasMember("YSecondaryScaleNum"))
				ChartXYLineConfig.YScaleSet.SecondaryScaleNum = json["YSecondaryScaleNum"].GetInt();
			if (json.HasMember("YSecondaryScaleLen"))
				ChartXYLineConfig.YScaleSet.SecondaryScaleLen = json["YSecondaryScaleLen"].GetInt();
			if (json.HasMember("YScaleMarkShowNumber"))
				ChartXYLineConfig.YScaleSet.MarkType = (Project::ScaleMarkType)json["YScaleMarkShowNumber"].GetInt();
			if (json.HasMember("YScaleMarkFont") && !json["YScaleMarkFont"].IsNull())
				ChartXYLineConfig.YScaleSet.ScaleMarkFont = json["YScaleMarkFont"].GetString();
			if (json.HasMember("YScaleMarkSize"))
				ChartXYLineConfig.YScaleSet.ScaleMarkSize = json["YScaleMarkSize"].GetInt();
			if (json.HasMember("YScaleMarkColor"))
				ChartXYLineConfig.YScaleSet.ScaleMarkColor = json["YScaleMarkColor"].GetInt();
			if (json.HasMember("YScaleMarkAct"))
				ChartXYLineConfig.YScaleSet.IntegerNum = json["YScaleMarkAct"].GetInt();
			if (json.HasMember("YScaleMarkBct"))
				ChartXYLineConfig.YScaleSet.DecimalnNum = json["YScaleMarkBct"].GetInt();
		}
	}
}