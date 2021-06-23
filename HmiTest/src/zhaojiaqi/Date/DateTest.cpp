#include "DateTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "DateConfig.h"
#include "GTestConfig.h"
#include "ClockConfig.h"
#include "System.h"

static Project::HMIProject prj;
void DateTest::Init()
{
	char backpic[255];
	char backpic1[255];
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(800, 480);
	prjHelper.AddWin(0, 0, 0, 800, 480, 0, 0xFF);//global page must!
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);
	//prjHelper.AddRes("picOn", System::GetFilePath("\\..\\HmiTest\\res\\zhaojiaqi\\motor\\bt.bmp", bufOn));



	prjHelper.AddRes("backpic", System::GetFilePath("\\..\\HmiTest\\res\\zhaojiaqi\\motor\\bt.bmp", backpic));

	prjHelper.AddCtrl(1, "DateControl", "date.tc1", [&prjHelper](void* param) {
		Project::DateConfig* cfg = (Project::DateConfig*)param;
		cfg->CtrlName = "date.tc1";
		cfg->BackUpImg.KeyVal = "backpic";
		cfg->X = 50;
		cfg->Y = 50;
		//m_motorConfig.DataCtlSign.Vid = 0;

	});


	prjHelper.AddRes("backpic1", System::GetFilePath("\\..\\HmiTest\\res\\zhaojiaqi\\motor\\bt1.bmp", backpic1));

	prjHelper.AddCtrl(1, "ClockControl", "clock.tc1", [&prjHelper](void* param) {
		Project::ClockConfig* cfg = (Project::ClockConfig*)param;
		cfg->CtrlName = "clock.tc1";
		cfg->BackUpImg.KeyVal = "backpic1";
		cfg->X = 150;
		cfg->Y = 50;
		//m_motorConfig.DataCtlSign.Vid = 0;

	});
	prjHelper.Update();



	prjHelper.Update();
}

bool DateTest::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}


