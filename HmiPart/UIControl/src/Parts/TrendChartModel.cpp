/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TrendChartModel.cpp
 * Author   : qiaodan
 * Date     : 2020-11-14
 * Descript : 解析json，关于趋势图参数内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "TrendChartModel.h"
#include <FL/Fl.H>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
using namespace rapidjson;
namespace UI
{
	void TrendChartModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/json/TrendChart.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
			return;
		}
		LOG_INFO_("TrendChartModel Parse Json\n");
		ChartTrendConfig.InitBaseData(json);
		if (json.HasMember("DataCapacity"))
			ChartTrendConfig.DataCapacity = json["DataCapacity"].GetInt();
		if (json.HasMember("PerSceenViewMode"))
			ChartTrendConfig.PerSceenViewMode = json["PerSceenViewMode"].GetInt();
		if (json.HasMember("PerScreendot"))
			ChartTrendConfig.PerScreendot = json["PerScreendot"].GetInt();
		if (json.HasMember("PerScreenPeriod"))
			ChartTrendConfig.PerScreenPeriod = json["PerScreenPeriod"].GetInt();
		if (json.HasMember("SimpleGroup"))
			ChartTrendConfig.SimpleGroup = json["SimpleGroup"].GetInt();
		if (json.HasMember("SimpleNo"))
			ChartTrendConfig.SimpleNo = json["SimpleNo"].GetInt();
		if (json.HasMember("ChannelViewLst") && json["ChannelViewLst"].IsArray())
			Project::TrendChannel::Parse(ChartTrendConfig.SetChannel,json["ChannelViewLst"], ChartTrendConfig.SimpleGroup, ChartTrendConfig.SimpleNo);
		if (json.HasMember("SampleVarId") && json["SampleVarId"].IsObject())
			ChartTrendConfig.SampleVarId.Parse(json["SampleVarId"]);

		if (json.HasMember("IsDataDisplay"))
			ChartTrendConfig.IsDataDisplay = json["IsDataDisplay"].GetBool();
		if (json.HasMember("InfoWinBgColor"))
			ChartTrendConfig.InfoBgColor = json["InfoWinBgColor"].GetInt();
		if (json.HasMember("FontColor"))
			ChartTrendConfig.InfoFontColor = json["FontColor"].GetInt();
		if (json.HasMember("DataLineColor"))
			ChartTrendConfig.InfoLineColor = json["DataLineColor"].GetInt();

		if (json.HasMember("BgColor"))
			ChartTrendConfig.BgColor = json["BgColor"].GetInt();
		if (json.HasMember("ScaleAreabgColor"))
			ChartTrendConfig.ScaleAreabgColor = json["ScaleAreabgColor"].GetInt();
		if (json.HasMember("IsGriddisplay"))
			ChartTrendConfig.IsGriddisplay = json["IsGriddisplay"].GetBool();
		if (json.HasMember("XUniformNum"))
			ChartTrendConfig.XUniformNum = json["XUniformNum"].GetInt();
		if (json.HasMember("YUniformNum"))
			ChartTrendConfig.YUniformNum = json["YUniformNum"].GetInt();
		if (json.HasMember("GridStyle") && json["GridStyle"].IsObject())
			ChartTrendConfig.GridStyle.Parse(json["GridStyle"]);


		if (json.HasMember("IsdisplayXscale"))
			ChartTrendConfig.AxisX.IsdisplayScale = json["IsdisplayXscale"].GetBool();
		if (json.HasMember("XScaleColor"))
			ChartTrendConfig.AxisX.ScaleColor = json["XScaleColor"].GetInt();
		if (json.HasMember("DateFormat"))
			ChartTrendConfig.AxisX.DateStyle = (Project::DateMode)json["DateFormat"].GetInt();
		if (json.HasMember("TimeFormat"))
			ChartTrendConfig.AxisX.TimeStyle = (Project::TimeMode)json["TimeFormat"].GetInt();
		if (json.HasMember("XMainScaleNum"))
			ChartTrendConfig.AxisX.MainScaleNum = json["XMainScaleNum"].GetInt();
		if (json.HasMember("XMainScaleLen"))
			ChartTrendConfig.AxisX.MainScaleLen = json["XMainScaleLen"].GetInt();
		if (json.HasMember("IsXViceScaleNum"))
			ChartTrendConfig.AxisX.IsViceScaleNum = json["IsXViceScaleNum"].GetBool();
		if (json.HasMember("XViceScaleNum"))
			ChartTrendConfig.AxisX.ViceScaleNum = json["XViceScaleNum"].GetInt();
		if (json.HasMember("XViceScaleLen"))
			ChartTrendConfig.AxisX.ViceScaleLen = json["XViceScaleLen"].GetInt();
		if (json.HasMember("IsUseTimeScale"))
			ChartTrendConfig.AxisX.IsUseTimeScale = json["IsUseTimeScale"].GetBool();
		if (json.HasMember("IsdiaplayDate"))
			ChartTrendConfig.AxisX.IsdiaplayDate = json["IsdiaplayDate"].GetBool();
		if (json.HasMember("IsdisplayTime"))
			ChartTrendConfig.AxisX.IsdisplayTime = json["IsdisplayTime"].GetBool();
		if (json.HasMember("XScaleStyle") && json["XScaleStyle"].IsObject())
			ChartTrendConfig.AxisX.ScaleStyle.Parse(json["XScaleStyle"]);

		if (json.HasMember("IsdisplayYscale"))
			ChartTrendConfig.AxisY.IsdisplayScale = json["IsdisplayYscale"].GetBool();
		if (json.HasMember("YScaleColor"))
			ChartTrendConfig.AxisY.ScaleColor = json["YScaleColor"].GetInt();
		if (json.HasMember("YMainScaleNum"))
			ChartTrendConfig.AxisY.MainScaleNum = json["YMainScaleNum"].GetInt();
		if (json.HasMember("YMainScaleLen"))
			ChartTrendConfig.AxisY.MainScaleLen = json["YMainScaleLen"].GetInt();
		if (json.HasMember("IsYViceScaleNum"))
			ChartTrendConfig.AxisY.IsViceScaleNum = json["IsYViceScaleNum"].GetBool();
		if (json.HasMember("YViceScaleNum"))
			ChartTrendConfig.AxisY.ViceScaleNum = json["YViceScaleNum"].GetInt();
		if (json.HasMember("YViceScaleLen"))
			ChartTrendConfig.AxisY.ViceScaleLen = json["YViceScaleLen"].GetInt();
		if (json.HasMember("ScaleTag"))
			ChartTrendConfig.AxisY.ScaleTag = (Project::YScaleTag)json["ScaleTag"].GetInt();
		if (json.HasMember("YIntegerNum"))
			ChartTrendConfig.AxisY.YIntegerNum = json["YIntegerNum"].GetInt();
		if (json.HasMember("YDecimalnNum"))
			ChartTrendConfig.AxisY.YDecimalnNum = json["YDecimalnNum"].GetInt();
		if (json.HasMember("ScaleRange"))
			ChartTrendConfig.AxisY.ScaleRange = (Project::YScaleMode)json["ScaleRange"].GetInt();
		if (json.HasMember("YScaleMax"))
			ChartTrendConfig.AxisY.YScaleMax = json["YScaleMax"].GetDouble();
		if (json.HasMember("YScalemaxVarId") && json["YScalemaxVarId"].IsObject())
			ChartTrendConfig.AxisY.YScalemaxVarId.Parse(json["YScalemaxVarId"]);
		if (json.HasMember("YScaleMin"))
			ChartTrendConfig.AxisY.YScaleMin = json["YScaleMin"].GetDouble();
		if (json.HasMember("YScaleminVarId") && json["YScaleminVarId"].IsObject())
			ChartTrendConfig.AxisY.YScaleminVarId.Parse(json["YScaleminVarId"]);
		if (json.HasMember("YScaleStyle") && json["YScaleStyle"].IsObject())
			ChartTrendConfig.AxisY.ScaleStyle.Parse(json["YScaleStyle"]);
		if (json.HasMember("UseChannelNo"))
			ChartTrendConfig.AxisY.UseRangeChanbelNo = json["UseChannelNo"].GetInt();

		if (json.HasMember("ZoomVarId") && json["ZoomVarId"].IsObject())
			ChartTrendConfig.ZoomVarId.Parse(json["ZoomVarId"]);
		if (json.HasMember("PageCtrlVarId") && json["PageCtrlVarId"].IsObject())
			ChartTrendConfig.PageCtrlVarId.Parse(json["PageCtrlVarId"]);

		if (json.HasMember("DrawPosition") && json["DrawPosition"].IsObject())
		{
			Value &value = json["DrawPosition"];
			if (value.HasMember("OriginPos") && value["OriginPos"].IsObject())
			{
				if (value["OriginPos"].HasMember("x"))
					ChartTrendConfig.OriginPos.X = value["OriginPos"]["x"].GetInt();
				if (value["OriginPos"].HasMember("y"))
					ChartTrendConfig.OriginPos.Y = value["OriginPos"]["y"].GetInt();
			}
			if (value.HasMember("XaxisLen"))
				ChartTrendConfig.XaxisLen = value["XaxisLen"].GetInt();
			if (value.HasMember("YaxisLen"))
				ChartTrendConfig.YaxisLen = value["YaxisLen"].GetInt();
			if (value.HasMember("XscaleInfo") && value["XscaleInfo"].IsArray())
			{
				std::vector<Project::ScaleInfo>().swap(ChartTrendConfig.XScaleInfo);
				for (unsigned i = 0; i < value["XscaleInfo"].Size(); i++)
				{
					Project::ScaleInfo tmpInfo;
					if (value["XscaleInfo"][i].HasMember("Coordinate") && value["XscaleInfo"][i]["Coordinate"].IsObject())
					{
						if (value["XscaleInfo"][i]["Coordinate"].HasMember("x"))
							tmpInfo.Coordinate.X = value["XscaleInfo"][i]["Coordinate"]["x"].GetInt();
						if (value["XscaleInfo"][i]["Coordinate"].HasMember("y"))
							tmpInfo.Coordinate.Y = value["XscaleInfo"][i]["Coordinate"]["y"].GetInt();
					}
					if (value["XscaleInfo"][i].HasMember("Width"))
						tmpInfo.Width = value["XscaleInfo"][i]["Width"].GetInt();
					if (value["XscaleInfo"][i].HasMember("Height"))
						tmpInfo.Height = value["XscaleInfo"][i]["Height"].GetInt();
					if (value["XscaleInfo"][i].HasMember("Scalecontent") && !value["XscaleInfo"][i]["Scalecontent"].IsNull())
						tmpInfo.ScaleContent = value["XscaleInfo"][i]["Scalecontent"].GetString();
					ChartTrendConfig.XScaleInfo.push_back(tmpInfo);
				}
			}
			if (value.HasMember("YscaleInfo") && value["YscaleInfo"].IsArray())
			{
				std::vector<vector<Project::ScaleInfo>>().swap(ChartTrendConfig.YScaleInfo);
				for (unsigned i = 0; i < value["YscaleInfo"].Size(); i++)
				{
					vector<Project::ScaleInfo> tmpVector;
					if (value["YscaleInfo"][i].IsArray())
					{
						Value &array = value["YscaleInfo"][i];
						for (unsigned j = 0; j < array.Size(); j++)
						{
							Project::ScaleInfo tmpInfo;
							if (array[j].HasMember("Coordinate") && array[j]["Coordinate"].IsObject())
							{
								if (array[j]["Coordinate"].HasMember("x"))
									tmpInfo.Coordinate.X = array[j]["Coordinate"]["x"].GetInt();
								if (array[j]["Coordinate"].HasMember("y"))
									tmpInfo.Coordinate.Y = array[j]["Coordinate"]["y"].GetInt();
							}
							if (array[j].HasMember("Width"))
								tmpInfo.Width = array[j]["Width"].GetInt();
							if (array[j].HasMember("Height"))
								tmpInfo.Height = array[j]["Height"].GetInt();
							if (array[j].HasMember("Scalecontent") && !array[j]["Scalecontent"].IsNull())
								tmpInfo.ScaleContent = array[j]["Scalecontent"].GetString();
							tmpVector.push_back(tmpInfo);
						}
						ChartTrendConfig.YScaleInfo.push_back(tmpVector);
					}
				}
				
			}
			if (value.HasMember("ScrollHeight"))
				ChartTrendConfig.ScrollHeight = value["ScrollHeight"].GetInt();
		}

		if (json.HasMember("ViewInfoLst") && json["ViewInfoLst"].IsArray())
		{
			std::vector<Project::DataDisplay>().swap(ChartTrendConfig.DisplayContent);
			for (unsigned i = 0; i < json["ViewInfoLst"].Size(); i++)
			{
				if (json["ViewInfoLst"][i].HasMember("IsSelectPro")
					&& json["ViewInfoLst"][i]["IsSelectPro"].GetBool())
				{
					Project::DataDisplay content;
					if (json["ViewInfoLst"][i].HasMember("ViewPro") && !json["ViewInfoLst"][i]["ViewPro"].IsNull())
						content.ProjectName = json["ViewInfoLst"][i]["ViewPro"].GetString();
					if (json["ViewInfoLst"][i].HasMember("ContentDescribe") && !json["ViewInfoLst"][i]["ContentDescribe"].IsNull())
						content.DescribeName = json["ViewInfoLst"][i]["ContentDescribe"].GetString();
					ChartTrendConfig.DisplayContent.push_back(content);
				}			
			}
		}
		if (json.HasMember("IsEnableSearch"))
			ChartTrendConfig.EnableSearch = json["IsEnableSearch"].GetBool();
		if (json.HasMember("SearchMode"))
			ChartTrendConfig.SearchMode = (Project::TrendSeekMode)json["SearchMode"].GetInt();
		if (json.HasMember("SearchModeVarId") && json["SearchModeVarId"].IsObject())
			ChartTrendConfig.SeekModeVar.Parse(json["SearchModeVarId"]);
		if (json.HasMember("SearchCtrlVarId") && json["SearchCtrlVarId"].IsObject())
			ChartTrendConfig.SeekCtrlVar.Parse(json["SearchCtrlVarId"]);

		if (json.HasMember("SearchDate"))
			ChartTrendConfig.SearchDate = json["SearchDate"].GetInt64() * 1000;
		if (json.HasMember("SearchDateVarId") && json["SearchDateVarId"].IsArray())
			Project::DataVarId::Parse(ChartTrendConfig.SearchDateVarId, json["SearchDateVarId"]);

		if (json.HasMember("SearchFromTimePeriod"))
			ChartTrendConfig.SearchTimeStart = json["SearchFromTimePeriod"].GetInt64() * 1000;
		if (json.HasMember("SearchToTimePeriod"))
			ChartTrendConfig.SearchTimeEnd = json["SearchToTimePeriod"].GetInt64() * 1000;
		if (json.HasMember("SearchTimePeriodVarId") && json["SearchTimePeriodVarId"].IsArray())
			Project::DataVarId::Parse(ChartTrendConfig.SearchTimeVar, json["SearchTimePeriodVarId"]);
		
		if (json.HasMember("SearchChannel"))
			ChartTrendConfig.SearchChannelNo = json["SearchChannel"].GetInt();
		if (json.HasMember("SearchChannelVarId") && json["SearchChannelVarId"].IsObject())
			ChartTrendConfig.SearchChannelNoVar.Parse(json["SearchChannelVarId"]);
		if (ChartTrendConfig.SearchMode == Project::TrendSeekMode::REG)
			ChartTrendConfig.RegMode = true;
		if (json.HasMember("SearchRegVarId") && json["SearchRegVarId"].IsArray())
			Project::DataVarId::Parse(ChartTrendConfig.SearchRegVarId, json["SearchRegVarId"]);

	}
}
