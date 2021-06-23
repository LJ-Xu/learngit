#include "AlarmBarTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "AlarmBarConfig.h"
#include "AlarmInputConfig.h"
#include "GTestConfig.h"
#include "GStickChartConfig.h"
#include "System.h"

static Project::HMIProject prj;
void AlarmBarTest::Init()
{
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(800, 480);
	// prjHelper.AddWin(0, 0, 0, 800, 480, 0, 0xFF);//global page must!
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);
	// 添加寄存器
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_SPSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Unsigned, 98, 2, "win1.var0");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "DataCtlSign.var1");

	//警报录入
	prjHelper.AddGCtrl(0, "AlarmGControl", "win1.tc1", [&prjHelper](void * param) {
		Project::AlarmInputConfig * cfg = (Project::AlarmInputConfig *)param;
		cfg->CtrlName = "win1.tc1";
		cfg->AlarmNoticeVarId = prjHelper.GetVarID("win1.var0");
		Project::AlarmInfoRes res;
		res.RegType = 1;
		res.GroupName = 0;
		res.GroupNo = 1;
		res.BtyeCondition = Project::Conditions::GT;
		res.ComPareValue = 15;
		res.ReadRegVar = prjHelper.GetVarID("win1.var1");
		cfg->InfoLst.push_back(res);
	});
	prjHelper.BindCtrlAndVar("win1.var0", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var1", "win1.tc1");
	//++按钮
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
	//prjHelper.BindCtrlAndVar("btn1.var1", "btn1.tc1");
	//--按钮
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
	// 添加寄存器
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Unsigned, 0, 2, "alabar.var0");
	//警报条
	prjHelper.AddCtrl(1, "AlarmBarControl", "alabar.tc1", [&prjHelper](void * param) {
		Project::AlarmBarConfig * cfg = (Project::AlarmBarConfig *)param;
		cfg->CtrlName = "alabar.tc1";
		cfg->AlarmNoticeVarId = prjHelper.GetVarID("alabar.var0");
	});
	prjHelper.BindCtrlAndVar("alabar.var0", "alabar.tc1");

	//棒图
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "BarVar.var1");
	prjHelper.AddCtrl(1, "GStickChartControl", "bar1.tc1", [&prjHelper](void* param) {
		Project::GStickChartConfig* cfg = (Project::GStickChartConfig*)param;
		cfg->X = 350;
		cfg->Y = 50;
		cfg->CtrlName = "bar1.tc1";
		cfg->BarVar = prjHelper.GetVarID("BarVar.var1");
//		cfg->tempBackImg.KeyVal = "buffBar";
		cfg->UseFillinShade = false;
		cfg->ShowBackground = false;
		//当前选择的形状：0直条、1扇形
		cfg->Shape = 0;
		cfg->ArcStartAngle = 0;
		cfg->ArcSweepAngle = 180;
		///直条
		//方向：0上；1下；2左；3右
		cfg->BarDirection = 0;
		cfg->UseRangeWarn = true;

	});
	prjHelper.BindCtrlAndVar("BarVar.var1", "bar1.tc1");



	prjHelper.Update();
}

bool AlarmBarTest::Run(int argc, char ** argv) {
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}
