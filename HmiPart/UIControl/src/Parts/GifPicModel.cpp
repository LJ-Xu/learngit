/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GifPicModel.cpp
 * Author   : qiaodan
 * Date     : 2020-12-22
 * Descript : 解析json，关于GIF图片参数内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "GifPicModel.h"
#include <FL/Fl.H>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
using namespace rapidjson;
namespace UI
{
	void GifPicModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/json/gif.json", "r");
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
			LOG_INFO_("GifPicModel Parse Json\n");
			GifConfig.InitBaseData(json);

			if (json.HasMember("ResIdRefs") && json["ResIdRefs"].IsArray())
				Project::ImageResId::Parse(GifConfig.PicKeys, json["ResIdRefs"]);
			if (json.HasMember("DelayTime") && json["DelayTime"].IsArray())
			{
				for (size_t i = 0; i < json["DelayTime"].Size(); i++)
					GifConfig.DelayTime.push_back(json["DelayTime"][i].GetInt());
			}

			if (json.HasMember("ImageFmt") && json["ImageFmt"].IsObject())
			{
				Value& Picparam = json["ImageFmt"];
				if (Picparam.IsObject()) {
					if (Picparam.HasMember("ImgHashkey") && !Picparam["ImgHashkey"].IsNull())
						GifConfig.PicFormat.KeyVal = Picparam["ImgHashkey"].GetString();
					if (Picparam.HasMember("UseFaheout"))
						GifConfig.PicFormat.UseFaheout = Picparam["UseFaheout"].GetBool();
					if (Picparam.HasMember("FadeoutPecent"))
						GifConfig.PicFormat.FadeoutPecent = Picparam["FadeoutPecent"].GetInt();
					if (Picparam.HasMember("UseRotate"))
						GifConfig.PicFormat.UseRotate = Picparam["UseRotate"].GetBool();
					if (Picparam.HasMember("RotateAngle"))
						GifConfig.PicFormat.RotateAngle = Picparam["RotateAngle"].GetInt();
				}
			}
			if (json.HasMember("UseFlashing"))
				GifConfig.UseFlashing = json["UseFlashing"].GetBool();
			if (json.HasMember("Frequency"))
				GifConfig.Frequency = json["Frequency"].GetDouble();
		}
	}
}
