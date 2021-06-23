#include "ProjectPortVarsInfo.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
using namespace rapidjson;
namespace Project
{
	void ProjectPortVarsInfo::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/ProjectPortVarsInfo.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else
		{
			//解析std::map<int, PrjPortPoint> points;
			if (json.HasMember("Points") && !json["Points"].IsNull())
			{
				Value& PointArray = json["Points"];
				for (rapidjson::Value::MemberIterator iter = PointArray.MemberBegin();iter != PointArray.MemberEnd();iter++)
				{
					string name = (iter->name).GetString();
					Value& value = iter->value;
					if (value.IsObject() && !value.IsNull())
					{
#if MASK
						PrjPortPoint pointVector;
						if (value.HasMember("DevName") && !value["DevName"].IsNull())
							pointVector.Name = value["DevName"].GetString();
						if (value.HasMember("FDevId"))
							pointVector.ProtocolId = value["FDevId"].GetInt();
						if (value.HasMember("Port"))
							pointVector.Port = (PortType)value["Port"].GetInt();
						if (value.HasMember("Point"))
							pointVector.Point = (PointType)value["Point"].GetInt();
						if (value.HasMember("Param") && value["Param"].IsObject())
						{
							if (value["Param"].HasMember("OutTime"))
								pointVector.Param.OutTime = value["Param"]["OutTime"].GetInt();
							if (value["Param"].HasMember("DelayTime"))
								pointVector.Param.DelayTime = value["Param"]["DelayTime"].GetInt();
							if (value["Param"].HasMember("RetryTimes"))
								pointVector.Param.RetryTimes = value["Param"]["RetryTimes"].GetInt();
						}
						Points.push_back(pointVector);
#endif
					}
				}
			}
			//解析vector<DataVarInfo> staticVarInfos;
			if (json.HasMember("DataVars") && !json["DataVars"].IsNull())
			{
				Value& sVarArray = json["DataVars"];
				if (sVarArray.IsArray()) {
					for (unsigned i = 0; i < sVarArray.Size(); i++)
					{
						DataVarInfo staticVar;
						Value &sVarTmp = sVarArray[i];
						if (!sVarArray[i].IsNull())
						{
							
							if (sVarTmp.HasMember("Data"))
								staticVar.Data = sVarTmp["Data"].GetInt();
							if (sVarTmp.HasMember("DevId"))
								staticVar.DevId = sVarTmp["DevId"].GetInt();
							if (sVarTmp.HasMember("StaNo"))
								staticVar.StaNo = sVarTmp["StaNo"].GetInt();
							if (sVarTmp.HasMember("RegType"))
								staticVar.RegType = sVarTmp["RegType"].GetInt();
							if (sVarTmp.HasMember("Count"))
								staticVar.Count = sVarTmp["Count"].GetInt();
							if (sVarTmp.HasMember("Addr"))
								staticVar.Addr = sVarTmp["Addr"].GetInt();
							if (sVarTmp.HasMember("Addr2"))
								staticVar.Addr1 = sVarTmp["Addr2"].GetInt();	
							if (sVarTmp.HasMember("RegType"))
								staticVar.RegType = sVarTmp["RegType"].GetInt();
							if (sVarTmp.HasMember("DataType"))
								staticVar.DataType = sVarTmp["DataType"].GetInt();
							
							/*if (sVarTmp.HasMember("Winno"))
								staticVar.Winno = sVarTmp["Winno"].GetInt();*/
							if (sVarTmp.HasMember("DataFmt"))
								staticVar.DataExType = sVarTmp["DataFmt"].GetInt();
							if (sVarTmp.HasMember("CtrlIdx"))
								staticVar.CtrlId = sVarTmp["CtrlIdx"].GetInt();
							if (sVarTmp.HasMember("RawAddr"))
								staticVar.RawAddr = sVarTmp["RawAddr"].GetInt();
							if (sVarTmp.HasMember("Sub") && !sVarTmp["Sub"].IsNull())
							{
									staticVar.Sub.Parse(sVarTmp["Sub"]);
							}
							if (sVarTmp.HasMember("OffsetVarID") && !sVarTmp["OffsetVarID"].IsNull())
							{
									staticVar.IndctVid.Parse(sVarTmp["OffsetVarID"]);
							}
 
							if (sVarTmp.HasMember("FNext") && !sVarTmp["FNext"].IsNull())
							{
									staticVar.FNext.Parse(sVarTmp["FNext"]);
							}
							VarInfos.push_back(staticVar);
						}
						else
							VarInfos.push_back(DataVarInfo());
					}
				}
			}
			//解析map<int, WindowVar> WinVarInfos;
			if (json.HasMember("WinVarsDic"))
			{
				if (json["WinVarsDic"].IsObject() && !json["WinVarsDic"].IsNull())
				{
					Value &winvarTmp = json["WinVarsDic"];
					for (rapidjson::Value::MemberIterator iter = winvarTmp.MemberBegin();iter != winvarTmp.MemberEnd();iter++)
					{
						WindowVar winvar;
						string num = (iter->name).GetString();
						Value& value = iter->value;
						if (value.IsObject() && !value.IsNull())
						{
							if (value.HasMember("FrameInfos") && value["FrameInfos"].IsArray())
							{
								vector<FrameInfo>().swap(winvar.Frames);
								for (size_t i = 0; i < value["FrameInfos"].Size(); i++)
								{
									FrameInfo tmpframe;
									if (value["FrameInfos"][i].HasMember("RegBitSize"))
										tmpframe.RISize = value["FrameInfos"][i]["RegBitSize"].GetInt();

									if (value["FrameInfos"][i].HasMember("DataVarIds") &&
										value["FrameInfos"][i]["DataVarIds"].IsArray())
									{
										vector<DataVarId>().swap(tmpframe.VIds);
										for (size_t j = 0; j < value["FrameInfos"][i]["DataVarIds"].Size(); j++)
										{
											DataVarId vid;
											Value& framejson = value["FrameInfos"][i]["DataVarIds"][j];
											bool isnull = false;
											if (framejson.HasMember("IsNull"))
												isnull = framejson["IsNull"].GetBool();
											if (!isnull)
											{
												if (framejson.HasMember("VID"))
												{
													vid.Vid = framejson["VID"].GetInt();
													tmpframe.VIds.push_back(vid);
												}
											}
										}
									}
									if (value["FrameInfos"][i].HasMember("BaseVar") &&
										value["FrameInfos"][i]["BaseVar"].IsObject())
									{
										if (value["FrameInfos"][i]["BaseVar"].HasMember("DevId"))
											tmpframe.Var.DevId = value["FrameInfos"][i]["BaseVar"]["DevId"].GetInt();
										if (value["FrameInfos"][i]["BaseVar"].HasMember("StaNo"))
											tmpframe.Var.StaNo = value["FrameInfos"][i]["BaseVar"]["StaNo"].GetInt();
										if (value["FrameInfos"][i]["BaseVar"].HasMember("RegType"))
											tmpframe.Var.RegType = value["FrameInfos"][i]["BaseVar"]["RegType"].GetInt();
										if (value["FrameInfos"][i]["BaseVar"].HasMember("Addr"))
											tmpframe.Var.Addr = value["FrameInfos"][i]["BaseVar"]["Addr"].GetInt();
										if (value["FrameInfos"][i]["BaseVar"].HasMember("Addr2"))
											tmpframe.Var.Addr1 = value["FrameInfos"][i]["BaseVar"]["Addr2"].GetInt();
										if (value["FrameInfos"][i]["BaseVar"].HasMember("Count"))
											tmpframe.Var.Count = value["FrameInfos"][i]["BaseVar"]["Count"].GetInt();
										if (value["FrameInfos"][i]["BaseVar"].HasMember("DataType"))
											tmpframe.Var.DataType = value["FrameInfos"][i]["BaseVar"]["DataType"].GetInt();
										//if (value["FrameInfos"][i]["BaseVar"].HasMember("DataFmt"))
										//	tmpframe.Var.DataExType = value["FrameInfos"][i]["BaseVar"]["DataFmt"].GetInt();
									}
									winvar.Frames.push_back(tmpframe);
								}
							}
							if (value.HasMember("DataVarIds") && value["DataVarIds"].IsArray())
							{
								vector<DataVarId>().swap(winvar.VIds);
								for (size_t i = 0; i < value["DataVarIds"].Size(); i++)
								{
									DataVarId vid;
									bool isnull = false;
									if (value["DataVarIds"][i].HasMember("IsNull"))
										isnull = value["DataVarIds"][i]["IsNull"].GetBool();
									if (!isnull)
									{
										if (value["DataVarIds"][i].HasMember("VID"))
										{
											vid.Vid = value["DataVarIds"][i]["VID"].GetInt();
											winvar.VIds.push_back(vid);
										}
									}
								}
							}
							if (value.HasMember("Buffsize"))
								winvar.BufSize = value["Buffsize"].GetInt();
							WinVarInfos.insert(pair<int, WindowVar>(atoi(num.c_str()), winvar));
						}
					}
				}

			}
 
 
		}
	}
}