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
		//安全时间相关
		bool IsOperatecfm;				//是否需要操作前确认
		int WaitTime;					//操作前确认时间
		bool IsKeydelay;				//是否需要按键延时
		int DelayTime;					//按键延时时间

		//显示控制
		bool IsShowVerity;				//是否使用显示控制
		Project::DataVarId ShowVID;		//显示ID
		int ShowLogic;					//0：ON为使能  1：OFF失能
		char DisplayMode;				//行为模式 0:关闭时任然显示  1:关闭时隐藏   2:关闭时灰字显示


		//使能控制
		bool IsEnableVerity;			//是否使用使能控制
		Project::DataVarId EnableVID;	//显示ID
		int EnableLogic;				////0：ON为使能  1：OFF失能

		//用户权限
		int RequireRegion;			//需要的权限范围
		bool QuitLimit;				//权限取消
		bool IsPopwin;				//弹窗
		bool IsHideElement;			//隐藏
		char Mode;					//0:   1:无权限下弹出窗体   2：无权限下隐藏

		bool HasShowPerm = true;		//是否有显示权限
		bool HasLimitShowPerm = true;		//是否有显示权限
		bool HasEnablePerm = true;		//是否有显示权限
		bool HasShowEnablePerm = true;		//是否有显示权限
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


