#include "BarCodeTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "BarCodeConfig.h"
#include "GTestConfig.h"
#include "System.h"

static Project::HMIProject prj;

void BarCodeTest::Init()
{
	char bufValue[255];
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(800, 480);
	prjHelper.AddWin(0, 0, 0, 800, 480, 0, 0xFF);//global page must!
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW,
		Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2,
		"barcode.var1");
	prjHelper.AddCtrl(1, "BarCodeControl", "barcode.tc1", [&prjHelper](void* param) {
		Project::BarCodeConfig* cfg = (Project::BarCodeConfig*)param;
		cfg->ReadVar = prjHelper.GetVarID("barcode.var1");
		cfg->CtrlName = "barcode.tc1";
		cfg->X = 50;
		cfg->Y = 50;
		cfg->Width = 200;
		cfg->Height = 200;
		cfg->UseConstValue = true;
		cfg->ConstString = "1";
		cfg->CorrectErrorLevel = 3;
		//m_BarCodeConfig.DataCtlSign.Vid = 0;
	});
	prjHelper.BindCtrlAndVar("barcode.var1", "barcode.tc1");



}

bool BarCodeTest::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}