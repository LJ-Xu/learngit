#include "SampleTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "DataSampleConfig.h"
#include "GTestConfig.h"
#include "System.h"

static Project::HMIProject prj;

void SampleTest::Init() {
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(800, 480);
	// prjHelper.AddWin(0, 0, 0, 800, 480, 0, 0xFF);//global page must!
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);

	// Ìí¼Ó¼Ä´æÆ÷
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_SPSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Unsigned, 99, 2, "win1.var0");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var2");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var3");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 3, 2, "win1.var4");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 4, 2, "win1.var5");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var6");	

	// Ìí¼Ó¿Ø¼þ
	prjHelper.AddGCtrl(0, "SampleGCtrl", "win1.tc1", [&prjHelper](void * param) {
		Project::DataSampleConfig * cfg = (Project::DataSampleConfig *)param;
		cfg->CtrlName = "win1.tc1";
		cfg->SampleNoticeVarId = prjHelper.GetVarID("win1.var0");
		Project::SampleInfoRes res;
		res.SimpleGroupName = 111;
		res.SimpleGroupNo = 123;
		res.SimpleDescribe = "Sample";
		res.SimpleCtrlVarId = prjHelper.GetVarID("win1.var1");
		res.SimpleStyle = 0;
		res.CollectMode = Project::SampleMode::Cycle;
		// res.SimpleCycleVarId = prjHelper.GetVarID("win1.var2");
		res.SimpleCycle = 2000;
		res.TriggerBitMode = 1;
		res.TriggerReadAddr = prjHelper.GetVarID("win1.var3");
		res.BtyeCondition = Project::Conditions::LT;
		res.BitCondition = 0;
		res.BitCond = Project::BitConditions::ON;
		res.ComPareValue = 10;
		// res.ComPareVar = prjHelper.GetVarID("win1.var4");
		res.CollectTimeFrom = 0;
		res.CollectTimeTo = 1;
		res.IsContinueAddr = true;
		Project::SampleChannel channel;
		channel.Channel = 1;
		channel.SampleAddr = prjHelper.GetVarID("win1.var5");
		channel.WordNum = 1;
		channel.DataStyle = "Word";
		channel.DataFormat = "Unsigned";
		channel.Describe = "Channel1";
		res.SimpleChannelLst.push_back(channel);
		cfg->InfoLst.push_back(res);
	});

	prjHelper.AddCtrl(1, "GTestControl", "win1.tc2", [&prjHelper](void* param) {
		Project::GTestConfig* cfg = (Project::GTestConfig*)param;
		cfg->FuncID = Project::FuncType::StringF;
		cfg->Val = prjHelper.GetVarID("win1.var6");
		cfg->CtrlName = "win1.tc2";
		cfg->Title = "switch";
		cfg->Height = 50;
		cfg->Width = 50;
		cfg->X = 300;
		cfg->Y = 100;
	});

	prjHelper.BindCtrlAndVar("win1.var0", "win1.tc1");
	// prjHelper.BindCtrlAndVar("win1.var1", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var2", "win1.tc1");
	// prjHelper.BindCtrlAndVar("win1.var3", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var4", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var5", "win1.tc2");

	prjHelper.Update();
}

bool SampleTest::Run(int argc, char ** argv) {
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}