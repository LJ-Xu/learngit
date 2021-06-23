/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : SlideInputModel.cpp
 * Author   : qiaodan
 * Date     : 2020-12-17
 * Descript : 解析json，关于滑动输入参数内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "SlideInputModel.h"
#include <FL/Fl.H>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
using namespace rapidjson;
namespace UI
{
	void SlideInputModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/SlideInput.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
			return;
		}
		LOG_INFO_("SlideInputModel Parse Json\n");
		SlideInputConfig.InitBaseData(json);
		if (json.HasMember("ResBef") && json["ResBef"].IsObject())
			SlideInputConfig.ResBef.Parse(json["ResBef"]);
		if (json.HasMember("ResAft") && json["ResAft"].IsObject())
			SlideInputConfig.ResAft.Parse(json["ResAft"]);

		if (json.HasMember("ReadVar") && json["ReadVar"].IsObject())
			SlideInputConfig.ReadVar.Parse(json["ReadVar"]);
		if (json.HasMember("DataStyle"))
			SlideInputConfig.DataStyle = json["DataStyle"].GetInt();
		if (json.HasMember("AttrMax"))
			SlideInputConfig.AttrMax = json["AttrMax"].GetDouble();
		if (json.HasMember("AttrMaxVar") && json["AttrMaxVar"].IsObject())
			SlideInputConfig.AttrMaxVar.Parse(json["AttrMaxVar"]);
		if (json.HasMember("AttrMin"))
			SlideInputConfig.AttrMin = json["AttrMin"].GetDouble();
		if (json.HasMember("AttrMinVar") && json["AttrMinVar"].IsObject())
			SlideInputConfig.AttrMinVar.Parse(json["AttrMinVar"]);
		if (json.HasMember("Dir"))
			SlideInputConfig.Dir = (Project::SlideDir)json["Dir"].GetInt();
		if (json.HasMember("MinScale"))
			SlideInputConfig.MinScale = json["MinScale"].GetInt();
		if (json.HasMember("UseIncDecPerClick"))
			SlideInputConfig.UseIncDecPerClick = json["UseIncDecPerClick"].GetBool();
		if (json.HasMember("IncDecPerClickScale"))
			SlideInputConfig.IncDecPerClickScale = json["IncDecPerClickScale"].GetInt();
		if (json.HasMember("UseChangeInput"))
			SlideInputConfig.UseChangeInput = json["UseChangeInput"].GetBool();

		if (json.HasMember("SlideRailWidth"))
			SlideInputConfig.SlideRailWidth = json["SlideRailWidth"].GetInt();
		if (json.HasMember("SlideRailHeight"))
			SlideInputConfig.SlideRailHeight = json["SlideRailHeight"].GetInt();
		if (json.HasMember("SlideRailBackColor"))
			SlideInputConfig.SlideRailBackColor = json["SlideRailBackColor"].GetInt();
		if (json.HasMember("SlideRailBorderColor"))
			SlideInputConfig.SlideRailBorderColor = json["SlideRailBorderColor"].GetInt();
		if (json.HasMember("SlideRailStyle") && json["SlideRailStyle"].IsObject())
			SlideInputConfig.SlideRailKey.Parse(json["SlideRailStyle"]);

		
		if (json.HasMember("SlideWidth"))
			SlideInputConfig.SlideWidth = json["SlideWidth"].GetInt();
		if (json.HasMember("SlideHeight"))
			SlideInputConfig.SlideHeight = json["SlideHeight"].GetInt();
		if (json.HasMember("SlideBackColor"))
			SlideInputConfig.SlideBackColor = json["SlideBackColor"].GetInt();
		if (json.HasMember("SlideBorderColor"))
			SlideInputConfig.SlideBorderColor = json["SlideBorderColor"].GetInt();
		if (json.HasMember("SlideStyle") && json["SlideStyle"].IsObject())
			SlideInputConfig.SlideKey.Parse(json["SlideStyle"]);

		if (json.HasMember("ShowScale"))
			SlideInputConfig.ShowScale = json["ShowScale"].GetBool();
		if (json.HasMember("ScalePos"))
			SlideInputConfig.ScalePos = json["ScalePos"].GetInt();
		if (json.HasMember("PrimaryScaleNum"))
			SlideInputConfig.PrimaryScaleNum = json["PrimaryScaleNum"].GetInt();
		if (json.HasMember("PrimaryScaleLen"))
			SlideInputConfig.PrimaryScaleLen = json["PrimaryScaleLen"].GetInt();
		if (json.HasMember("SecondaryScaleNum"))
			SlideInputConfig.SecondaryScaleNum = json["SecondaryScaleNum"].GetInt();
		if (json.HasMember("SecondaryScaleLen"))
			SlideInputConfig.SecondaryScaleLen = json["SecondaryScaleLen"].GetInt();
		if (json.HasMember("ScaleLine") && json["ScaleLine"].IsObject())
			SlideInputConfig.ScaleLineStyle.Parse(json["ScaleLine"]);
		if (json.HasMember("DrawPosition") && json["DrawPosition"].IsObject())
		{
			if (json["DrawPosition"].HasMember("OriginPos") && json["DrawPosition"]["OriginPos"].IsObject())
			{
				if (json["DrawPosition"]["OriginPos"].HasMember("x"))
					SlideInputConfig.AxisPos.X = json["DrawPosition"]["OriginPos"]["x"].GetInt();
				if (json["DrawPosition"]["OriginPos"].HasMember("y"))
					SlideInputConfig.AxisPos.Y = json["DrawPosition"]["OriginPos"]["y"].GetInt();
			}
			if (json["DrawPosition"].HasMember("ScaleInfo") && json["DrawPosition"]["ScaleInfo"].IsArray())
				Project::ScaleInfo::Parse(SlideInputConfig.ScaleInfo, json["DrawPosition"]["ScaleInfo"]);
			if (json["DrawPosition"].HasMember("SlideRailStartPos") && json["DrawPosition"]["SlideRailStartPos"].IsObject())
			{
				if (json["DrawPosition"]["SlideRailStartPos"].HasMember("x"))
					SlideInputConfig.SlideRailPos.X = json["DrawPosition"]["SlideRailStartPos"]["x"].GetInt();
				if (json["DrawPosition"]["SlideRailStartPos"].HasMember("y"))
					SlideInputConfig.SlideRailPos.Y = json["DrawPosition"]["SlideRailStartPos"]["y"].GetInt();
			}
		}
		if (json.HasMember("ShowMark"))
			SlideInputConfig.ShowMark = json["ShowMark"].GetBool();
		if (json.HasMember("Act"))
			SlideInputConfig.Act = json["Act"].GetInt();
		if (json.HasMember("Bct"))
			SlideInputConfig.Bct = json["Bct"].GetInt();
		if (json.HasMember("MarkStyle") && json["MarkStyle"].IsObject())
			SlideInputConfig.MarkStyle.Parse(json["MarkStyle"]);

		if (json.HasMember("ShowPer"))
			SlideInputConfig.ShowPer = json["ShowPer"].GetBool();
		if (json.HasMember("PerStyle") && json["PerStyle"].IsObject())
			SlideInputConfig.PerStyle.Parse(json["PerStyle"]);

		if (json.HasMember("ShowAxis"))
			SlideInputConfig.ShowAxis = json["ShowAxis"].GetBool();
	}
}
