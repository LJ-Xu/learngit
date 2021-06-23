#include "ProjectPages.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
using namespace rapidjson;
namespace Project
{
	void ProjectPages::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else
		{
			//解析map<int, HMIWindow> screens;
			if (json.HasMember("Screens"))
			{
				if (json["Screens"].IsObject() && !json["Screens"].IsNull())
				{
					PageInfo Win;
					Value &screensTmp = json["Screens"];
					for (rapidjson::Value::MemberIterator iter = screensTmp.MemberBegin();iter != screensTmp.MemberEnd();iter++)
					{
						string name = (iter->name).GetString();
						Value& value = iter->value;
						if (value.IsObject() && !value.IsNull())
						{
							if (value.HasMember("Pos") && value["Pos"].IsObject() && !value["pos"].IsNull())
							{
								if (value["Pos"].HasMember("X"))
									Win.Pos.X = value["Pos"]["X"].GetInt();
								if (value["Pos"].HasMember("Y"))
									Win.Pos.Y = value["Pos"]["Y"].GetInt();
							}
							if (value.HasMember("Name") && !value["Name"].IsNull())
								Win.Name = value["Name"].GetString();
							if (value.HasMember("Id"))
								Win.Id = value["Id"].GetInt();
							if (value.HasMember("Size") && value["Size"].IsObject() && !value["Size"].IsNull())
							{
								if (value["Size"].HasMember("W"))
									Win.Sz.W = value["Size"]["W"].GetInt();
								if (value["Size"].HasMember("H"))
									Win.Sz.H = value["Size"]["H"].GetInt();
							}
							if (value.HasMember("Shapes"))
							{
								Value& array = value["Shapes"];
								if (array.IsArray()) {
									for (unsigned i = 0; i < array.Size(); i++) {
										Value& tmp = array[i];
										UnitInfo tmpVector;
										if (tmp.HasMember("CtrName") && !tmp["CtrName"].IsNull())
											tmpVector.CtrName = tmp["TmpVector"].GetString();
										if (tmp.HasMember("Data"))
										{
											if (tmp["Data"].IsArray())
											{
												for (unsigned j = 0; j < tmp["Data"].Size(); i++) {
													Value &dataTmp = tmp["Data"][j];
													tmpVector.Data.push_back(tmp["Data"][j].GetString()[0]);
												}
											}
										}
										Win.Shapes.push_back(tmpVector);
									}
								}
							}

						}
						Screens.insert(pair<int, PageInfo>((name[0]-'0'), Win));
					}

				}

			}
		}
	}

	void PageInfo::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else
		{
					
			if (json.HasMember("Pos") && json["Pos"].IsObject() && !json["Pos"].IsNull())
			{
				if (json["Pos"].HasMember("X"))
					Pos.X = json["Pos"]["X"].GetInt();
				if (json["Pos"].HasMember("Y"))
					Pos.Y = json["Pos"]["Y"].GetInt();
			}
			if (json.HasMember("WinColor"))
				WinColor = json["WinColor"].GetInt();
			if (json.HasMember("Name") && !json["Name"].IsNull())
				Name = json["Name"].GetString();
			if (json.HasMember("WinId"))
				Id = json["WinId"].GetInt();
			if (json.HasMember("Size") && json["Size"].IsObject() && !json["Size"].IsNull())
			{
				if (json["Size"].HasMember("W"))
					Sz.W = json["Size"]["W"].GetInt();
				if (json["Size"].HasMember("H"))
					Sz.H = json["Size"]["H"].GetInt();
			}
			if (json.HasMember("Tempelement") && json["Tempelement"].IsObject())
			{
				if (json["Tempelement"].HasMember("TopWinId"))
					Template.TopWinId = json["Tempelement"]["TopWinId"].GetInt();
				if (json["Tempelement"].HasMember("BottomWinId"))
					Template.BottomWinId = json["Tempelement"]["BottomWinId"].GetInt();
			}
			
			if (json.HasMember("pageConfig") && json["pageConfig"].IsObject())
			{
					Value &cfg = json["pageConfig"];
					if (cfg.HasMember("IsMonopoly"))
						PageCfg.IsMonopoly = cfg["IsMonopoly"].GetBool();
					if (cfg.HasMember("IsCloseBtn"))
						PageCfg.IsCloseBtn = cfg["IsCloseBtn"].GetBool();
					if (cfg.HasMember("WinBgMode"))
						PageCfg.WinBgMode = cfg["WinBgMode"].GetInt();
					if (cfg.HasMember("WinKey") && cfg["WinKey"].IsObject())
						PageCfg.WinKey.Parse(cfg["WinKey"]);
					if (cfg.HasMember("UserLimit"))
						PageCfg.UserLimit = cfg["UserLimit"].GetInt();
					if (cfg.HasMember("ChangeToLimit"))
						PageCfg.ChangeToLimit = cfg["ChangeToLimit"].GetInt();
					if (cfg.HasMember("IsChangeToLimit"))
						PageCfg.IsChangeToLimit = cfg["IsChangeToLimit"].GetBool();
			}
		}
	}
	void UnitInfo::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else
		{
			if (json.HasMember("CtrName") && !json["CtrName"].IsNull())
				CtrName = json["CtrName"].GetString();
		}
	}
 
}
