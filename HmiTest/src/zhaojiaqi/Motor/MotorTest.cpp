#include "MotorTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "MotorConfig.h"
#include "GTestConfig.h"
#include "System.h"

static Project::HMIProject prj;
void MotorTest::Init()
{
	char bufOn[255];
	char bufOff[255];
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(800, 480);
	prjHelper.AddWin(0, 0, 0, 800, 480, 0, 0xFF);//global page must!
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);
	//prjHelper.AddRes("picOn", System::GetFilePath("\\..\\HmiTest\\res\\zhaojiaqi\\motor\\bt.bmp", bufOn));



	prjHelper.AddRes("picOn", System::GetFilePath("\\..\\HmiTest\\res\\zhaojiaqi\\motor\\bt.bmp", bufOn));
	prjHelper.AddRes("picOff", System::GetFilePath("\\..\\HmiTest\\res\\zhaojiaqi\\motor\\bt1.bmp", bufOff));

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW,
		Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2,
		"moto1.var1");
	prjHelper.AddCtrl(1, "MotorControl", "moto1.tc1", [&prjHelper](void* param) {
		Project::MotorConfig* cfg = (Project::MotorConfig*)param;
		cfg->DataCtlSign = prjHelper.GetVarID("moto1.var1");
		cfg->CtrlName = "moto1.tc1";
		cfg->KeyImgOn.KeyVal = "picOn";
		cfg->KeyImgOff.KeyVal = "picOff";
		cfg->X = 50;
		cfg->Y = 50;
		cfg->Width = 100;
		cfg->Height = 100;
		//m_motorConfig.DataCtlSign.Vid = 0;

		cfg->CtlLogic = 0;
		cfg->UseBlink = true;
		//0ÎªonÉÁË¸
		cfg->BlinkState = 0;
		//1Ãë0.2´Î
		cfg->BlinkFrequency = 2;
	});
	prjHelper.BindCtrlAndVar("moto1.var1", "moto1.tc1");


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

bool MotorTest::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}
