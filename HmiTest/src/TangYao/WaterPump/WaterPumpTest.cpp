#include "WaterPumpTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "WaterPumpConfig.h"
#include "GTestConfig.h"
#include "System.h"

static Project::HMIProject prj;

void WaterPumpTest::Init() {
	char buf1[255] = { 0 };
	char buf2[255] = { 0 };
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(800, 480);
	prjHelper.AddWin(0, 0, 0, 800, 480, 0, 0xFF);//global page must!
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);
	// 添加资源图片
	prjHelper.AddRes("WaterPump1", System::GetFilePath("\\..\\HmiTest\\Res\\TangYao\\WaterPump\\pump_01_a.bmp", buf1));
	prjHelper.AddRes("WaterPump2", System::GetFilePath("\\..\\HmiTest\\Res\\TangYao\\WaterPump\\pump_01_b.bmp", buf2));
	// 添加寄存器
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var2");
	// 添加控件
	prjHelper.AddCtrl(1, "WaterPumpControl", "win1.tc1", [&prjHelper](void* param) {
		Project::WaterPumpConfig * cfg = (Project::WaterPumpConfig*)param;
		cfg->ReadVar = prjHelper.GetVarID("win1.var1");
		cfg->CtrlName = "win1.tc1";
		unique_ptr<Project::ImageResId> res;
		res.reset(new Project::ImageResId());
		res->KeyVal = "WaterPump1";
		cfg->Keys.push_back(*res);
		res.reset(new Project::ImageResId());
		res->KeyVal = "WaterPump2";
		cfg->Keys.push_back(*res);
		res.reset();
		cfg->Height = 150;
		cfg->Width = 90;
		cfg->X = 100;
		cfg->Y = 100;
		cfg->Logic = 0;
		cfg->Dir = 0;
		cfg->Speed = 100;
	});
	prjHelper.AddCtrl(1, "GTestControl", "win1.tc2", [&prjHelper](void* param) {
		Project::GTestConfig* cfg = (Project::GTestConfig*)param;
		cfg->FuncID = Project::FuncType::StringF;
		cfg->Val = prjHelper.GetVarID("win1.var2");
		cfg->CtrlName = "win1.tc2";
		cfg->Title = "switch";
		cfg->Height = 50;
		cfg->Width = 50;
		cfg->X = 300;
		cfg->Y = 100;
	});
	prjHelper.BindCtrlAndVar("win1.var1", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var2", "win1.tc2");
	prjHelper.Update();
}

bool WaterPumpTest::Run(int argc, char ** argv) {
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}