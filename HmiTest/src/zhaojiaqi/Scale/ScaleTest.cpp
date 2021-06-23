#include "ScaleTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "ScaleConfig.h"
#include "GTestConfig.h"
#include "System.h"

static Project::HMIProject prj;
void ScaleTest::Init()
{
	ProjectTestHelper prjHelper(&prj);
	prjHelper.SettingScreen(800, 480);
	prjHelper.AddWin(0, 0, 0, 800, 480, 0, 0xFF);//global page must!
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);
	prjHelper.AddCtrl(1, "ScaleControl", "scale.tc1", [&prjHelper](void* param) {
		Project::ScaleConfig* cfg = (Project::ScaleConfig*)param;
		cfg->CtrlName = "scale.tc1";

		//当前选择的形状：0直条、1扇形
		cfg->ScaleTp = cfg->Horizontal;
		// 是否翻转刻度标记,暂时认为↑、→、逆时针为默认数字增长方向,即IsReserveMark=false时
		cfg->IsReserveMark = false;
		cfg->MarkPlace = 0;

	});
	prjHelper.Update();
}

bool ScaleTest::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}