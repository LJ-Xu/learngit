#pragma once
#include "DisplayUnit.h"
#include "DataVarInfo.h"
#include "CerealCommon.hpp"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

namespace Project
{
	class MoveableDisplayUnit :
		public DisplayUnit
	{
	public:
		DataVarId VOffX;
		DataVarId VOffY;
		int OffX = 0;
		int	OffY = 0;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(VOffX, VOffY);
		}
		void InitBaseData(rapidjson::Document& json)
		{
			if (json.HasMember("X"))
				X = json["X"].GetInt();
			if (json.HasMember("Y"))
				Y = json["Y"].GetInt();
			if (json.HasMember("Width"))
				Width = json["Width"].GetInt();
			if (json.HasMember("Height"))
				Height = json["Height"].GetInt();
			if (json.HasMember("IsEnable"))
				IsEnable = json["IsEnable"].GetBool();
			if (json.HasMember("IsRecord"))
				IsRecord = json["IsRecord"].GetBool();
			if (json.HasMember("CtrlName") && !json["CtrlName"].IsNull())
				CtrlName = json["CtrlName"].GetString();
			if (json.HasMember("OffX") && json["OffX"].IsObject())
				VOffX.Parse(json["OffX"]);
			if (json.HasMember("OffY") && json["OffY"].IsObject())
				VOffY.Parse(json["OffY"]);
			if (json.HasMember("Perm") && json["Perm"].IsObject())
				Perm.Parse(json["Perm"]);
		}
/*ºóÐø¿ÉÉ¾³ý*/
		void InitOffXData(rapidjson::Value& OffX)
		{
			if (OffX.IsObject())
			{
				VOffX.Parse(OffX);
				
			}
		}
			
		void InitOffYData(rapidjson::Value& OffY)
		{
			if (OffY.IsObject())
			{
				VOffY.Parse(OffY);
			}
		}

	};

}

