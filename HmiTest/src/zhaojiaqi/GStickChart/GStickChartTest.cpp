#include "GStickChartTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "GStickChartConfig.h"
#include "GTestConfig.h"
#include "System.h"

static Project::HMIProject prj;
void GStickChartTest::Init()
{
	char buffBar[255];
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(800, 480);
	prjHelper.AddWin(0, 0, 0, 800, 480, 0, 0xFF);//global page must!
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);
	prjHelper.AddRes("buffBar", System::GetFilePath("\\..\\HmiTest\\res\\zhaojiaqi\\motor\\testfade.bmp", buffBar));

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "BarVar.var1");
	/*prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW,background
		Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2,
		"DstValueAddr.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 4, 2, "UpperWarnAddr.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 6, 2, "LowerWarnAddr.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 8, 2, "MaxData.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 10, 2, "MinData.var1");*/


	prjHelper.AddCtrl(1, "GStickChartControl", "bar1.tc1", [&prjHelper](void* param) {
		Project::GStickChartConfig* cfg = (Project::GStickChartConfig*)param;
		cfg->CtrlName = "bar1.tc1";
		cfg->BarVar = prjHelper.GetVarID("BarVar.var1");
		cfg->BarRes.KeyVal = "buffBar";
		cfg->UseFillinShade = true;
		cfg->ShowBackground = false;
		/*cfg->DstRangeAddr = prjHelper.GetVarID("DstRangeAddr.var1");
		cfg->UpperWarnAddr = prjHelper.GetVarID("UpperWarnAddr.var1");
		cfg->LowerWarnAddr = prjHelper.GetVarID("LowerWarnAddr.var1");
		cfg->MaxData = prjHelper.GetVarID("MaxData.var1"); 
		cfg->MinData = prjHelper.GetVarID("MinData.var1"); */
		//当前选择的形状：0直条、1扇形
		cfg->Shape = 1;
		cfg->ArcStartAngle = 0;
		cfg->ArcSweepAngle = 180;
		///直条
		//方向：0上；1下；2左；3右
		cfg->BarDirection = 0;
		cfg->UseRangeWarn = true;



	});
	prjHelper.BindCtrlAndVar("BarVar.var1", "bar1.tc1");
	prjHelper.BindCtrlAndVar("DstValueAddr.var1", "bar1.tc1");
	prjHelper.BindCtrlAndVar("UpperWarnAddr.var1", "bar1.tc1");
	prjHelper.BindCtrlAndVar("LowerWarnAddr.var1", "bar1.tc1");

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

	prjHelper.BindCtrlAndVar("btn2.var1", "btn2.tc1");

	prjHelper.Update();
}

bool GStickChartTest::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}