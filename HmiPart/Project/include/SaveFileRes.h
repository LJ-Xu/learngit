#pragma once
#include "BtnFunctionRes.h"
namespace Project
{
	enum StoreLocat:char
	{
		SL_HMI = 0,
		SL_SDCard,
		SL_USB,
		SL_RegString
	};
	enum StoreTrigMode:char
	{
		STM_ByCount=0,//满足一定采集数量时自动触发
		STM_ByAddr,//地址触发
		STM_ByExit//退出时触发
	};
//通用
#define SAVE_ID		0x01
#define SAVE_DATE	0x02
#define SAVE_TIME	0x03
//采样
#define SAVE_ADDR		0x04	//采样地址
#define SAVE_DATA		0x05	//采样数据
//报警
#define SAVE_INFO			0x04	//报警信息
#define SAVE_COUNT			0x05	//报警次数
#define SAVE_CONFIRM		0x06	//确认时间
#define SAVE_RECOVERYTIME	0x07	//报警回复时间


#define MAXSQLCOUNT     100//触发保存的最大条目数
	struct SaveFileRes
	{
		StoreLocat StoreLocation;//保存位置 1:HMI 2:SD卡 3:U盘 4:寄存器指定位置（字符串地址）
		DataVarId StorePosVarId;//动态存储位置
		vector<string> Contents;//需要保存信息名称
		FlieNameType FileNameMode;//文件名称格式
		string FileName;//文件名称
		DataVarId FileNameDataVar;
		FlieLocation FileStoreSite;	//存储位置
		int StoreSpaceLack;	//存储空间不足时动作	0：停止保存	1：覆盖旧纪录
		bool IsFileSaveTimeLimit;//是否限制数据保留时限
		int SaveDays;//数据保留时限(天)
		char SaveFormat[10];//存储格式
		//非配置信息(当前)
		int UpperLmt;//存储上限
        Project::StoreTrigMode StoreTrigMode= Project::StoreTrigMode::STM_ByCount;
		int SaveTrigCount= MAXSQLCOUNT;//允许的最大未保存条目数
		DataVarId SaveTrigAddr;//保存触发地址
		int SavedCount=0;//已经保存的条目数
		unsigned long long SavedFlag=0;//上一次保存的标记，取自最后的时间
		string FilePath;//最终保存地址

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Contents, FileNameMode, FileName, FileNameDataVar, StoreLocation, StorePosVarId,
				StoreSpaceLack, SaveFormat,IsFileSaveTimeLimit, SaveDays);
		}

		void Parse(rapidjson::Value& jsonObj)
		{
			if (jsonObj.HasMember("StoreLocation") && !jsonObj["StoreLocation"].IsNull())
			{
				StoreLocation = (StoreLocat)jsonObj["StoreLocation"].GetInt();
			}
			if (jsonObj.HasMember("StoreLocationReg") && jsonObj["StoreLocationReg"].IsObject())
				StorePosVarId.Parse(jsonObj["StoreLocationReg"]);

			if (jsonObj.HasMember("SimpleSaveInfoLst")&& jsonObj["SimpleSaveInfoLst"].IsArray())
			{
				vector<string>().swap(Contents);
				memset(SaveFormat, 0, 10);
				for (unsigned i = 0; i < jsonObj["SimpleSaveInfoLst"].Size(); i++)
				{
					bool select;
					if (jsonObj["SimpleSaveInfoLst"][i].HasMember("IsSelect"))
						select = jsonObj["SimpleSaveInfoLst"][i]["IsSelect"].GetBool();
					if (!select)
						continue;
					if (jsonObj["SimpleSaveInfoLst"][i].HasMember("Project")
						&& !jsonObj["SimpleSaveInfoLst"][i]["Project"].IsNull())
					{
						string strTemp = jsonObj["SimpleSaveInfoLst"][i]["Project"].GetString();
						if (!strTemp.compare("序号"))
						{
							SaveFormat[i] = SAVE_ID;
						}
						else if (!strTemp.compare("日期"))
						{
							SaveFormat[i] = SAVE_DATE;
						}
						else if (!strTemp.compare("时间"))
						{
							SaveFormat[i] = SAVE_TIME;
						}
						else if (!strTemp.compare("地址"))
						{
							SaveFormat[i] = SAVE_ADDR;
						}
						else
						{
							SaveFormat[i] = SAVE_DATA;
						}
						/*string strIn;
						if (!select)
						{
							strIn = "0";
						}
						else
						{
							strIn = "1";
						}
						strIn.append(strTemp);*/
						Contents.push_back(strTemp);
					}
				}
			}
			if (jsonObj.HasMember("FileName") && !jsonObj["FileName"].IsNull())
				FileName = jsonObj["FileName"].GetString();
			if (jsonObj.HasMember("FileNameMode"))
				FileNameMode = (FlieNameType)jsonObj["FileNameMode"].GetInt();
			if (jsonObj.HasMember("DynaticFileNameReg") && jsonObj["DynaticFileNameReg"].IsObject())
				FileNameDataVar.Parse(jsonObj["DynaticFileNameReg"]);
			//if (jsonObj.HasMember("StoreLocation"))
			//	FileStoreSite = (FlieLocation)jsonObj["StoreLocation"].GetInt();
			if (jsonObj.HasMember("StoreSpaceLack"))
				StoreSpaceLack = jsonObj["StoreSpaceLack"].GetInt();
			if (jsonObj.HasMember("IsFileSaveTimeLimit"))
				IsFileSaveTimeLimit = jsonObj["IsFileSaveTimeLimit"].GetBool();
			if (jsonObj.HasMember("SaveDays"))
				SaveDays = jsonObj["SaveDays"].GetInt();
		}
	};
}


