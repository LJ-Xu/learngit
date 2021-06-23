/*******************************************************************************
 * Copyright (C) 2011-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * FileName	: HmiBuilderLib.cpp
 * Author	: zhanglinbo
 * Descript	: 用于PC机上进行编译序列化的接口集合,该库只在WIN32上运行
 * Version	: 0.1
 * Modify	:
 *			:
 *******************************************************************************/
#include "stdafx.h"
#include <Windows.h>
#include "HMIProject.h"
#include "ControlFactory.h"
#include "BaseControl.h"
#include "BaseModel.h"
using namespace std;
Project::HMIProject* prj;
extern "C"  __declspec(dllexport) int __stdcall Builder_Init()
{
	prj = new Project::HMIProject();
	return 0;

}
extern "C"  __declspec(dllexport) int __stdcall Builder_Exit()
{
	if (prj)
	{
		delete prj;
		prj = nullptr;
	}	
	return 0;
}
extern "C"  __declspec(dllexport) int __stdcall Builder_AddProjectInfo(char * json)
{
	if (prj == nullptr||json==nullptr)
		return -1;
	prj->Info.InitData(json);
	return 0;
}
extern "C"  __declspec(dllexport) int __stdcall Builder_AddProjectSetting(char * json)
{
	if (prj == nullptr || json == nullptr)
		return -1;
	prj->Setting.InitData(json);
	return 0;
}

extern "C"  __declspec(dllexport) int __stdcall Builder_AddProjectDevices(char * json)
{
	if (prj == nullptr || json == nullptr)
		return -1;
	prj->Devices.InitData(json);
	return 0;
}
extern "C"  __declspec(dllexport) int __stdcall Builder_AddProjectDataTables(char * json)
{
	if (prj == nullptr || json == nullptr)
		return -1;
	prj->Tables.InitData(json);
	return 0;
}
extern "C"  __declspec(dllexport) int __stdcall Builder_AddProjectMacros(char * json)
{
	if (prj == nullptr || json == nullptr)
		return -1;
	prj->Macros.InitData(json);
	return 0;
}
extern "C"  __declspec(dllexport) int __stdcall Builder_AddDataVars(char * json)
{

	if (prj == nullptr || json == nullptr)
		return -1;
	prj->Vars.InitData(json);
	return 0;

}

extern "C"  __declspec(dllexport) int __stdcall Builder_AddProjectSystemSet(char * json)
{

	if (prj == nullptr || json == nullptr)
		return -1;
	prj->SysSetting.InitData(json);
	return 0;
}
//该区域留空
extern "C"  __declspec(dllexport) int __stdcall Builder_Resources_Init(int size)
{
	prj->Reses.Size = size;
	return 0;
}

extern "C"  __declspec(dllexport) int __stdcall Builder_Resources_AddKeyIndex(char* key,int idx)
{
	prj->Reses.ResKeyIdxs.insert(std::pair<string, int>(key, idx));
	return 0;
}
extern "C"  __declspec(dllexport) int __stdcall Builder_Resources_AddUnitInfo(
										//char* key,
										int   fileSize,
										char  filetype,
										int   offset,
										char* param,
                                        int   paramSize)
{
	Project::ResItem item;
	item.FileType = filetype;
	item.Size = fileSize;
	item.Offset = offset;
	memcpy(item.Param, param, min(paramSize, sizeof(item.Param)));
	prj->Reses.ResMap.push_back(item);
	//prj->Reses.ResMap.insert(std::pair<string, Project::ResItem>(key, item));
	return 0;
}
extern "C"  __declspec(dllexport) int __stdcall Builder_AddPagesInfo(char * json)
{
	return 1;
}
extern "C"  __declspec(dllexport) int __stdcall Builder_AddGlobalWindow(char * json)
{
	if (prj == nullptr || json == nullptr)
		return -1;
	/*添加窗口*/
	prj->Pages.Screens.insert(std::pair<int, Project::PageInfo>(0, Project::PageInfo()));
	if (prj->Pages.Screens.find(0) == prj->Pages.Screens.end())
		return -2;

	Project::PageInfo& win = prj->Pages.Screens[0];
	win.InitData(json);
	win.Catogray = 0xff;
	std::shared_ptr<UI::BaseControl> alarmctrl = UI::ControlFactory::GetNewControlByNameEx("AlarmGControl");
	std::shared_ptr<UI::BaseModel> alarmmode = alarmctrl->GetMode();
	alarmmode->InitData(json);
	std::unique_ptr<std::vector<char>> alarmvector = alarmmode->GetData();
	win.Shapes.push_back(Project::UnitInfo(string("AlarmGControl")));
	win.Shapes[win.Shapes.size() - 1].Data.swap(*alarmvector);

	std::shared_ptr<UI::BaseControl> samplectrl = UI::ControlFactory::GetNewControlByNameEx("SampleGCtrl");
	std::shared_ptr<UI::BaseModel> samplemode = samplectrl->GetMode();
	samplemode->InitData(json);
	std::unique_ptr<std::vector<char>> samplevector = samplemode->GetData();
	win.Shapes.push_back(Project::UnitInfo(string("SampleGCtrl")));
	win.Shapes[win.Shapes.size() - 1].Data.swap(*samplevector);

	std::shared_ptr<UI::BaseControl> advancectrl = UI::ControlFactory::GetNewControlByNameEx("AdvanceFuncGControl");
	std::shared_ptr<UI::BaseModel> advancemode = advancectrl->GetMode();
	advancemode->InitData(json);
	std::unique_ptr<std::vector<char>> advancevector = advancemode->GetData();
	win.Shapes.push_back(Project::UnitInfo(string("AdvanceFuncGControl")));
	win.Shapes[win.Shapes.size() - 1].Data.swap(*advancevector);


	std::shared_ptr<UI::BaseControl> operatorctrl = UI::ControlFactory::GetNewControlByNameEx("OperatorGControl");
	std::shared_ptr<UI::BaseModel> operatormode = operatorctrl->GetMode();
	operatormode->InitData(json);
	std::unique_ptr<std::vector<char>> operatorvector = operatormode->GetData();
	win.Shapes.push_back(Project::UnitInfo(string("OperatorGControl")));
	win.Shapes[win.Shapes.size() - 1].Data.swap(*operatorvector);

	
	std::shared_ptr<UI::BaseControl> syssetctrl = UI::ControlFactory::GetNewControlByNameEx("SysSetGControl");
	std::shared_ptr<UI::BaseModel> syssetmode = syssetctrl->GetMode();
	syssetmode->InitData(json);
	std::unique_ptr<std::vector<char>> syssetvector = syssetmode->GetData();
	win.Shapes.push_back(Project::UnitInfo(string("SysSetGControl")));
	win.Shapes[win.Shapes.size() - 1].Data.swap(*syssetvector);
	/*std::shared_ptr<UI::BaseControl> hirePurctrl = UI::ControlFactory::GetNewControlByNameEx("HirePurchaseGControl");
	std::shared_ptr<UI::BaseModel> hirePurmode = hirePurctrl->GetMode();
	hirePurmode->InitData(json);
	std::unique_ptr<std::vector<char>> hirePurvector = hirePurmode->GetData();
	win.Shapes.push_back(Project::UnitInfo(string("HirePurchaseGControl")));
	win.Shapes[win.Shapes.size() - 1].Data.swap(*hirePurvector);*/
	return 1;
}
//添加窗体的信息，然后再Builder_Pages_AddShapes 分布添加shapes的内容
extern "C"  __declspec(dllexport) int __stdcall Builder_Pages_AddWindow(int winno,char * json)
{
	if (prj == nullptr || json == nullptr)

		return -1;

	prj->Pages.Screens.insert(std::pair<int,Project::PageInfo>(winno, Project::PageInfo()));
	
	prj->Pages.Screens[winno].InitData(json);
	prj->Pages.Screens[winno].Catogray = 0;
	return 0;
}
extern "C"  __declspec(dllexport) int __stdcall Builder_Pages_AddShapes(int winno,char* name,char * json)
{
	if (prj == nullptr || json == nullptr)

		return -1;

	if (prj->Pages.Screens.find(winno) == prj->Pages.Screens.end())

		return -2;

	Project::PageInfo& win  = prj->Pages.Screens[winno];

	std::shared_ptr<UI::BaseControl> ctrl = UI::ControlFactory::GetNewControlByNameEx(name);

	std::shared_ptr<UI::BaseModel> mode = ctrl->GetMode();

	mode->InitData(json);
	
	std::unique_ptr<std::vector<char>> vector =  mode->GetData();

	win.Shapes.push_back(Project::UnitInfo(string(name)));
 
	win.Shapes[win.Shapes.size() - 1].Data.swap(*vector);

	return 0;
}
extern "C"  __declspec(dllexport) int __stdcall Builder_Bin_WriteFile(char * path)
{
	prj->ToBinFile(path);
	return 0;
}



/****************************文件生成后的处理*******************************/
//CRC
extern "C"  __declspec(dllexport) int __stdcall Builder_Bin_UpdateHeader(char * path)
{
	
	return 0;
}