#include "DashboardTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "DashboardConfig.h"
#include "GTestConfig.h"
#include "System.h"

static Project::HMIProject prj;
void DashboardTest::Init()
{
	char buffBack[255];
	char buffArrow[255];
	char buffAxis[255];
	ProjectTestHelper prjHelper(&prj);
	prjHelper.SettingScreen(800, 480);
	prjHelper.AddWin(0, 0, 0, 800, 480, 0, 0xFF);//global page must!
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);
	prjHelper.AddRes("background", System::GetFilePath("\\..\\HmiTest\\res\\zhaojiaqi\\dashboard\\background.bmp", buffBack));
	prjHelper.AddRes("arrow", System::GetFilePath("\\..\\HmiTest\\res\\zhaojiaqi\\dashboard\\arrow.bmp", buffArrow));
	prjHelper.AddRes("axis", System::GetFilePath("\\..\\HmiTest\\res\\zhaojiaqi\\dashboard\\axis.bmp", buffAxis));
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "DataCtlSign.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "MaxData.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 4, 2, "MinData.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, 
		Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 6, 2,
		"UpperWarnAddr.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 8, 2, "LowerWarnAddr.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 10, 2, "UpperDangerRegister.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 12, 2, "LowerDangerRegister.var1");

	prjHelper.AddCtrl(1, "DashboardControl", "Dashboard.tc1", [&prjHelper](void* param)
	{
		Project::DashboardConfig* cfg = (Project::DashboardConfig*)param;
		cfg->CtrlName = "bar1.tc1";
		cfg->DataCtlSign = prjHelper.GetVarID("DataCtlSign.var1");


		cfg->DashBoardStyle.KeyVal = "background";
		cfg->WatchHandleStyle.KeyVal = "arrow";
		cfg->AxisStyle.KeyVal = "axis";
	});
	prjHelper.BindCtrlAndVar("DataCtlSign.var1", "Dashboard.tc1");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW,
		Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2,
		"btn1.var1");
	prjHelper.AddCtrl(1, "GTestControl", "btn1.tc1", [&prjHelper](void* param) {
		Project::GTestConfig* cfg = (Project::GTestConfig*)param;
		cfg->FuncID = Project::FuncType::AddValueF;
		cfg->X = 290;
		cfg->Y = 190;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Val = prjHelper.GetVarID("btn1.var1");
		cfg->CtrlName = "btn1.tc1";
		cfg->Title = "switch";
	});//0
	prjHelper.BindCtrlAndVar("btn1.var1", "btn1.tc1");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW,
		Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2,
		"btn2.var1");
	prjHelper.AddCtrl(1, "GTestControl", "btn2.tc1", [&prjHelper](void* param) {
		Project::GTestConfig* cfg = (Project::GTestConfig*)param;
		cfg->FuncID = Project::FuncType::SubValueF;
		cfg->X = 290;
		cfg->Y = 240;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Val = prjHelper.GetVarID("btn2.var1");
		cfg->CtrlName = "btn2.tc1";
		cfg->Title = "switch";
	});//0


	prjHelper.Update();
}


bool DashboardTest::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}
