/*******************************************************************************
 * Copyright (C) 2011-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * FileName	: ControlFactory.cpp
 * Author	: zhanglinbo
 * Descript	: 创建各种Control
 * Version	: 0.1
 * Modify	:
 *			:
 *******************************************************************************/
#ifdef WIN32
#include <Windows.h>
#endif

#include "GArcControl.h"
#include "GCircleControl.h"
#include "GLabelControl.h"
#include "GLineControl.h"
#include "GPolyLineControl.h"
#include "GRectangleControl.h"
#include "GScaleControl.h"
#include "GStickChartControl.h"
#include "ScaleControl.h"
//#include "AlarmDisplayControl.h"
#include "BtnFunctionControl.h"
#include "MultiStatusKeypadControl.h"
#include "BtnPilotLampControl.h"
#include "BtnTriggleControl.h"
#include "DateControl.h"
#include "FluidUnitControl.h"
#include "DigitalInputControl.h"
#include "KeyBtnControl.h"
#include "StaticStringControl.h"
#include "VarStringControl.h"
#include "GTestControl.h"
#include "CharacterInputControl.h"
#include "GDynPicControl.h"
#include "CharacterDisplayControl.h"
#include "DigitalDisplayControl.h"
#include "PilotLampControl.h"
#include "KeyMsgControl.h"
#include "ChineseDisplayControl.h"
#include "ChineseBtnControl.h"
#include "KeypadControl.h"
#include "ChineseInputControl.h"
#include "TrendChartControl.h"
#include "GTestControl.h"
#include "SampleGCtrl.h"
#include "TestBitVarControl.h"
#include "MotorControl.h"
#include "ValveControl.h"
#include "StirrerControl.h"
#include "PieChartControl.h"
#include "WaterPumpControl.h"
#include "BuzzerControl.h"
#include "BarCodeControl.h"
#include "AlarmBarControl.h"
#include "AlarmGControl.h"
#include "GFuncControl.h"
#include "GifPicControl.h"
#include "DateControl.h"
#include "ClockControl.h"
#include "TableControl.h"
#include "DropMenuControl.h"
#include "SlideInputControl.h"
#include "MultiPilotLampControl.h"
#include "DashboardControl.h"
#include "PinYinPageControl.h"
#include "ReportFormControl.h"
#include "XYLineChartControl.h"
#include "AlarmDisplayControl.h"
#include "RecipeChartControl.h"
#include "OperatorGControl.h"
#include "OperatorChartControl.h"
//#include "HirePurchaseControl.h"
#include "RecipeTransferControl.h"
#include "SysSetGControl.h"

//#include "RecipeGControl.h"
#include "RegionFunctionControl.h"
#include "XYTrendControl.h"
#include "AdvanceFuncGControl.h"
#include "DataTableControl.h"
#include "FanControl.h"
#include "BackLightControl.h"
#include "TimerControl.h"
#include "stdafx.h"
#include <math.h>
#include <memory>
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
#include <map>
#include "BaseGControl.h"
#include "ControlFactory.h"

namespace UI
{
	typedef std::unique_ptr<BaseControl> (*NewControlFunc)(void);
	typedef std::unique_ptr<BaseGControl>(*NewGControlFunc)(void);
	template < class T,class M>
	std::unique_ptr<M> NewControlEx()
	{
		std::unique_ptr<M> ptr(new T());
		return ptr;
	
	}
	template < class T>
	std::unique_ptr<BaseControl> NewControl()
	{
		return NewControlEx<T,BaseControl>();
	}
	template < class T>
	std::unique_ptr<BaseGControl> NewGControl()
	{
		return NewControlEx<T,BaseGControl>();
	}
	static map<int, NewGControlFunc> newGControlFuncMap = {
		{GCtrlEnum::SAMPLE, []() {return NewGControl<SampleGCtrl>(); }  },	 
	};
	static map<string, NewControlFunc> newControlFuncMap = {
#if 0
		{"GArcControl", []() {return NewControl<GArcControl>(); }  },
		{"GCircleControl",[]() {return NewControl<GCircleControl>(); }  },
		{"GLabelControl",[]() {return NewControl<GLabelControl>(); }  },
		{"GLineControl",[]() {return NewControl<GLineControl>(); }  },
		{"GPolyLineControl",[]() {return NewControl<GPolyLineControl>(); }  },
		{"GDynPicControl",[]() {return NewControl<GDynPicControl>(); }  },
		{"GRectangleControl",[]() {return NewControl<GRectangleControl>(); }  },
		{"GScaleControl",[]() {return NewControl<GScaleControl>(); }  },
		{"BarControl",[]() {return NewControl<GStickChartControl>(); }  },
		//{"AlarmDisplayControl",[]() {return NewControl<AlarmDisplayControl>(); }  },
		{"BtnFunctionControl",[]() {return NewControl<BtnFunctionControl>(); }  },
		{"BtnMultiStatusControl",[]() {return NewControl<MultiStatusKeypadControl>(); }  },
		{"BtnPilotLampControl",[]() {return NewControl<BtnPilotLampControl>(); }  },
		{"BtnTriggleControl",[]() {return NewControl<BtnTriggleControl>(); }  },
		{"DateControl",[]() {return NewControl<DateControl>(); }  },
		{"FluidUnitControl",[]() {return NewControl<FluidUnitControl>(); }  },
		{"DigitalInputControl",[]() {return NewControl<DigitalInputControl>(); }  },
		{"CharacterInputControl",[]() {return NewControl<CharacterInputControl>(); }  },
		{"StaticStringControl",[]() {return NewControl<StaticStringControl>(); }  },
		{"VarStringControl",[]() {return NewControl<VarStringControl>(); }  },
		{"KeyBtnControl",[]() {return NewControl<KeyBtnControl>(); }  },
		{"KeyMsgControl",[]() {return NewControl<KeyMsgControl>(); }  },
		{"DigitalDisplayControl",[]() {return NewControl<DigitalDisplayControl>(); }  },
		{"CharacterDisplayControl",[]() {return NewControl<CharacterDisplayControl>(); }  },
		{"PilotLampControl",[]() {return NewControl<PilotLampControl>(); }  },
		{"ChineseBtnControl",[]() {return NewControl<ChineseBtnControl>(); }  },
		{"ChineseDisplayControl",[]() {return NewControl<ChineseDisplayControl>(); }  },
		{"ChineseInputControl",[]() {return NewControl<ChineseInputControl>(); }  },
		{"KeypadControl",[]() {return NewControl<KeypadControl>(); }  },
		{"MultiStatusKeypadControl",[]() {return NewControl<MultiStatusKeypadControl>(); }  },
		{"TrendChartControl",[]() {return NewControl<TrendChartControl>(); }  },
		{"ScaleControl",[]() {return NewControl<ScaleControl>(); }  },
		{"DashboardControl",[]() {return NewControl<DashboardControl>(); }  },
		{"BuzzerControl",[]() {return NewControl<BuzzerControl>(); }  },
		{"BarCodeControl",[]() {return NewControl<BarCodeControl>(); }  },
		{"AlarmBarControl",[]() {return NewControl<AlarmBarControl>(); }  },
#endif	
	{"GTestControl",[]() {return NewControl<GTestControl>(); }  },
		{"GDynPicControl",[]() {return NewControl<GDynPicControl>(); }  },
		{"TestBitVarControl",[]() {return NewControl<TestBitVarControl>(); }  },
		{"MotorControl", []() {return NewControl<MotorControl>(); }  },
		{"GPolyLineControl",[]() {return NewControl<GPolyLineControl>(); }  },
		{"GPolylineControl",[]() {return NewControl<GPolyLineControl>(); }  },
		{"TableControl",[]() {return NewControl<TableControl>(); }  },
		{"GTableControl",[]() {return NewControl<TableControl>(); }  },
		{"DropMenuControl",[]() {return NewControl<DropMenuControl>(); }  },
		{"DropDownMenuControl",[]() {return NewControl<DropMenuControl>(); }  },
		{"SwipeInputControl",[]() {return NewControl<SlideInputControl>(); }  },
		{"SlideInputControl",[]() {return NewControl<SlideInputControl>(); }  },
		{"ValveControl", []() {return NewControl<ValveControl>(); }},
		{"BarControl",[]() {return NewControl<GStickChartControl>(); }  },
		{"DateBubbleControl",[]() {return NewControl<DateControl>(); }  },
		{"ClockBubbleControl",[]() {return NewControl<ClockControl>(); }  },
		{"FluidUnitControl",[]() {return NewControl<FluidUnitControl>(); }  },
		{"FluidControl",[]() {return NewControl<FluidUnitControl>(); }  },
		{"GifPicControl",[]() {return NewControl<GifPicControl>(); }  },
		{"GGifPicControl",[]() {return NewControl<GifPicControl>(); }  },
		{"KeypadControl",[]() {return NewControl<KeypadControl>(); }  },
		{"BtnPilotLampControl",[]() {return NewControl<BtnPilotLampControl>(); }  },
		{"PilotLampBtnControl",[]() {return NewControl<BtnPilotLampControl>(); }  },
		{"PilotLampControl",[]() {return NewControl<PilotLampControl>(); }  },
		{"GLineControl",[]() {return NewControl<GLineControl>(); }  },
		{"GArcControl", []() {return NewControl<GArcControl>(); }  },
		{"GCircleControl",[]() {return NewControl<GCircleControl>(); }  },
		{"GRectangleControl",[]() {return NewControl<GRectangleControl>(); }  },
		{"StaticStringControl",[]() {return NewControl<StaticStringControl>(); }  },
		{"DigitalDisplayControl",[]() {return NewControl<DigitalDisplayControl>(); }  },
		{"DigitalInputControl",[]() {return NewControl<DigitalInputControl>(); }  },
		{"KeyBtnControl",[]() {return NewControl<KeyBtnControl>(); }  },
		{"CharacterDisplayControl",[]() {return NewControl<CharacterDisplayControl>(); }  },
		{"TrendChartControl",[]() {return NewControl<TrendChartControl>(); }  },
		{"MultiStatusKeypadControl",[]() {return NewControl<MultiStatusKeypadControl>(); }  },
		{"MultiPilotLampControl",[]() {return NewControl<MultiPilotLampControl>(); }  },
		{"VarStringControl",[]() {return NewControl<VarStringControl>(); }  },
		{"CharacterInputControl",[]() {return NewControl<CharacterInputControl>(); }  },
		{"ChineseDisplayControl",[]() {return NewControl<ChineseDisplayControl>(); }  },
		{"PinYinPageControl",[]() {return NewControl<PinYinPageControl>(); }  },
		{"ChineseInputControl",[]() {return NewControl<ChineseInputControl>(); }  },
		{"ReportFormControl",[]() {return NewControl<ReportFormControl>(); }  },
		{"ReportControl",[]() {return NewControl<ReportFormControl>(); }  },
		{"XYLineChartControl",[]() {return NewControl<XYLineChartControl>(); }  },
		{"AlarmDisplayControl",[]() {return NewControl<AlarmDisplayControl>(); }  },
		{"AlarmDisPlayControl",[]() {return NewControl<AlarmDisplayControl>(); }  },
		{"AlarmGControl",[]() {return NewControl<AlarmGControl>(); }  },
		/*{"HirePurchaseGControl",[]() {return NewControl<HirePurchaseGControl>(); }  },*/
		//{"RecipeGControl",[]() {return NewControl<RecipeGControl>(); }  },
		{"RecipeChartControl",[]() {return NewControl<RecipeChartControl>(); }  },
		{"RecipeTableControl",[]() {return NewControl<RecipeChartControl>(); }  },
		{"RecipeTableControl",[]() {return NewControl<RecipeChartControl>(); }  },
		{"BtnFunctionControl",[]() {return NewControl<BtnFunctionControl>(); }  },
		{"FunctionKeyControl",[]() {return NewControl<BtnFunctionControl>(); }  },
		{"FunctionDomainControl",[]() {return NewControl<RegionFunctionControl>(); }  },
		{"XYTrendControl",[]() {return NewControl<XYTrendControl>(); }  },
		{"XYTrendChartControl",[]() {return NewControl<XYTrendControl>(); }  },
		{"AdvanceFuncGControl",[]() {return NewControl<AdvanceFuncGControl>(); }  },
		{"DataTableControl",[]() {return NewControl<DataTableControl>(); }  },
		{"FanControl",[]() {return NewControl<FanControl>(); }  },
		{"TimerControl",[]() {return NewControl<TimerControl>(); }  },
		{"BackLightControl",[]() {return NewControl<BackLightControl>(); }  },
		{"SysSetGControl",[]() {return NewControl<SysSetGControl>(); }  },
		{ "MacroMapControl",[]() {return NewControl<GFuncControl>(); } },
		{"StirrerControl", []() {return NewControl<StirrerControl>(); }},
		{"PieChartControl", []() {return NewControl<PieChartControl>(); }},
		{"WaterPumpControl", []() {return NewControl<WaterPumpControl>(); }},
		{"ScaleControl", []() {return NewControl<ScaleControl>(); }},
		{"MotorControl", []() {return NewControl<MotorControl>(); }},
		
		{"SampleGCtrl", []() {return NewControl<SampleGCtrl>(); }  },
		{"DashboardControl",[]() {return NewControl<DashboardControl>(); }  },
		{"BuzzerControl",[]() {return NewControl<BuzzerControl>(); }  },
		{"BarCodeControl",[]() {return NewControl<BarCodeControl>(); }  },
		{"AlarmBarControl",[]() {return NewControl<AlarmBarControl>(); }  },
		{"AlarmGControl",[]() {return NewControl<AlarmGControl>(); } },
	/*	{ "HirePurchaseGControl",[]() {return NewControl<HirePurchaseGControl>(); } },*/
		{"OperatorGControl",[]() {return NewControl<OperatorGControl>(); } },
		{"RecipeTransferControl",[]() {return NewControl<RecipeTransferControl>(); } },
		{"OperationRecordDisplayControl",[]() {return NewControl<OperatorChartControl>(); }  },
	};
	std::unique_ptr<BaseControl> ControlFactory::GetNewControlByNameEx(string name)
	{
		if (newControlFuncMap.count(name))
			return newControlFuncMap[name]();
		return nullptr;
	}
	std::unique_ptr<BaseControl> ControlFactory::GetNewControlByName(string name)
	{
		return GetNewControlByNameEx(name);
	}

	std::unique_ptr<BaseGControl> ControlFactory::GetNewGControlByID(int ctrlEm)
	{
		if (newGControlFuncMap.count(ctrlEm))
			return newGControlFuncMap[ctrlEm]();
		return nullptr;
	}

	std::unique_ptr<BaseGControl> ControlFactory::NewGControlByID(int ctrlEm)
	{
		if (newGControlFuncMap.count(ctrlEm))
			return newGControlFuncMap[ctrlEm]();
		return nullptr;
	}
}
