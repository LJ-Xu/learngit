#include "WindowDemo.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "GTestConfig.h"
#include "System.h"
static Project::HMIProject prj;
void WindowDemo::Init()
{
	char buf[255];
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(800, 480);
	prjHelper.AddWin(0, 0, 0, 800, 480, 0, 0xFF);//global page must!
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);
	prjHelper.AddRes("pic1", System::GetFilePath("\\..\\HmiTest\\Res\\bt.bmp", buf));
	prjHelper.AddStrings("switch", 0, "OFF");
	prjHelper.AddStrings("switch", 1, "ON");
	prjHelper.AddStrings("timerLB", 0, "timer bt");
	prjHelper.AddStrings("opendialogLB", 0, "open dlg bt");
	prjHelper.AddStrings("closedialogLB", 0, "cls dlg bt");
	prjHelper.AddStrings("str2", 0, "str2.1");
	prjHelper.AddStrings("str2", 1, "str2.2");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW,
		Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2,
		"win1.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var2");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var3");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win2.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win2.var2");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win2.var3");

	/*for (size_t i = 0; i < 10000; i++)
	{
		char buf[50];
		sprintf(buf, "win1.varx%d", i);
		prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, i+10, 2, buf);
	}*/


	int ctrlid = prjHelper.AddCtrl(1, "GTestControl", "win1.tc1", [&prjHelper](void* param) {
		Project::GTestConfig* cfg = (Project::GTestConfig*)param;
		cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 50;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Val = prjHelper.GetVarID("win1.var1");
		cfg->CtrlName = "win1.tc1";
		cfg->Title = "switch";
	});//0
	prjHelper.AddCtrl(1, "GTestControl", "win1.tc2", [&prjHelper](void* param) {
		Project::GTestConfig* cfg = (Project::GTestConfig*)param;
		cfg->FuncID = Project::FuncType::OpenDialog;
		cfg->VOffX.Vid = prjHelper.GetVarID("win1.var3");
		cfg->X = 100;
		cfg->Y = 100;
		cfg->Width = 50;
		cfg->Height = 50;
		cfg->BGKey = "pic1";
		cfg->Val = prjHelper.GetVarID("win1.var2");
		cfg->CtrlName = "win1.tc2";
		cfg->Title = "opendialogLB";
	});//1
	prjHelper.BindCtrlAndVar("win1.var1", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var2", "win1.tc2");
	prjHelper.BindCtrlAndVar("win1.var3", "win1.tc2");
	for (size_t i = 0; i < 400; i++)
	{
		char* buf = new char[15];
		sprintf(buf, "win1.varx%d", i);
		prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_String, Project::VarNumberType::NT_Unsigned, i + 10, 1000, buf);
		prjHelper.BindCtrlIDAndVar(buf, ctrlid);

		prjHelper.AddCtrl(1, "GTestControl", "win1.tc3", [&prjHelper](void* param) {
			static int i = 0;
			Project::GTestConfig* cfg = (Project::GTestConfig*)param;
			cfg->FuncID = Project::FuncType::OpenDialog;
			cfg->VOffX.Vid = prjHelper.GetVarID("win1.var3");
			cfg->X = 200 + i++ % 100;
			cfg->Y = 200 + i++ % 100;
			cfg->Width = 50;
			cfg->Height = 50;
			cfg->BGKey = "pic1";
			cfg->Val = prjHelper.GetVarID("win1.var2");
			cfg->CtrlName = "win1.tc2";
			cfg->Title = "opendialogLB";
		});//1
	}





	prjHelper.AddWin(2, 100, 100, 600, 300, 0x10f008F0);

	prjHelper.AddCtrl(2, "GTestControl", "win2.tc1", [&prjHelper](void* param) {
		Project::GTestConfig* cfg = (Project::GTestConfig*)param;
		cfg->FuncID = Project::FuncType::TimerF;
		cfg->X = 150;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Val = prjHelper.GetVarID("win2.var1");
		cfg->CtrlName = "win2.tc1";
		cfg->Title = "timerLB";
	});//0
	prjHelper.AddCtrl(2, "GTestControl", "win2.tc2", [&prjHelper](void* param) {
		Project::GTestConfig* cfg = (Project::GTestConfig*)param;
		cfg->FuncID = Project::FuncType::CloseDialog;
		//cfg->VOffX.Vid = prjHelper.GetVarID("win2.var3");
		cfg->X = 250;
		cfg->Y = 250;
		cfg->Width = 50;
		cfg->Height = 50;
		cfg->BGKey = "pic1";
		cfg->Val = prjHelper.GetVarID("win2.var2");
		cfg->CtrlName = "win2.tc2";
		cfg->Title = "closedialogLB";
	});//1
	prjHelper.BindCtrlAndVar("win2.var1", "win2.tc1");
	prjHelper.BindCtrlAndVar("win2.var2", "win2.tc2");
	prjHelper.BindCtrlAndVar("win2.var3", "win2.tc2");



	prjHelper.Update();
}

bool WindowDemo::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}