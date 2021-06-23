#include "BuzzerTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "BuzzerConfig.h"
#include "GTestConfig.h"
#include "System.h"


static Project::HMIProject prj;

void BuzzerTest::Init()
{
	ProjectTestHelper prjHelper(&prj);
	prjHelper.SettingScreen(800, 480);
	prjHelper.AddWin(0, 0, 0, 800, 480, 0, 0xFF);//global page must!
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW,
		Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2,
		"beep1.var1");

	prjHelper.AddCtrl(1, "BuzzerControl", "beep.tc1", [&prjHelper](void* param) {
		Project::BuzzerConfig* cfg = (Project::BuzzerConfig*)param;
		cfg->TrigVar = prjHelper.GetVarID("beep1.var1");
		cfg->CtrlName = "beep.tc1";
		cfg->Mode = 1;
	});

	prjHelper.BindCtrlAndVar("beep1.var1", "beep.tc1");


	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW,
		Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2,
		"win1.var1");
	prjHelper.AddCtrl(1, "GTestControl", "win1.tc1", [&prjHelper](void* param) {
		Project::GTestConfig* cfg = (Project::GTestConfig*)param;
		cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 190;
		cfg->Y = 190;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Val = prjHelper.GetVarID("win1.var1");
		cfg->CtrlName = "win1.tc1";
		cfg->Title = "switch";
	});//0

	prjHelper.BindCtrlAndVar("win1.var1", "win1.tc1");



	prjHelper.Update();
}

bool BuzzerTest::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}