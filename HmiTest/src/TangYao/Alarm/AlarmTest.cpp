#include "AlarmTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "AlarmInputConfig.h"
#include "GTestConfig.h"
#include "System.h"

static Project::HMIProject prj;

void AlarmTest::Init() {
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(800, 480);
	// prjHelper.AddWin(0, 0, 0, 800, 480, 0, 0xFF);//global page must!
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);

	// Ìí¼Ó¼Ä´æÆ÷
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_SPSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Unsigned, 98, 2, "win1.var0");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var2");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var3");

	prjHelper.AddGCtrl(0, "AlarmGControl", "win1.tc1", [&prjHelper](void * param) {
		Project::AlarmInputConfig * cfg = (Project::AlarmInputConfig *)param;
		cfg->CtrlName = "win1.tc1";
		cfg->AlarmNoticeVarId = prjHelper.GetVarID("win1.var0");
		Project::AlarmInfoRes res;
		res.GroupName = 1;
		res.GroupNo = 1;
		res.RegType = 1;
		res.ReadRegVar = prjHelper.GetVarID("win1.var1");
		res.BtyeCondition = Project::Conditions::LT;
		res.ComPareValue = 10;
		res.Text.Texts.push_back("Alarm");
		cfg->InfoLst.push_back(res);
	});

	prjHelper.AddCtrl(1, "GTestControl", "win1.tc2", [&prjHelper](void* param) {
		Project::GTestConfig* cfg = (Project::GTestConfig*)param;
		cfg->FuncID = Project::FuncType::StringF;
		cfg->Val = prjHelper.GetVarID("win1.var2");
		cfg->CtrlName = "win1.tc2";
		cfg->Title = "switch";
		cfg->Height = 50;
		cfg->Width = 50;
		cfg->X = 100;
		cfg->Y = 100;
	});

	prjHelper.AddCtrl(1, "GTestControl", "win1.tc3", [&prjHelper](void* param) {
		Project::GTestConfig* cfg = (Project::GTestConfig*)param;
		cfg->FuncID = Project::FuncType::StringF;
		cfg->Val = prjHelper.GetVarID("win1.var3");
		cfg->CtrlName = "win1.tc3";
		cfg->Title = "switch";
		cfg->Height = 50;
		cfg->Width = 50;
		cfg->X = 300;
		cfg->Y = 100;
	});

	prjHelper.BindCtrlAndVar("win1.var0", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var1", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var2", "win1.tc2");
	prjHelper.BindCtrlAndVar("win1.var3", "win1.tc3");

	prjHelper.Update();
}

bool AlarmTest::Run(int argc, char ** argv) {
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}