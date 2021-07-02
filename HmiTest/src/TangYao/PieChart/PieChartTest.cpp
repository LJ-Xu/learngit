#include "PieChartTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "PieChartConfig.h"
#include "System.h"

using namespace Project;

static Project::HMIProject prj;
vector<Project::Color> colors;

void PieChartTest::Init() {
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(800, 480);
	prjHelper.AddWin(0, 0, 0, 800, 480, 0, 0xFF);//global page must!
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);

	// Ìí¼Ó¼Ä´æÆ÷
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var2");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var3");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 3, 2, "win1.var4");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 4, 2, "win1.var5");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 5, 2, "win1.var6");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 6, 2, "win1.var7");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 7, 2, "win1.var8");

	colors.push_back(Color(240, 248, 255));
	colors.push_back(Color(250, 235, 215));
	colors.push_back(Color(0, 255, 255));
	colors.push_back(Color(127, 255, 212));

	// Ìí¼Ó¿Ø¼þ
	prjHelper.AddCtrl(1, "PieChartControl", "win1.tc1", [&prjHelper](void* param) {
		PieChartConfig * cfg = (PieChartConfig*)param;
		// cfg->ReadVars.push_back(prjHelper.GetVarID("win1.var1"));
		// cfg->ReadVars.push_back(prjHelper.GetVarID("win1.var2"));
		// cfg->ReadVars.push_back(prjHelper.GetVarID("win1.var3"));
		// cfg->ReadVars.push_back(prjHelper.GetVarID("win1.var4"));
		//cfg->ReadVar = prjHelper.GetVarID("win1.var1");
		cfg->Dir = 0;
		cfg->Mode = 2;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 12;
		for (int i = 0; i < cfg->ChannelNum; ++i) {
			PieChartChannelStyle style = PieChartChannelStyle();
			style.Val = i + 1;
			style.FontColor = Color(0, 0, 255);
			style.BackColor = colors[i];
			cfg->Channels.push_back(style);
			cfg->TotalVal += style.Val;
		}
		for (int i = 0; i < cfg->ChannelNum; ++i) {
			cfg->Channels[i].Percent = cfg->Channels[i].Val / cfg->TotalVal;
		}
		cfg->CenterRadius = 0;
		cfg->BorderColor = Color(255, 0, 0);
		cfg->CtrlName = "win1.tc1";
		cfg->Height = 200;
		cfg->Width = 200;
		cfg->X = 100;
		cfg->Y = 100;
	});

	prjHelper.BindCtrlAndVar("win1.var1", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var2", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var3", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var4", "win1.tc1");
	prjHelper.Update();
}

bool PieChartTest::Run(int argc, char ** argv) {
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}