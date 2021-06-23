#pragma once
#include "BasePerm.h"
#include "PermissionVar.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

namespace Project
{

	class Permission 
	{
	public:
		//��ȫʱ�����
		bool IsOperatecfm;				//�Ƿ���Ҫ����ǰȷ��
		int WaitTime;					//����ǰȷ��ʱ��
		bool IsKeydelay;				//�Ƿ���Ҫ������ʱ
		int DelayTime;					//������ʱʱ��

		//��ʾ����
		bool IsShowVerity;				//�Ƿ�ʹ����ʾ����
		Project::DataVarId ShowVID;		//��ʾID
		int ShowLogic;					//0��ONΪʹ��  1��OFFʧ��
		char DisplayMode;				//��Ϊģʽ 0:�ر�ʱ��Ȼ��ʾ  1:�ر�ʱ����   2:�ر�ʱ������ʾ


		//ʹ�ܿ���
		bool IsEnableVerity;			//�Ƿ�ʹ��ʹ�ܿ���
		Project::DataVarId EnableVID;	//��ʾID
		int EnableLogic;				////0��ONΪʹ��  1��OFFʧ��

		//�û�Ȩ��
		int RequireRegion;			//��Ҫ��Ȩ�޷�Χ
		bool QuitLimit;				//Ȩ��ȡ��
		bool IsPopwin;				//����
		bool IsHideElement;			//����
		char Mode;					//0:   1:��Ȩ���µ�������   2����Ȩ��������

		bool HasShowPerm = true;		//�Ƿ�����ʾȨ��
		bool HasLimitShowPerm = true;		//�Ƿ�����ʾȨ��
		bool HasEnablePerm = true;		//�Ƿ�����ʾȨ��
		bool HasShowEnablePerm = true;		//�Ƿ�����ʾȨ��
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(DelayTime, ShowVID, ShowLogic, DisplayMode, EnableVID, 
				IsOperatecfm, IsKeydelay, IsShowVerity, IsEnableVerity,
				QuitLimit, IsPopwin, EnableLogic, IsHideElement, RequireRegion,
				Mode, WaitTime);
		}

		void Parse(rapidjson::Value& jsonObj)
		{
			if (jsonObj.HasMember("IsOperatecfm"))
				IsOperatecfm = jsonObj["IsOperatecfm"].GetBool();
			if (jsonObj.HasMember("IsKeydelay"))
				IsKeydelay = jsonObj["IsKeydelay"].GetBool();

			if (jsonObj.HasMember("WaitTime"))
				WaitTime = jsonObj["WaitTime"].GetInt();
			if (jsonObj.HasMember("DelayTime"))
				DelayTime = jsonObj["DelayTime"].GetInt();

			if (jsonObj.HasMember("ShowVID") && jsonObj["ShowVID"].IsObject())
				ShowVID.Parse(jsonObj["ShowVID"]);
			if (jsonObj.HasMember("IsShowVerity"))
				IsShowVerity = jsonObj["IsShowVerity"].GetBool();
			if (jsonObj.HasMember("ShowVIDState"))
				ShowLogic = jsonObj["ShowVIDState"].GetInt();
			if (jsonObj.HasMember("DisPlayMode"))
				DisplayMode = (char)jsonObj["DisPlayMode"].GetInt();

			if (jsonObj.HasMember("IsEnableVerity"))
				IsEnableVerity = jsonObj["IsEnableVerity"].GetBool();
			if (jsonObj.HasMember("EnableVID") && jsonObj["EnableVID"].IsObject())
				EnableVID.Parse(jsonObj["EnableVID"]);
			if(jsonObj.HasMember("EnableVIDState"))
				EnableLogic = jsonObj["EnableVIDState"].GetInt();

			if (jsonObj.HasMember("LimitRegion"))
				RequireRegion = jsonObj["LimitRegion"].GetInt();
			if (jsonObj.HasMember("QuitLimit"))
				QuitLimit = jsonObj["QuitLimit"].GetBool();
			if (jsonObj.HasMember("IsPopwin"))
				IsPopwin = jsonObj["IsPopwin"].GetBool();
			if (jsonObj.HasMember("IsHideElement"))
				IsHideElement = jsonObj["IsHideElement"].GetBool();
			if (jsonObj.HasMember("Mode"))
				Mode = (char)jsonObj["Mode"].GetInt();
		}

	};

}


