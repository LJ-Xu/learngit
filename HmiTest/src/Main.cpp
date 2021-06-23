//
// "$Id$"
//
// Another button test program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     http://www.fltk.org/COPYING.php
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//
#if 1
#include <map>
#include <string>
#include "demo/DataService_Test_PSW.h"
#include "demo/HmiCore_Test_Run.h"
#include "demo/Serialize_Test_Memory.h"
#include "demo/Log4cpp_Test_Use.h"
#include "demo/Breakpad_Test_Use.h"
#include "demo/Sqlite_Test_Use.h"
#include "demo/Gtest_test_Use.h"
#include "demo/TrendChart_Test_Run.h"
//#include "ProjectPortVarsInfo.h"
#include "./TemplateDemo/WindowDemo.h"
#include "./qd/GDynPicTest/GDynPicTest.h"
#include "./qd/GPolyLineTest/GPolyLineTest.h"
#include "./qd/TableTest/TableTest.h"
#include "./qd/DropMenuTest/DropMenuTest.h"
#include "./qd/SlideInputTest/SlideInputTest.h"
#include "./qd/FluidTest/FluidTest.h"
#include "./qd/GifTest/GifTest.h"
#include "./qd/KeyPadTest/KeyPadTest.h"
#include "./qd/BtnPilotLampTest/BtnPilotLampTest.h"
#include "./qd/PilotLampTest/PilotLampTest.h"
#include "./qd/GLineTest/GLineTest.h"
#include "./qd/GArcTest/GArcTest.h"
#include "./qd/GCircleTest/GCircleTest.h"
#include "./qd/GRectangleTest/GRectangleTest.h"
#include "./qd/StaticStringTest/StaticStringTest.h"
#include "./zhaojiaqi/Motor/MotorTest.h"
#include "./zhaojiaqi/Scale/ScaleTest.h"
#include "./zhaojiaqi/Dashboard/DashboardTest.h"
#include "./zhaojiaqi/GStickChart/GStickChartTest.h"
#include "./zhaojiaqi/Date/DateTest.h"
#include "./zhaojiaqi/Buzzer/BuzzerTest.h"
#include "./zhaojiaqi/BarCode/BarCodeTest.h"
#include "./zhaojiaqi/AlarmBar/AlarmBarTest.h"

#include "./TangYao/Valve/ValveTest.h"
#include "./TangYao/Stirrer/StirrerTest.h"
#include "./TangYao/PieChart/PieChartTest.h"
#include "./TangYao/WaterPump/WaterPumpTest.h"
#include "./TangYao/Sample/SampleTest.h"
#include "./TangYao/Alarm/AlarmTest.h"

#include "./qd/DigitalDisplayTest/DigitalDisplayTest.h"
#include "./qd/DigitalInputTest/DigitalInputTest.h"
#include "./qd/CharacterDisplayTest/CharacterDisplayTest.h"
#include "./qd/TrendChartTest/TrendChartTest.h"
#include "./qd/MultiStatusKeypadTest/MultiStatusKeypadTest.h"
#include "./qd/MultiPilotLampTest/MultiPilotLampTest.h"
#include "./qd/VarStringTest/VarStringTest.h"
#include "./qd/CharacterInputTest/CharacterInputTest.h"
#include "./qd/ChineseDisplayTest/ChineseDisplayTest.h"
#include "./qd/PinYinPageTest/PinYinPageTest.h"
#include "./qd/ChineseInputTest/ChineseInputTest.h"
#include "./qd/ReportFormTest/ReportFormTest.h"
#include "./qd/XYLineChartTest/XYLineChartTest.h"
#include "./qd/AlarmDisplayTest/AlarmDisplayTest.h"
#include "./qd/RecipeChartTest/RecipeChartTest.h"
#include "./qd/XYTrendTest/XYTrendTest.h"
#include "./qd/DataTableTest/DataTableTest.h"
typedef BaseTest* (*NewTestFunc)(void);
template < class T>
BaseTest* NewTest()
{
	return (BaseTest*)new T();
}
std::map<std::string, NewTestFunc> newTestFuncMap = {
		//{"TrendChart_Test_Run", []() {return NewTest<TrendChart_Test_Run>(); }  },
		//{"DataService_Test_PSW", []() {return NewTest<DataService_Test_PSW>(); }  },
		//{"HmiCore_Test_Run", []() {return NewTest<HmiCore_Test_Run>(); }  },
		//{"Sqlite_Test_Use", []() {return NewTest<Sqlite_Test_Use>(); }  },
		//{"Gtest_test_Use", []() {return NewTest<Gtest_test_Use>(); }  },
		//{"Log4cpp_Test_Use", []() {return NewTest<Log4cpp_Test_Use>(); }  },
		//{"Breakpad_Test_Use", []() {return NewTest<Breakpad_Test_Use>(); }  },
		//{"Serialize_Test_Memory", []() {return NewTest<Serialize_Test_Memory>(); }  },
		//{"WindowDemo", []() {return NewTest<WindowDemo>(); }  },


		/**********************QiaoDan**************************/
		//{"GDynPicTest", []() {return NewTest<GDynPicTest>(); }  },
		//{"GPolyLineTest", []() {return NewTest<GPolyLineTest>(); }  },
		//{"TableTest", []() {return NewTest<TableTest>(); }  },
		//{"DropMenuTest", []() {return NewTest<DropMenuTest>(); }  },
		//{"SlideInputTest", []() {return NewTest<SlideInputTest>(); }  },
		//{"FluidTest", []() {return NewTest<FluidTest>(); }  },
		//{"GifTest", []() {return NewTest<GifTest>(); }  },
		//{"KeyPadTest", []() {return NewTest<KeyPadTest>(); }  },
		//{"BtnPilotLampTest", []() {return NewTest<BtnPilotLampTest>(); }  },
		//{"PilotLampTest", []() {return NewTest<PilotLampTest>(); }  },
		//{"GLineTest", []() {return NewTest<GLineTest>(); }  },
		//{"GArcTest", []() {return NewTest<GArcTest>(); }  },
		//{"GCircleTest", []() {return NewTest<GCircleTest>(); }  },
		//{"GRectangleTest", []() {return NewTest<GRectangleTest>(); }  },
		//{"StaticStringTest", []() {return NewTest<StaticStringTest>(); }  },
		//{"DigitalDisplayTest", []() {return NewTest<DigitalDisplayTest>(); }  },
		//{"DigitalInputTest", []() {return NewTest<DigitalInputTest>(); }  },
		//{"CharacterDisplayTest", []() {return NewTest<CharacterDisplayTest>(); }  },
		//{"CharacterInputTest", []() {return NewTest<CharacterInputTest>(); }  },
		//{"TrendChartTest", []() {return NewTest<TrendChartTest>(); }  },
		//{"MultiStatusKeypadTest", []() {return NewTest<MultiStatusKeypadTest>(); }  },
		//{"MultiPilotLampTest", []() {return NewTest<MultiPilotLampTest>(); }  },
		//{"VarStringTest", []() {return NewTest<VarStringTest>(); }  },
		//{"ChineseDisplayTest", []() {return NewTest<ChineseDisplayTest>(); }  },
		//{"PinYinPageTest", []() {return NewTest<PinYinPageTest>(); }  },
		//{"ChineseInputTest", []() {return NewTest<ChineseInputTest>(); }  },
		//{"ReportFormTest", []() {return NewTest<ReportFormTest>(); }  },
		//{"XYLineChartTest", []() {return NewTest<XYLineChartTest>(); }  },
		//{"AlarmDisplayTest", []() {return NewTest<AlarmDisplayTest>(); }  },
		//{"RecipeChartTest", []() {return NewTest<RecipeChartTest>(); }  },
		//{"XYTrendTest", []() {return NewTest<XYTrendTest>(); }  },
		{"DataTableTest", []() {return NewTest<DataTableTest>(); }  },

	    //{"WindowDemo", []() {return NewTest<WindowDemo>(); }  },
	    //{"MotorTest", []() {return NewTest<MotorTest>(); }  },
	    //{"ValveTest", []() {return NewTest<ValveTest>(); }}
		//{"GStickChartTest", []() {return NewTest<GStickChartTest>(); }  },
		//{"BuzzerTest", []() {return NewTest<BuzzerTest>(); }  },
		//{"GStickChartTest", []() {return NewTest<GStickChartTest>(); }  },
		//{"BuzzerTest", []() {return NewTest<BuzzerTest>(); }  },
		//{"BarCodeTest", []() {return NewTest<BarCodeTest>(); }  },
		//{"AlarmBarTest", []() {return NewTest<AlarmBarTest>(); }  },
		//{"DateTest", []() {return NewTest<DateTest>(); }  },
		//{"WaterPumpTest", []() {return NewTest<WaterPumpTest>(); }},
		//{"StirrerTest", []() {return NewTest<StirrerTest>(); }},
		//{"SampleTest", [] {return NewTest<SampleTest>(); }},
		//{"DashboardTest", []() {return NewTest<DashboardTest>(); }  },

		//{"ScaleTest", []() {return NewTest<ScaleTest>(); }  },
	{"PieChartTest", []() {return NewTest<PieChartTest>(); }}

};
BaseTest* GetNewTestByName(std::string name)
{
	if (newTestFuncMap.count(name))
		return newTestFuncMap[name]();
	return nullptr;
}

int main(int argc, char ** argv)
{
	//struct Project::ProjectPortVarsInfo	prjifo;
	//prjifo.InitData("s");
	std::map<std::string, NewTestFunc>::iterator it;
	it = newTestFuncMap.begin();
	while (it != newTestFuncMap.end())
	{
		BaseTest* test = it->second();
		test->Run(argc, argv);
		it++;
	}
	return 0;
}

#else


#include <memory.H>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Secret_Input.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Color_Chooser.H>
#include "HMIWindow.h"
#include "Message.h"
#include "client/windows/handler/exception_handler.h"

#include "StaticStringControl.h"
#include "GArcControl.h"
#include "GLabelControl.h"
#include "DateControl.h"
#include "FluidUnitControl.h"
#include "BtnPilotLampControl.h"
#include "VarStringControl.h"
#include "KeyBoardControl.h"
#include "GRectangleControl.h"
#include "GArcControl.h"
#include "GCircleControl.h"
#include "GLineControl.h"
#include "GDynPicControl.h"
#include "PilotLampControl.h"
#include "CharacterInputControl.h"
#include "GPolyLineControl.h"
#include "DigitalInputControl.h"
#include "GScaleControl.h"
#include "GStickChartControl.h"
#include "KeypadControl.h"
#include "DigitalDisplayControl.h"
#include "TableControl.h"
#include "DropMenuControl.h"
#include "GifPicControl.h"

UI::HMIWindow *window;

int main(int argc, char ** argv) {
	//HMIWindow window(500, 500);
	UI::HMIPage *page = new UI::HMIPage(1,0,0,800,480);
	window = new UI::HMIWindow(0,0,800, 480);
#if 1
	std::shared_ptr<UI::GifPicControl> ctrl = std::make_shared<UI::GifPicControl>(page);
	std::shared_ptr<UI::BaseControl> pctrl = std::static_pointer_cast<UI::BaseControl>(ctrl);
	pctrl->CreateView("cd");

#endif
	window->end();
	window->show(argc, argv);
	return Fl::run();
}
#endif
#if 0

#include "HMIProject.h"
#include "ProjectDataTables.h"

int main(int argc, char ** argv)
{
	Project::HMIProject prj;

	map<string, Project::StringStatusItem>& dt = prj.tables.strStatusDT.stringStatusDT;

	Project::StringStatusItem item;
	item.tag = "name";
	vector<string> st1;
	st1.push_back("s1-zhang-lang1");
	st1.push_back("s1-zhang-lang2");
	vector<string> st2;
	st2.push_back("s2-zhang-lang1");
	st2.push_back("s2-zhang-lang2");
	item.statelangs.push_back(st1);
	item.statelangs.push_back(st2);
	dt.insert(pair<string, Project::StringStatusItem>("name", item));
	prj.ToBinFile("D:\\1.bin");


	Project::HMIProject prj2;
	prj2.LoadBinFile("D:\\1.bin");
	map<string, Project::StringStatusItem>& dtt = prj2.tables.strStatusDT.stringStatusDT;
	cout << dtt["name"].statelangs[0][0] << endl;




	while (1);

}
#endif
