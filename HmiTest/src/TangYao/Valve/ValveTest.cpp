#include "ValveTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "ValveConfig.h"
#include "GTestConfig.h"
#include "StirrerCOnfig.h"
#include "System.h"

static Project::HMIProject prj;

void ValveTest::Init() {
	char bufOpen[255] = { 0 };
	char bufClose[255] = { 0 };
	char buf1[255] = { 0 };
	char buf2[255] = { 0 };
	char buf3[255] = { 0 };
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(800, 480);
	prjHelper.AddWin(0, 0, 0, 800, 480, 0, 0xFF);//global page must!
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);
	// 添加资源图片
	prjHelper.AddRes("valveOpen", System::GetFilePath("\\..\\HmiTest\\Res\\TangYao\\Valve\\valve_05_a.bmp", bufOpen));
	prjHelper.AddRes("valveClose", System::GetFilePath("\\..\\HmiTest\\Res\\TangYao\\Valve\\valve_05_b.bmp", bufClose));
	prjHelper.AddRes("stirrer1", System::GetFilePath("\\..\\HmiTest\\Res\\TangYao\\Stirrer\\agitator_02_a.bmp", buf1));
	prjHelper.AddRes("stirrer2", System::GetFilePath("\\..\\HmiTest\\Res\\TangYao\\Stirrer\\agitator_02_b.bmp", buf2));
	prjHelper.AddRes("stirrer3", System::GetFilePath("\\..\\HmiTest\\Res\\TangYao\\Stirrer\\agitator_02_c.bmp", buf3));
	// 添加寄存器
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var2");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var3");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var4");
	// 添加控件
	prjHelper.AddCtrl(1, "ValveControl", "win1.tc1", [&prjHelper](void* param) {
		Project::ValveConfig * cfg = (Project::ValveConfig*)param;
		cfg->ReadVar = prjHelper.GetVarID("win1.var1");
		cfg->WriteVar = cfg->ReadVar;
		cfg->CtrlName = "win1.tc1";
		// 添加控件图片资源
		unique_ptr<Project::ImageResId> res;
		res.reset(new Project::ImageResId());
		res->KeyVal = "valveOpen";
		cfg->PicKey.push_back(*res);
		res.reset(new Project::ImageResId());
		res->KeyVal = "valveClose";
		cfg->PicKey.push_back(*res);
		res.reset();
		cfg->Act = Project::BtnAction(3);
		// 控件属性
		cfg->Logic = 0;
		cfg->Dir = 0;
		cfg->Speed = 5;
		// 控件基础属性
		cfg->Height = 110;
		cfg->Width = 100;
		cfg->X = 100;
		cfg->Y = 50;
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
		cfg->Y = 106;
	});

	prjHelper.AddCtrl(1, "StirrerControl", "win1.tc3", [&prjHelper](void* param) {
		Project::StirrerConfig * cfg = (Project::StirrerConfig*)param;
		cfg->ReadVar = prjHelper.GetVarID("win1.var3");
		cfg->CtrlName = "win1.tc3";
		unique_ptr<Project::ImageResId> res;
		res.reset(new Project::ImageResId());
		res->KeyVal = "stirrer1";
		cfg->Keys.push_back(*res);
		res.reset(new Project::ImageResId());
		res->KeyVal = "stirrer2";
		cfg->Keys.push_back(*res);
		res.reset(new Project::ImageResId());
		res->KeyVal = "stirrer3";
		cfg->Keys.push_back(*res);
		res.reset();
		cfg->Height = 180;
		cfg->Width = 90;
		cfg->X = 100;
		cfg->Y = 200;
		cfg->Logic = 0;
		cfg->Dir = 0;
		cfg->Speed = 200;
	});
	
	prjHelper.AddCtrl(1, "StirrerControl", "win1.tc4", [&prjHelper](void* param) {
		Project::StirrerConfig * cfg = (Project::StirrerConfig*)param;
		cfg->ReadVar = prjHelper.GetVarID("win1.var4");
		cfg->CtrlName = "win1.tc4";
		unique_ptr<Project::ImageResId> res;
		res.reset(new Project::ImageResId());
		res->KeyVal = "stirrer1";
		cfg->Keys.push_back(*res);
		res.reset(new Project::ImageResId());
		res->KeyVal = "stirrer2";
		cfg->Keys.push_back(*res);
		res.reset(new Project::ImageResId());
		res->KeyVal = "stirrer3";
		cfg->Keys.push_back(*res);
		res.reset();
		cfg->Height = 180;
		cfg->Width = 90;
		cfg->X = 300;
		cfg->Y = 200;
		cfg->Logic = 0;
		cfg->Dir = 1;
		cfg->Speed = 50;
	});
	

	prjHelper.BindCtrlAndVar("win1.var1", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var2", "win1.tc2");
	prjHelper.BindCtrlAndVar("win1.var3", "win1.tc3");
	prjHelper.BindCtrlAndVar("win1.var4", "win1.tc4");
	prjHelper.Update();
}


bool ValveTest::Run(int argc, char ** argv) {
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}