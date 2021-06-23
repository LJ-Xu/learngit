#pragma once
#include "ProjectDataTables.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

using namespace std;
using namespace rapidjson;
namespace Project
{
	RecipeDT* RecipeDT::ins_;
	void RecipeDT::SetRecipeDTIns(RecipeDT* ins)
	{
		ins_ = ins;
	}
	RecipeDT* RecipeDT::Ins()
	{
		if(ins_)
			return ins_;
		else
		{
			static RecipeDT ins;
			return &ins;
		}
	}
	void RecipeDT::GetDataTypes(string groupname, vector<ColDataTypeInfo>& info)
	{
		vector<ColDataTypeInfo>().swap(info);
		map<string, RecipeTable>::iterator  iter;
		iter = RecipeTbVct.find(groupname);
		if (iter != RecipeTbVct.end())			//存在数据
		{
			for (size_t i = 0; i < iter->second.RecipeItem.size(); i++)
			{
				if (iter->second.RecipeItem[i].ColTitle != "名称")
				{
					ColDataTypeInfo tmpinfo;
					tmpinfo.ColTitle = iter->second.RecipeItem[i].ColTitle;
					tmpinfo.DataFmt = iter->second.RecipeItem[i].DataFmt;
					tmpinfo.DataType = iter->second.RecipeItem[i].DataType;
					tmpinfo.RegCount = iter->second.RecipeItem[i].RegCount;
					info.push_back(tmpinfo);
				}

			}
		}
	}

	DataVarId RecipeDT::GetIndexVar(string groupname)
	{
		map<string, RecipeTable>::iterator  iter;
		iter = RecipeTbVct.find(groupname);
		if (iter != RecipeTbVct.end())			//存在数据
			return iter->second.IndexVar;
		else 
			return DataVarId::NullId;
	}

	RecipeInfoRes* RecipeDT::GetDataType(string groupname, string recipename)
	{
		map<string, RecipeTable>::iterator  iter;
		iter = RecipeTbVct.find(groupname);
		if (iter != RecipeTbVct.end())			//存在数据
		{
			for (size_t i = 0; i < iter->second.RecipeItem.size(); i++)
			{
				if (iter->second.RecipeItem[i].ColTitle == recipename)
				{
					//RecipeInfoRes *res = &iter->second.RecipeItem[i];
					return  &iter->second.RecipeItem[i];
				}
			}
		}
		return NULL;
	}

	int RecipeDT::GetColNum(string groupname, string recipename)
	{
		map<string, RecipeTable>::iterator  iter;
		iter = RecipeTbVct.find(groupname);
		if (iter != RecipeTbVct.end())			//存在数据
		{
			for (size_t i = 0; i < iter->second.RecipeItem.size(); i++)
			{
				if (iter->second.RecipeItem[i].ColTitle == recipename)
					return  i;
			}
		}
		return -1;
	}
	int RecipeDT::GetColNum(string groupname)
	{
		map<string, RecipeTable>::iterator  iter;
		iter = RecipeTbVct.find(groupname);
		if (iter != RecipeTbVct.end())			//存在数据
			return iter->second.RecipeItem.size();
		return 0;
	}
#if 0
	bool RecipeDT::InsertRecipeRecord(string groupname,unsigned int no, string datastr)
	{
		map<string, RecipeTable>::iterator  iter;
		iter = RecipeTbVct.find(groupname);
		if (iter != RecipeTbVct.end())			//存在数据
		{
			if (iter->second.RecipeItem.size() == no)
				return false;
			else
			{
				for (size_t i = 0; i < iter->second.RecipeItem.size(); i++)
				{
					if (iter->second.RecipeItem[i].RecipeData.size() >= no)
					{
						if (iter->second.RecipeItem[i].ColTitle == "名称")
							iter->second.RecipeItem[i].RecipeData.insert(iter->second.RecipeItem[i].RecipeData.begin() + no, " ");
						else
						{
							//DDWORD data = 0;
							//XJDataType tmptp;
							//tmptp.Cls = iter->second.RecipeItem[i].DataType;
							//tmptp.Type = iter->second.RecipeItem[i].DataFmt;
							//string datastr = Utility::DataFormate::NumFmt(data, tmptp, iter->second.RecipeItem[i].Fmt);
							iter->second.RecipeItem[i].RecipeData.insert(iter->second.RecipeItem[i].RecipeData.begin() + no, datastr);
						}
					}

				}
			}
		}
		return false;
	}
	bool RecipeDT::DeleteRecipeRecord(string groupname, unsigned int no)
	{
		map<string, RecipeTable>::iterator  iter;
		iter = RecipeTbVct.find(groupname);
		if (iter != RecipeTbVct.end())			//存在数据
		{
			if (iter->second.RecipeItem.size() == 0)
				return false;
			else
			{
				for (size_t i = 0; i < iter->second.RecipeItem.size(); i++)
				{
					if (iter->second.RecipeItem[i].RecipeData.size() > no)
					{
						iter->second.RecipeItem[i].RecipeData.erase((iter->second.RecipeItem[i].RecipeData.begin() + no));
					}
				}
				return true;
			}
		}
		return false;
	}
	bool RecipeDT::AddRecipeRecord(string groupname,string datastr)
	{
		map<string, RecipeTable>::iterator  iter;
		iter = RecipeTbVct.find(groupname);
		if (iter != RecipeTbVct.end())			//存在数据
		{
			if (iter->second.RecipeItem.size() == 0)
				return false;
			else
			{
				for (size_t i = 0; i < iter->second.RecipeItem.size(); i++)
				{
					if (iter->second.RecipeItem[i].ColTitle == "名称")
						iter->second.RecipeItem[i].RecipeData.push_back(" ");
					else
					{
						//DDWORD data = 0;
						//XJDataType tmptp;
						//tmptp.Cls = iter->second.RecipeItem[i].DataType;
						//tmptp.Type = iter->second.RecipeItem[i].DataFmt;
						//string datastr = Utility::DataFormate::NumFmt(data, tmptp, iter->second.RecipeItem[i].Fmt);
						iter->second.RecipeItem[i].RecipeData.push_back(datastr);
					}
				}
				return true;
			}
		}
		return false;
	}
	string RecipeDT::GetRecipeData(string groupname, string recipename, unsigned int rowno)
	{
		string data = "";
		map<string, RecipeTable>::iterator  iter;
		iter = RecipeTbVct.find(groupname);
		if (iter != RecipeTbVct.end())			//存在数据
		{
			for (size_t i = 0; i < iter->second.RecipeItem.size(); i++)
			{
				if (iter->second.RecipeItem[i].ColTitle == recipename)
				{
					if (iter->second.RecipeItem[i].RecipeData.size() > rowno)
					{
						data = iter->second.RecipeItem[i].RecipeData[rowno];
						return data;
					}
				}
			}
		}
		return data;
	}

	bool RecipeDT::SetRecipeData(string groupname, string recipename, unsigned int rowno, string data)
	{
		map<string, RecipeTable>::iterator  iter;
		iter = RecipeTbVct.find(groupname);
		if (iter != RecipeTbVct.end())			//存在数据
		{
			for (size_t i = 0; i < iter->second.RecipeItem.size(); i++)
			{
				if (iter->second.RecipeItem[i].ColTitle == recipename)
				{
					if (iter->second.RecipeItem[i].RecipeData.size() > rowno)
					{
						if (iter->second.RecipeItem[i].ColTitle == "名称")
							iter->second.RecipeItem[i].RecipeData[rowno] = data;
						else
						{
							//XJDataType tmptp;
							//tmptp.Cls = iter->second.RecipeItem[i].DataType;
							//tmptp.Type = iter->second.RecipeItem[i].DataFmt;
							//string datastr = Utility::DataFormate::StrFmt(data, tmptp, iter->second.RecipeItem[i].Fmt);
							iter->second.RecipeItem[i].RecipeData[rowno] = data;
						}
						return true;
					}
				}
			}
		}
		return false;
	}
	bool RecipeDT::SearchRecipeData(string groupname, vector<int>& rownos, string data, int type)
	{
		map<string, RecipeTable>::iterator  iter;
		iter = RecipeTbVct.find(groupname);
		if (iter != RecipeTbVct.end())			//存在数据
		{
			for (size_t i = 0; i < iter->second.RecipeItem.size(); i++)
			{
				if (type)
				{
					if (iter->second.RecipeItem[i].ColTitle != "名称")
					{
						for (size_t j = 0; j < iter->second.RecipeItem[i].RecipeData.size(); j++)
						{
							if (iter->second.RecipeItem[i].RecipeData[j] == data)
							{
								vector <int>::iterator tmpint = find(rownos.begin(), rownos.end(), j);
								if (tmpint == rownos.end())		//不存在
									rownos.push_back(j);
							}
						}
					}
				}
				else
				{
					for (size_t j = 0; j < iter->second.RecipeItem[i].RecipeData.size(); j++)
					{
						if (iter->second.RecipeItem[i].RecipeData[j] == data)
						{
							vector <int>::iterator tmpint = find(rownos.begin(), rownos.end(), j);
							if (tmpint == rownos.end())		//不存在
								rownos.push_back(j);
						}
					}
				}
			}
			return true;
		}
		return false;
	}

	int RecipeDT::UpLoadRecipe(string groupname, int row, char * data)
	{
		return 0;
	}

	int RecipeDT::DownLoadRecipe(string groupname, int row, Project::DataVarId plcvar)
	{
		return 0;
	}

	int RecipeDT::GetRecipeDataNum(string groupname)
	{
		map<string, RecipeTable>::iterator  iter;
		if (RecipeTbVct.size() == 0)
			return 0;
		iter = RecipeTbVct.find(groupname);
		if (iter != RecipeTbVct.end())			//存在数据
		{
			if (!iter->second.RecipeItem.empty())
				return iter->second.RecipeItem[0].RecipeData.size();
		}
		return 0;
	}
#endif
	void ProjectDataTables::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/1.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else
		{
			//解析userDT结构体
			if (json.HasMember("UserDT"))
			{
				if (json["UserDT"].IsObject() && !json["UserDT"].IsNull())
				{
					//解析UserItem admin;
					if (json["UserDT"].HasMember("Admin"))
					{
						if (json["UserDT"]["Admin"].HasMember("Uname") && !json["UserDT"]["Admin"]["Uname"].IsNull())
							User.Admin.Uname = json["UserDT"]["Admin"]["Uname"].GetString();
						if (json["UserDT"]["Admin"].HasMember("Passwd") && !json["UserDT"]["Admin"]["Passwd"].IsNull())
							User.Admin.Passwd = json["UserDT"]["Admin"]["Passwd"].GetString();
						if (json["UserDT"]["Admin"].HasMember("Region"))
							User.Admin.Region = json["UserDT"]["Admin"]["Region"].GetInt();
						if (json["UserDT"]["Admin"].HasMember("Hide"))
							User.Admin.Hide = json["UserDT"]["Admin"]["Hide"].GetBool();
						if (json["UserDT"]["Admin"].HasMember("Enable"))
							User.Admin.Enable = json["UserDT"]["Admin"]["Enable"].GetBool();
					}
					//解析vector<UserItem> users;
					if (json["UserDT"].HasMember("Users") && !json["UserDT"]["Users"].IsNull())
					{
						Value& array = json["UserDT"]["Users"];
						if (array.IsArray()) {
							for (unsigned i = 0; i < array.Size(); i++) {
								Value& tmp = array[i];
								if (tmp.IsObject() && !tmp.IsNull()) {
									Project::UserItem tmpVector;
									if (tmp.HasMember("Uname") && !tmp["Uname"].IsNull())
										tmpVector.Uname = tmp["Uname"].GetString();
									if (tmp.HasMember("Passwd") && !tmp["Passwd"].IsNull())
										tmpVector.Passwd = tmp["Passwd"].GetString();
									if (tmp.HasMember("Region"))
										tmpVector.Region = tmp["Region"].GetInt();
									if (tmp.HasMember("Hide"))
										tmpVector.Hide = tmp["Hide"].GetBool();
									if (tmp.HasMember("Enable"))
										tmpVector.Enable = tmp["Enable"].GetBool();
									User.Users.push_back(tmpVector);
								}
							}
						}
					}
					//解析vector<string> colDescript;
					if (json["UserDT"].HasMember("ColDescript") && !json["UserDT"]["ColDescript"].IsNull())
					{
						Value& array = json["UserDT"]["ColDescript"];
						if (array.IsArray())
						{
							for (unsigned i = 0; i < array.Size(); i++)
							{
								if (!array[i].IsNull())
									User.ColDescript.push_back(array[i].GetString());
							}

						}
					}
				}
			}
			//解析StringStatusDT结构体
			if (json.HasMember("StrStatusDT"))
			{
				if (json["StrStatusDT"].IsObject() && !json["StrStatusDT"].IsNull())
				{
					if (json["StrStatusDT"].HasMember("StringStatusDt") && !json["StrStatusDT"]["StringStatusDt"].IsNull())
					{
						//解析vector<StringStatusItem> stringStatusDT
						Value &stringDT = json["StrStatusDT"]["StringStatusDt"];
						if (stringDT.IsArray())
						{
							for (unsigned i = 0; i < stringDT.Size(); i++)
							{
								StringStatusItem sItem;
								if (stringDT[i].IsObject() && !stringDT[i].IsNull())
								{
									if (stringDT[i].HasMember("Id"))
										sItem.Id = stringDT[i]["Id"].GetInt();
									if (stringDT[i].HasMember("Tag") && !stringDT[i]["Tag"].IsNull())
										sItem.Tag = stringDT[i]["Tag"].GetString();
									//解析vector<vector<string>> statelangs;
									if (stringDT[i].HasMember("Langs") && stringDT[i]["Langs"].IsArray())
									{
										Value& array = stringDT[i]["Langs"];
										if (array.IsArray()) {
											for (unsigned j = 0; j < array.Size(); j++) {
												Value& tmp = array[j];
												vector<string> tmpVector;
												if (tmp.IsArray() && !tmp.IsNull()) {
													for (unsigned k = 0; k < tmp.Size(); k++)
													{
														if(!tmp[k].IsNull())
															tmpVector.push_back(tmp[k].GetString());
													}
												}
												sItem.StateLangs.push_back(tmpVector);
											}
										}
									}
									//strStatusDT.stringStatusDT.push_back(sItem);
									StrStatus.StringStatusMap.insert(pair<string, StringStatusItem>(sItem.Tag, sItem));
								}
							}

						}
						
					}
				}

			}
			//解析StringLabelDT结构体
			if (json.HasMember("StrLabelDT"))
			{
				if (json["StrLabelDT"].IsObject() && json["StrLabelDT"].HasMember("StrLabelTBs"))
				{
					if (json["StrLabelDT"]["StrLabelTBs"].IsArray() && !json["StrLabelDT"]["StrLabelTBs"].IsNull())
					{
						Value &tbs = json["StrLabelDT"]["StrLabelTBs"];
						for (unsigned tabel = 0; tabel < tbs.Size(); tabel++) {
							if (tbs[tabel].HasMember("Rows") && !tbs[tabel].IsNull())
							{
								StringLabelTable rowsTmp;
								for (unsigned row = 0; row < tbs[tabel]["Rows"].Size();row++)
								{
									vector<string> langTmp;
									if (tbs[tabel]["Rows"][row].HasMember("Langs") && !tbs[tabel]["Rows"][row]["Langs"].IsNull())
									{
										for (unsigned lang = 0; lang < tbs[tabel]["Rows"][row]["Langs"].Size(); lang++)
										{
											if(!tbs[tabel]["Rows"][row]["Langs"][lang].IsNull())
												langTmp.push_back(tbs[tabel]["Rows"][row]["Langs"][lang].GetString());
										}
										rowsTmp.Rows.push_back(langTmp);
									}
								}
								StrLabel.StrLabelVct.push_back(rowsTmp);

							}

						}
					}
				}
			}
			//解析RecipeDT结构体
			if (json.HasMember("XJRecipeDT") && json["XJRecipeDT"].IsObject() && !json["XJRecipeDT"].IsNull())
			{
				if (json["XJRecipeDT"].HasMember("RecipeTBs") && json["XJRecipeDT"]["RecipeTBs"].IsArray())
				{
					Value &recipetbs = json["XJRecipeDT"]["RecipeTBs"];
					for (size_t i = 0; i < recipetbs.Size(); i++)
					{
						string tbname;
						RecipeTable table;
						if (recipetbs[i].HasMember("RecipeTag") && !recipetbs[i]["RecipeTag"].IsNull())
							tbname = recipetbs[i]["RecipeTag"].GetString();
						if (recipetbs[i].HasMember("RecipeTb") && recipetbs[i]["RecipeTb"].IsArray())
							Project::RecipeInfoRes::Parse(table.RecipeItem, recipetbs[i]["RecipeTb"]);
						if (recipetbs[i].HasMember("VarIdRef") && recipetbs[i]["VarIdRef"].IsObject())
							table.IndexVar.Parse(recipetbs[i]["VarIdRef"]);
						if (recipetbs[i].HasMember("OutAddrUsed"))
							table.OutAddrUsed = recipetbs[i]["OutAddrUsed"].GetBool();
						Recipe.RecipeTbVct.insert(pair<string, RecipeTable>(tbname, table));
					}
				}
			}
			//解析EventDT结构体
			if (json.HasMember("XJEventDT"))
			{
				if (json["XJEventDT"].IsObject() && !json["XJEventDT"].IsNull())
				{
					if (json["XJEventDT"].HasMember("EventTB"))
					{
						Value &tmpEvent = json["XJEventDT"]["EventTB"];
						if (tmpEvent.IsArray() && !tmpEvent.IsNull())
						{
							EventItem EItem;
							for (unsigned i = 0; i < tmpEvent.Size(); i++)
							{
								if (tmpEvent[i].HasMember("Id"))
								{
									EItem.id = tmpEvent[i]["Id"].GetInt();
									Event.eventTB.push_back(EItem);

								}

							}
						}
					}
				}
			}
			//解析SampleDT结构体
			if (json.HasMember("XJSimpleDT"))
			{
				if (json["XJSimpleDT"].IsObject() && !json["XJSimpleDT"].IsNull())
				{
					if (json["XJSimpleDT"].HasMember("SampleTB"))
					{
						Value &tmpsimple = json["XJSimpleDT"]["SampleTB"];
						if (tmpsimple.IsArray() && !tmpsimple.IsNull())
						{
							SampleItem SItem;
							for (unsigned i = 0; i < tmpsimple.Size(); i++)
							{
								if (tmpsimple[i].HasMember("Id"))
								{
									SItem.id = tmpsimple[i]["Id"].GetInt();
									Simple.SampleVct.push_back(SItem);
								}

							}
						}
					}
				}
			}

		}
	}
}
