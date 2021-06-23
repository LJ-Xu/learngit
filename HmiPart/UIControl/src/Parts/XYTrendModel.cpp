/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : XYTrendModel.cpp
 * Author   : qiaodan
 * Date     : 2021-01-29
 * Descript : 解析json，关于XY趋势图 参数内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "XYTrendModel.h"
#include <FL/Fl.H>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "XYLineChartViewUnit.h"
using namespace rapidjson;
namespace UI
{
	void XYTrendModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/XYTrend.json", "r");
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
			LOG_INFO_("XYTrendModel Parse Json\n");
			TrendXYConfig.InitBaseData(json);

			if (json.HasMember("TrendFreshUnit") && json["TrendFreshUnit"].IsObject())
			{
				if(json["TrendFreshUnit"].HasMember("SampleType"))
					TrendXYConfig.RefreshMode = json["TrendFreshUnit"]["SampleType"].GetInt();
				if (json["TrendFreshUnit"].HasMember("SampleCycle") && json["TrendFreshUnit"]["SampleCycle"].IsObject())
				{
					if (json["TrendFreshUnit"]["SampleCycle"].HasMember("CycleTime"))
						TrendXYConfig.SamplingCycle = json["TrendFreshUnit"]["SampleCycle"]["CycleTime"].GetInt();

					if (json["TrendFreshUnit"]["SampleCycle"].HasMember("CycleUnit"))
					{
						int timeunit = json["TrendFreshUnit"]["SampleCycle"]["CycleUnit"].GetInt();
						if (timeunit == 1) //ms
							TrendXYConfig.SamplingCycle *= 1000;
						else if (timeunit == 2) //1s
							TrendXYConfig.SamplingCycle *= 60000;
					}
				}
				if (json["TrendFreshUnit"].HasMember("SampleTrigger") && json["TrendFreshUnit"]["SampleTrigger"].IsObject())
				{
					if (json["TrendFreshUnit"]["SampleTrigger"].HasMember("TriggerVarType"))
						TrendXYConfig.TriggerType = json["TrendFreshUnit"]["SampleTrigger"]["TriggerVarType"].GetInt();
					if (json["TrendFreshUnit"]["SampleTrigger"].HasMember("TriggerCond") && json["TrendFreshUnit"]["SampleTrigger"]["TriggerCond"].IsObject())
					{
						Value &cond = json["TrendFreshUnit"]["SampleTrigger"]["TriggerCond"];
						if (cond.HasMember("TriggerVarIdRef") && cond["TriggerVarIdRef"].IsObject())
							TrendXYConfig.TriggerTypeVar.Parse(cond["TriggerVarIdRef"]);
						if (cond.HasMember("BitCondition"))
							TrendXYConfig.ConditionBit = cond["BitCondition"].GetInt();
						if (cond.HasMember("WordCondition"))
							TrendXYConfig.ConditionWord = (Project::Conditions)cond["WordCondition"].GetInt();
						if (cond.HasMember("MovValueWord") && cond["MovValueWord"].IsObject())
						{
							if (cond["MovValueWord"].HasMember("Num"))
								TrendXYConfig.TriggerData = cond["MovValueWord"]["Num"].GetFloat();
							if (cond["MovValueWord"].HasMember("VarIdRef") && cond["MovValueWord"]["VarIdRef"].IsObject())
								TrendXYConfig.TriggerDataVar.Parse(cond["MovValueWord"]["VarIdRef"]);
						}
					}
				}
			}
			
			if (json.HasMember("TrendZoomUnit") && json["TrendZoomUnit"].IsObject())
			{
				if(json["TrendZoomUnit"].HasMember("IsZoomed"))
					TrendXYConfig.UseZoom = json["TrendZoomUnit"]["IsZoomed"].GetBool();
				if (json["TrendZoomUnit"].HasMember("ZoomValue"))
					TrendXYConfig.ZoomPercent = json["TrendZoomUnit"]["ZoomValue"].GetInt();
			}
			if (json.HasMember("TrendPointsUnit") && json["TrendPointsUnit"].IsObject())
			{
				if(json["TrendPointsUnit"].HasMember("SamplePointNum"))
					TrendXYConfig.SamplePoints = json["TrendPointsUnit"]["SamplePointNum"].GetInt();
				if (json["TrendPointsUnit"].HasMember("PointsVarIdRef") && json["TrendPointsUnit"]["PointsVarIdRef"].IsObject())
					TrendXYConfig.SamplePointsVar.Parse(json["TrendPointsUnit"]["PointsVarIdRef"]);
				if (json["TrendPointsUnit"].HasMember("PointsPerScreen"))
					TrendXYConfig.PointsPerScreen = json["TrendPointsUnit"]["PointsPerScreen"].GetInt();
				if (json["TrendPointsUnit"].HasMember("PageTurnedMode"))
					TrendXYConfig.PageControlStyle = json["TrendPointsUnit"]["PageTurnedMode"].GetInt();
				if (json["TrendPointsUnit"].HasMember("PageTurnedVarIdRef") && json["TrendPointsUnit"]["PageTurnedVarIdRef"].IsObject())
					TrendXYConfig.TurnControlVar.Parse(json["TrendPointsUnit"]["PageTurnedVarIdRef"]);
				if (json["TrendPointsUnit"].HasMember("SampleFunc"))
					TrendXYConfig.HandleSampleMode = (Project::FullSampleAct)json["TrendPointsUnit"]["SampleFunc"].GetInt();

			}
			if (json.HasMember("TrendRangeUnit") && json["TrendRangeUnit"].IsObject())
			{
				if (json["TrendRangeUnit"].HasMember("NovUpperX") && json["TrendRangeUnit"]["NovUpperX"].IsObject())
				{
					if (json["TrendRangeUnit"]["NovUpperX"].HasMember("Num"))
						TrendXYConfig.XScaleSet.UpperLimit = json["TrendRangeUnit"]["NovUpperX"]["Num"].GetFloat();
					if (json["TrendRangeUnit"]["NovUpperX"].HasMember("VarIdRef"))
						TrendXYConfig.XScaleSet.UpperLimitVar.Parse(json["TrendRangeUnit"]["NovUpperX"]["VarIdRef"]);
				}
				if (json["TrendRangeUnit"].HasMember("NovLowerX") && json["TrendRangeUnit"]["NovLowerX"].IsObject())
				{
					if (json["TrendRangeUnit"]["NovLowerX"].HasMember("Num"))
						TrendXYConfig.XScaleSet.LowerLimit = json["TrendRangeUnit"]["NovLowerX"]["Num"].GetFloat();
					if (json["TrendRangeUnit"]["NovLowerX"].HasMember("VarIdRef"))
						TrendXYConfig.XScaleSet.LowerLimitVar.Parse(json["TrendRangeUnit"]["NovLowerX"]["VarIdRef"]);
				}
				if (json["TrendRangeUnit"].HasMember("NovUpperY") && json["TrendRangeUnit"]["NovUpperY"].IsObject())
				{
					if (json["TrendRangeUnit"]["NovUpperY"].HasMember("Num"))
						TrendXYConfig.YScaleSet.UpperLimit = json["TrendRangeUnit"]["NovUpperY"]["Num"].GetFloat();
					if (json["TrendRangeUnit"]["NovUpperY"].HasMember("VarIdRef"))
						TrendXYConfig.YScaleSet.UpperLimitVar.Parse(json["TrendRangeUnit"]["NovUpperY"]["VarIdRef"]);
				}
				if (json["TrendRangeUnit"].HasMember("NovLowerY") && json["TrendRangeUnit"]["NovLowerY"].IsObject())
				{
					if (json["TrendRangeUnit"]["NovLowerY"].HasMember("Num"))
						TrendXYConfig.YScaleSet.LowerLimit = json["TrendRangeUnit"]["NovLowerY"]["Num"].GetFloat();
					if (json["TrendRangeUnit"]["NovLowerY"].HasMember("VarIdRef"))
						TrendXYConfig.YScaleSet.LowerLimitVar.Parse(json["TrendRangeUnit"]["NovLowerY"]["VarIdRef"]);
				}
			}
			if (json.HasMember("ChannleNum"))
				TrendXYConfig.ChannelNum = json["ChannleNum"].GetInt();
			
			if (json.HasMember("TrendChannels") && json["TrendChannels"].IsArray())
			{
				vector<Project::XYLineChannel>().swap(TrendXYConfig.DataChanel);
				for (size_t i = 0; i < json["TrendChannels"].Size(); i++)
				{
					Value &channelinfo = json["TrendChannels"][i];
					Project::XYLineChannel tmpinfo;

					if (channelinfo.HasMember("ChannelId"))
						tmpinfo.Index = channelinfo["ChannelId"].GetInt();
					if (channelinfo.HasMember("XAsixVarIdRef") && channelinfo["XAsixVarIdRef"].IsObject())
						tmpinfo.StartXAddr.Parse(channelinfo["XAsixVarIdRef"]);
					if (channelinfo.HasMember("YAsixVarIdRef") && channelinfo["YAsixVarIdRef"].IsObject())
						tmpinfo.StartYAddr.Parse(channelinfo["YAsixVarIdRef"]);
					if (channelinfo.HasMember("CurveStyle"))
						tmpinfo.CurveStyle = (Project::XYLineMode)channelinfo["CurveStyle"].GetInt();
					if (channelinfo.HasMember("LineStyle") && channelinfo["LineStyle"].IsObject())
					{
						if (channelinfo["LineStyle"].HasMember("Colors"))
							tmpinfo.LineColor = channelinfo["LineStyle"]["Colors"].GetInt();
						if (channelinfo["LineStyle"].HasMember("Size"))
							tmpinfo.LineSize = channelinfo["LineStyle"]["Size"].GetInt();
						if (channelinfo["LineStyle"].HasMember("Style"))
							tmpinfo.LineStyle = channelinfo["LineStyle"]["Style"].GetInt();
					}
					if (channelinfo.HasMember("DotStyle") && channelinfo["DotStyle"].IsObject())
					{
						if (channelinfo["DotStyle"].HasMember("Color"))
							tmpinfo.DrawDotColor = channelinfo["DotStyle"]["Color"].GetInt();
						if (channelinfo["DotStyle"].HasMember("Size"))
							tmpinfo.DrawDotSize = channelinfo["DotStyle"]["Size"].GetInt();
						if (channelinfo["DotStyle"].HasMember("Style"))
							tmpinfo.DrawDotStyle = (Project::DotStyle)channelinfo["DotStyle"]["Style"].GetInt();
					}
					if (channelinfo.HasMember("SampleVarIdRef") && channelinfo["SampleVarIdRef"].IsObject())
						tmpinfo.SampleControlVarId.Parse(channelinfo["SampleVarIdRef"]);
					if (channelinfo.HasMember("SampleCondition"))
						tmpinfo.ControlCond = channelinfo["SampleCondition"].GetInt();
					TrendXYConfig.DataChanel.push_back(tmpinfo);
				}
			}
				//Project::XYLineChannel::Parse(TrendXYConfig.DataChanel, json["TrendChannels"]);

			if (json.HasMember("ReferenceNum"))
				TrendXYConfig.ReferenceLineNum = json["ReferenceNum"].GetInt();
			if (json.HasMember("TrendReferences") && json["TrendReferences"].IsArray())
			{
				vector<Project::ReferencePolyLines>().swap(TrendXYConfig.ReferenceLines);
				for (unsigned i = 0; i < json["TrendReferences"].Size(); i++)
				{
					Project::ReferencePolyLines tmpline;
					if (json["TrendReferences"][i].HasMember("LineColor"))
						tmpline.LineColor = json["TrendReferences"][i]["LineColor"].GetInt();
					if (json["TrendReferences"][i].HasMember("LineMode"))
						tmpline.LineMode = json["TrendReferences"][i]["LineMode"].GetInt();
					if (json["TrendReferences"][i].HasMember("Pts") &&
						json["TrendReferences"][i]["Pts"].IsArray())
					{
						for (size_t j = 0; j < json["TrendReferences"][i]["Pts"].Size(); j++)
						{
							if (json["TrendReferences"][i]["Pts"][j].IsObject())
							{
								Project::Point point;
								if (json["TrendReferences"][i]["Pts"][j].HasMember("X"))
									point.X = (int)json["TrendReferences"][i]["Pts"][j]["X"].GetDouble();
								if (json["TrendReferences"][i]["Pts"][j].HasMember("Y"))
									point.Y = (int)json["TrendReferences"][i]["Pts"][j]["Y"].GetDouble();
								tmpline.Pts.push_back(point);
							}
						}
					}
					TrendXYConfig.ReferenceLines.push_back(tmpline);
				}
			}
			
			if (json.HasMember("ChartRegionColor"))
				TrendXYConfig.LineChartBackColor = json["ChartRegionColor"].GetInt();
			if (json.HasMember("ScaleRegionColor"))
				TrendXYConfig.ScaleAreaBackColor = json["ScaleRegionColor"].GetInt();
			if (json.HasMember("IsGridUsed"))
				TrendXYConfig.UseGrid = json["IsGridUsed"].GetBool();
			if (json.HasMember("XAsixGridNum"))
				TrendXYConfig.XGridEqualNum = json["XAsixGridNum"].GetInt();
			if (json.HasMember("YAsixGridNum"))
				TrendXYConfig.YGridEqualNum = json["YAsixGridNum"].GetInt();
			if (json.HasMember("GridStyle"))
				TrendXYConfig.GridStyle.Parse(json["GridStyle"]);
			if (json.HasMember("XYDrawPosition") && json["XYDrawPosition"].IsObject())
			{
				Value &value = json["XYDrawPosition"];
				if (value.HasMember("OriginPos") && value["OriginPos"].IsObject())
				{
					if (value["OriginPos"].HasMember("x"))
						TrendXYConfig.OriginPos.X = value["OriginPos"]["x"].GetInt();
					if (value["OriginPos"].HasMember("y"))
						TrendXYConfig.OriginPos.Y = value["OriginPos"]["y"].GetInt();
				}
				if (value.HasMember("XaxisLen"))
					TrendXYConfig.XaxisLen = value["XaxisLen"].GetInt();
				if (value.HasMember("YaxisLen"))
					TrendXYConfig.YaxisLen = value["YaxisLen"].GetInt();
				if (value.HasMember("XscaleInfo") && value["XscaleInfo"].IsArray())
					Project::ScaleInfo::Parse(TrendXYConfig.XScaleInfo, value["XscaleInfo"]);
				if (value.HasMember("YscaleInfo") && value["YscaleInfo"].IsArray())
					Project::ScaleInfo::Parse(TrendXYConfig.YScaleInfo, value["YscaleInfo"]);
				if (value.HasMember("ScrollHeight"))
					TrendXYConfig.ScrollHeight = value["ScrollHeight"].GetInt();
			}
			if (json.HasMember("XAsixScale") && json["XAsixScale"].IsObject())
			{
				Value &xscale = json["XAsixScale"];
				if (xscale.HasMember("ScaleUsed"))
					TrendXYConfig.UseXScale = xscale["ScaleUsed"].GetBool();
				if (xscale.HasMember("ScaleStyle") && xscale["ScaleStyle"].IsObject())
				{
					if (xscale["ScaleStyle"].HasMember("ScaleColor"))
						TrendXYConfig.XScaleSet.ScaleColor = xscale["ScaleStyle"]["ScaleColor"].GetInt();
					if (xscale["ScaleStyle"].HasMember("MainScaleNum"))
						TrendXYConfig.XScaleSet.PrimaryScaleNum = xscale["ScaleStyle"]["MainScaleNum"].GetInt();
					if (xscale["ScaleStyle"].HasMember("MainScaleLen"))
						TrendXYConfig.XScaleSet.PrimaryScaleLen = xscale["ScaleStyle"]["MainScaleLen"].GetInt();
					if (xscale["ScaleStyle"].HasMember("SubScaleUsed"))
						TrendXYConfig.XScaleSet.UseSecondaryScaleNum = xscale["ScaleStyle"]["SubScaleUsed"].GetBool();
					if (xscale["ScaleStyle"].HasMember("SubScaleNum"))
						TrendXYConfig.XScaleSet.SecondaryScaleNum = xscale["ScaleStyle"]["SubScaleNum"].GetInt();
					if (xscale["ScaleStyle"].HasMember("SubScaleLen"))
						TrendXYConfig.XScaleSet.SecondaryScaleLen = xscale["ScaleStyle"]["SubScaleLen"].GetInt();
				}
				if (xscale.HasMember("MarkUsed"))
					TrendXYConfig.XScaleSet.UseScaleMark = xscale["MarkUsed"].GetBool();
				if (xscale.HasMember("MarkStyle") && xscale["MarkStyle"].IsObject())
				{
					if(xscale["MarkStyle"].HasMember("MarkType"))
						TrendXYConfig.XScaleSet.NumScaleType = xscale["MarkStyle"]["MarkType"].GetInt();
					if (xscale["MarkStyle"].HasMember("MarkCss") && xscale["MarkStyle"]["MarkCss"].IsObject())
					{
						if (xscale["MarkStyle"]["MarkCss"].HasMember("Fonts") && xscale["MarkStyle"]["MarkCss"]["Fonts"].IsObject())
						{
							if (xscale["MarkStyle"]["MarkCss"]["Fonts"].HasMember("Name") 
								&& !xscale["MarkStyle"]["MarkCss"]["Fonts"]["Name"].IsNull())
								TrendXYConfig.XScaleSet.ScaleMarkFont = xscale["MarkStyle"]["MarkCss"]["Fonts"]["Name"].GetString();
							if (xscale["MarkStyle"]["MarkCss"]["Fonts"].HasMember("Size"))
								TrendXYConfig.XScaleSet.ScaleMarkSize = (int)xscale["MarkStyle"]["MarkCss"]["Fonts"]["Size"].GetDouble();

						}							
						if (xscale["MarkStyle"]["MarkCss"].HasMember("Colors"))
							TrendXYConfig.XScaleSet.ScaleMarkColor = xscale["MarkStyle"]["MarkCss"]["Colors"].GetInt();
					}
					if (xscale["MarkStyle"].HasMember("IntegerNum"))
						TrendXYConfig.XScaleSet.IntegerNum = xscale["MarkStyle"]["IntegerNum"].GetInt();
					if (xscale["MarkStyle"].HasMember("DecimalNum"))
						TrendXYConfig.XScaleSet.DecimalnNum = xscale["MarkStyle"]["DecimalNum"].GetInt();
				}
			}
			if (json.HasMember("YAsixScale") && json["YAsixScale"].IsObject())
			{
				Value &yscale = json["YAsixScale"];
				if (yscale.HasMember("ScaleUsed"))
					TrendXYConfig.UseYScale = yscale["ScaleUsed"].GetBool();
				if (yscale.HasMember("ScaleStyle") && yscale["ScaleStyle"].IsObject())
				{
					if (yscale["ScaleStyle"].HasMember("ScaleColor"))
						TrendXYConfig.YScaleSet.ScaleColor = yscale["ScaleStyle"]["ScaleColor"].GetInt();
					if (yscale["ScaleStyle"].HasMember("MainScaleNum"))
						TrendXYConfig.YScaleSet.PrimaryScaleNum = yscale["ScaleStyle"]["MainScaleNum"].GetInt();
					if (yscale["ScaleStyle"].HasMember("MainScaleLen"))
						TrendXYConfig.YScaleSet.PrimaryScaleLen = yscale["ScaleStyle"]["MainScaleLen"].GetInt();
					if (yscale["ScaleStyle"].HasMember("SubScaleUsed"))
						TrendXYConfig.YScaleSet.UseSecondaryScaleNum = yscale["ScaleStyle"]["SubScaleUsed"].GetBool();
					if (yscale["ScaleStyle"].HasMember("SubScaleNum"))
						TrendXYConfig.YScaleSet.SecondaryScaleNum = yscale["ScaleStyle"]["SubScaleNum"].GetInt();
					if (yscale["ScaleStyle"].HasMember("SubScaleLen"))
						TrendXYConfig.YScaleSet.SecondaryScaleLen = yscale["ScaleStyle"]["SubScaleLen"].GetInt();
				}
				if (yscale.HasMember("MarkUsed"))
					TrendXYConfig.YScaleSet.UseScaleMark = yscale["MarkUsed"].GetBool();
				if (yscale.HasMember("MarkStyle") && yscale["MarkStyle"].IsObject())
				{
					if(yscale["MarkStyle"].HasMember("MarkType"))
						TrendXYConfig.YScaleSet.NumScaleType = yscale["MarkStyle"]["MarkType"].GetInt();
					if (yscale["MarkStyle"].HasMember("MarkCss") && yscale["MarkStyle"]["MarkCss"].IsObject())
					{
						if (yscale["MarkStyle"]["MarkCss"].HasMember("Fonts") && yscale["MarkStyle"]["MarkCss"]["Fonts"].IsObject())
						{
							if (yscale["MarkStyle"]["MarkCss"]["Fonts"].HasMember("Name") 
								&& !yscale["MarkStyle"]["MarkCss"]["Fonts"]["Name"].IsNull())
								TrendXYConfig.YScaleSet.ScaleMarkFont = yscale["MarkStyle"]["MarkCss"]["Fonts"]["Name"].GetString();
							if (yscale["MarkStyle"]["MarkCss"]["Fonts"].HasMember("Size"))
								TrendXYConfig.YScaleSet.ScaleMarkSize = (int)yscale["MarkStyle"]["MarkCss"]["Fonts"]["Size"].GetDouble();

						}							
						if (yscale["MarkStyle"]["MarkCss"].HasMember("Colors"))
							TrendXYConfig.YScaleSet.ScaleMarkColor = yscale["MarkStyle"]["MarkCss"]["Colors"].GetInt();
					}
					if (yscale["MarkStyle"].HasMember("IntegerNum"))
						TrendXYConfig.YScaleSet.IntegerNum = yscale["MarkStyle"]["IntegerNum"].GetInt();
					if (yscale["MarkStyle"].HasMember("DecimalNum"))
						TrendXYConfig.YScaleSet.DecimalnNum = yscale["MarkStyle"]["DecimalNum"].GetInt();
				}
			}
		}
	}
}