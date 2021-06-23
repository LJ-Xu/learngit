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
		STM_ByCount=0,//����һ���ɼ�����ʱ�Զ�����
		STM_ByAddr,//��ַ����
		STM_ByExit//�˳�ʱ����
	};
//ͨ��
#define SAVE_ID		0x01
#define SAVE_DATE	0x02
#define SAVE_TIME	0x03
//����
#define SAVE_ADDR		0x04	//������ַ
#define SAVE_DATA		0x05	//��������
//����
#define SAVE_INFO			0x04	//������Ϣ
#define SAVE_COUNT			0x05	//��������
#define SAVE_CONFIRM		0x06	//ȷ��ʱ��
#define SAVE_RECOVERYTIME	0x07	//�����ظ�ʱ��


#define MAXSQLCOUNT     100//��������������Ŀ��
	struct SaveFileRes
	{
		StoreLocat StoreLocation;//����λ�� 1:HMI 2:SD�� 3:U�� 4:�Ĵ���ָ��λ�ã��ַ�����ַ��
		DataVarId StorePosVarId;//��̬�洢λ��
		vector<string> Contents;//��Ҫ������Ϣ����
		FlieNameType FileNameMode;//�ļ����Ƹ�ʽ
		string FileName;//�ļ�����
		DataVarId FileNameDataVar;
		FlieLocation FileStoreSite;	//�洢λ��
		int StoreSpaceLack;	//�洢�ռ䲻��ʱ����	0��ֹͣ����	1�����Ǿɼ�¼
		bool IsFileSaveTimeLimit;//�Ƿ��������ݱ���ʱ��
		int SaveDays;//���ݱ���ʱ��(��)
		char SaveFormat[10];//�洢��ʽ
		//��������Ϣ(��ǰ)
		int UpperLmt;//�洢����
        Project::StoreTrigMode StoreTrigMode= Project::StoreTrigMode::STM_ByCount;
		int SaveTrigCount= MAXSQLCOUNT;//��������δ������Ŀ��
		DataVarId SaveTrigAddr;//���津����ַ
		int SavedCount=0;//�Ѿ��������Ŀ��
		unsigned long long SavedFlag=0;//��һ�α���ı�ǣ�ȡ������ʱ��
		string FilePath;//���ձ����ַ

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
						if (!strTemp.compare("���"))
						{
							SaveFormat[i] = SAVE_ID;
						}
						else if (!strTemp.compare("����"))
						{
							SaveFormat[i] = SAVE_DATE;
						}
						else if (!strTemp.compare("ʱ��"))
						{
							SaveFormat[i] = SAVE_TIME;
						}
						else if (!strTemp.compare("��ַ"))
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


