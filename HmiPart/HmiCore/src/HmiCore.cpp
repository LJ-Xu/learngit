/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name		: HMICore.cs
 * Author   : zhanglinbo
 * Date     : 2020/08/11
 * Descript	: HMI启动核心
 * Version	: 1.0
 * modify	:
 *			:
 *******************************************************************************/
#include <thread>
#include "stdafx.h"
#include "RunEnv.h"
#include "HmiCore.h"
#include "IDataService.h"
#include "ResourceService.h"
#include "ControlFactory.h"
#include "PageFactory.h"
#include "UsrPermission.h"
#include "ThreadRunHelper.h"
#include "UIThread.h"
#include "LocalData.h"
#include "UIServer.h"
#include "UICommand.h"
#include "ProjectParser.h"
#include "Logger.h"
#include "SysCtrlApi.h"
namespace Core
{
	/*******************************************************************************
	 * Name	    : CheckSysEnv
	 * Descript : 对运行环境的检查和系统参数设置
	 * Input	: none
	 * Output   : none
	 * Note	    :
	 *******************************************************************************/
	static bool CheckSysEnv()
	{
		return true;
	}
	int  HmiCore::Init()
	{
		return 0;
	}
	int HmiCore::InitProject(const char* path)
	{
		ProjectParser parser(path);
		//检查工程文件crc是否正常
		if (!parser.CheckCrc())
		{
			LOG_ERROR("Project CheckCrc wrong\n");
			return -1;
		}

		//检查工程版本是否匹配

		//工程数据反序列化
		prjData_ = new Project::HMIProject();
		LOG_INFO("Parser HmiProject\n");
		return parser.Parse(prjData_);
	}
	///初始化带线程的服务程序
	void HmiCore::InitThreadService()
	{
		if (UI::UIDataService::Ins().Init(uiManager_, prjData_))
		{
			UI::UIDataService::Ins().Start();
		}
		if (!UI::UIServer::Init())
			UI::UIServer::Start();
	}
	void HmiCore::ExitThreadService()
	{
		UI::UIServer::Stop();
		UI::UIDataService::Ins().Stop();

	}
	void InitBeep(UI::UIManager* um)
	{
		BeepHelper::Ins().Start();
		um->AddExit([]() {BeepHelper::Ins().Stop(); });
	}
	///初始化基础的服务,此类服务更多是些功能类
	void HmiCore::InitBasicService()
	{
		//Resource
		UI::ResourceService* rs = new UI::ResourceService(prjData_);
		rs->InitResources(prjData_, RunEnv::Cnf.BinPath.c_str());
		UI::IResourceService::SetIns(rs);
		deleteHelper_.AddDeleteMgr(rs);
		LOG_INFO("Success Init ResourceService\n");

		//UICommond
		UI::UICommand *comd = new UI::UICommand(prjData_);
		UI::UICommand::SetIns(comd);
		deleteHelper_.AddDeleteMgr(comd);
		LOG_INFO("Success Init UICommand\n");

		//ctrlfactory
		UI::ControlFactory* cf = new UI::ControlFactory();
		UI::IControlFactory::SetIns(cf);
		deleteHelper_.AddDeleteMgr(cf);
		LOG_INFO("Success Init ctrlfactory\n");

		UI::PageFactory* pf = new UI::PageFactory();
		UI::IPageFactory::SetIns(pf);
		deleteHelper_.AddDeleteMgr(pf);
		LOG_INFO("Success Init PageFactory\n");

		//permisson
		UI::UsrPermission* pm = new UI::UsrPermission(prjData_);
		UI::IPermission::SetIns(pm);
		deleteHelper_.AddDeleteMgr(pm);
		LOG_INFO("Success Init UsrPermission\n");


		InitBeep(uiManager_);

	}
	
	void HmiCore::InitManager()
	{
		uiManager_ = &UI::UIManager::Ins();
		uiManager_->Init(prjData_);
		uiManager_->AddExit([this]() {PickUp(); });
	}
	bool HmiCore::InitService()
	{

		InitManager();
		InitBasicService();
		InitThreadService();

		return true;

	}
	static void RunTd(void* ss)
	{

	}

	int HmiCore::RunModule()
	{

		return uiManager_->RunUI();


	}


	void  HmiCore::PickUp()
	{
		ExitThreadService();
#ifdef WIN32
		ExitProcess(0);
#else
    exit(0);
#endif
	}
	//对异常退出时，进行处理如数据恢复诊断等内容
	void RunCheckLastShutOff()
	{

	}
	int  HmiCore::Run()
	{
		int ret = 0;

		CheckSysEnv();  //检查和设置运行参数变量

		RunCheckLastShutOff();	//对上次异常的处理逻辑

		InitProject(RunEnv::Cnf.BinPath.c_str());//对Projecrt工程进行解析
		LOG_INFO("Success Parser HmiProject\n");

		InitService();         //初始化运行所需的服务
		LOG_INFO("Success InitService\n");

		ret = RunModule();     //启动应用程序
		LOG_INFO("Success RunModule\n");

		//PickUp();              //程序结束，做些清理工作
		
		return ret;

	}

	int  HmiCore::TestRun(Project::HMIProject* prj)
	{
		//RunCheckLastShutOff();
		int ret = 0;

		CheckSysEnv();

		prjData_ = prj;

		InitService();

		ret = RunModule();

		//PickUp();

		return ret;

	}


}
