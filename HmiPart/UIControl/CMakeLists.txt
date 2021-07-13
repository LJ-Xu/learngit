#aux_source_directory(./src src_files)
#aux_source_directory( ./src/Data src_files)
#aux_source_directory( ./src/GCtrl src_files)
#aux_source_directory( ./src/Macro src_files)
#aux_source_directory( ./src/Parts src_files)
#aux_source_directory( ./src/Service src_files)
#aux_source_directory( ./src/Storage src_files)
#aux_source_directory( ./src/TestDemo src_files)
#aux_source_directory( ./src/Utility src_files)
#aux_source_directory( ./src/Graphic src_files)


#list(REMOVE_ITEM src_files ./src/Parts/AlarmWinControl.cpp)
#list(REMOVE_ITEM src_files ./src/Parts/BtnTriggleView.cpp)
#list(REMOVE_ITEM src_files ./src/Parts/BtnTriggleControl.cpp)
#list(REMOVE_ITEM src_files ./src/Parts/BtnTriggleModel.cpp)
#list(REMOVE_ITEM src_files ./src/Parts/ChineseBtnControl.cpp)
#list(REMOVE_ITEM src_files ./src/Parts/ChineseBtnModel.cpp)
#list(REMOVE_ITEM src_files ./src/Parts/ChineseBtnView.cpp)
##list(REMOVE_ITEM src_files ./src/Parts/AlarmBarControl.cpp)
##list(REMOVE_ITEM src_files ./src/Parts/AlarmBarModel.cpp)
##list(REMOVE_ITEM src_files ./src/Parts/AlarmBarView.cpp)
#list(REMOVE_ITEM src_files ./src/Parts/AlarmInputControl.cpp)
#list(REMOVE_ITEM src_files ./src/Parts/AlarmInputModel.cpp)
#list(REMOVE_ITEM src_files ./src/Parts/AlarmInputView.cpp)
#list(REMOVE_ITEM src_files ./src/Parts/DataSampleControl.cpp)
#list(REMOVE_ITEM src_files ./src/Parts/DataSampleModel.cpp)
#list(REMOVE_ITEM src_files ./src/Parts/DataSampleView.cpp)
#list(REMOVE_ITEM src_files ./src/Parts/KeyBoardControl.cpp)
#list(REMOVE_ITEM src_files ./src/Parts/KeyBoardModel.cpp)
#list(REMOVE_ITEM src_files ./src/Parts/KeyBoardView.cpp)
#list(REMOVE_ITEM src_files ./src/Parts/KeyMsgControl.cpp)
#list(REMOVE_ITEM src_files ./src/Parts/KeyMsgModel.cpp)
#list(REMOVE_ITEM src_files ./src/Parts/KeyMsgView.cpp)
#list(REMOVE_ITEM src_files ./src/Parts/DashboardView.cpp)
#list(REMOVE_ITEM src_files ./src/Parts/DashboardControl.cpp)
#list(REMOVE_ITEM src_files ./src/Parts/DashboardModel.cpp)
##list(REMOVE_ITEM src_files ./src/Parts/PinYinPageControl.cpp)
#list(REMOVE_ITEM src_files ./src/Graphic/GTestView.cpp)
#list(REMOVE_ITEM src_files ./src/Graphic/GScaleControl.cpp)
#list(REMOVE_ITEM src_files ./src/Graphic/GTestControl.cpp)
#list(REMOVE_ITEM src_files ./src/Graphic/GTestModel.cpp)
#list(REMOVE_ITEM src_files ./src/Graphic/GLabelControl.cpp)


#list(REMOVE_ITEM src_files ./src/Utility/PicTool.cpp)
##list(REMOVE_ITEM src_files ./src/Utility/CodeFormatUtility.cpp)


#add_library(UIControl STATIC ${src_files})
add_library(UIControl STATIC 
	src/PageFactory.cpp
	src/ControlFactory.cpp
	src/Data/UIData.cpp
	src/GCtrl/AlarmGControl.cpp
	src/GCtrl/AlarmGModel.cpp
	src/GCtrl/AlarmGView.cpp
	src/GCtrl/SampleGModel.cpp
	src/GCtrl/AdvanceFuncGControl.cpp
	src/GCtrl/AdvanceFuncGModel.cpp
	src/GCtrl/PrintSetControl.cpp
	src/GCtrl/PrintSetModel.cpp
	src/GCtrl/OperatorGControl.cpp
	src/GCtrl/OperatorGModel.cpp
	src/GCtrl/SampleGCtrl.cpp
	src/GCtrl/SysSetGControl.cpp
	src/GCtrl/SysSetGModel.cpp
	src/Graphic/GArcControl.cpp
	src/Graphic/GArcModel.cpp
	src/Graphic/GArcView.cpp
	src/Graphic/GCircleControl.cpp
	src/Graphic/GCircleModel.cpp
	src/Graphic/GCircleView.cpp
	src/Graphic/GLineControl.cpp
	src/Graphic/GLineModel.cpp
	src/Graphic/GLineView.cpp
	src/Graphic/GRectangleControl.cpp
	src/Graphic/GRectangleModel.cpp
	src/Graphic/GRectangleView.cpp
	src/Graphic/BarCodeView.cpp
	src/Graphic/BarCodeModel.cpp
	src/Graphic/BarCodeControl.cpp
	src/Graphic/GFuncModel.cpp
	src/Graphic/GFuncView.cpp
	src/Graphic/GFuncControl.cpp
	src/Graphic/GDynPicControl.cpp
	src/Graphic/GDynPicModel.cpp
	src/Graphic/GDynPicView.cpp
	src/Graphic/GPolyLineControl.cpp
	src/Graphic/GPolyLineModel.cpp
	src/Graphic/GPolyLineView.cpp
	src/Graphic/GStickChartModel.cpp
	src/Graphic/GStickChartView.cpp
	src/Graphic/GStickChartControl.cpp
	src/Macro/UIApi.cpp
	src/Macro/UIServer.cpp
	src/Macro/UICommand.cpp
	src/Parts/DataTableView.cpp
	src/Parts/DataTableControl.cpp
	src/Parts/DataTableModel.cpp
	src/Parts/ValveView.cpp
	src/Parts/ValveControl.cpp
	src/Parts/ValveModel.cpp
	src/Parts/StirrerControl.cpp
	src/Parts/StirrerModel.cpp
	src/Parts/StirrerView.cpp
	src/Parts/SlideInputControl.cpp
	src/Parts/SlideInputModel.cpp
	src/Parts/SlideInputView.cpp
	src/Parts/PieChartControl.cpp
	src/Parts/PieChartModel.cpp
	src/Parts/PieChartView.cpp
	src/Parts/WaterPumpModel.cpp
	src/Parts/WaterPumpView.cpp
	src/Parts/WaterPumpControl.cpp
	src/Parts/ClockControl.cpp
	src/Parts/ClockModel.cpp
	src/Parts/ClockView.cpp
	src/Parts/SlideInputControl.cpp
	src/Parts/SlideInputModel.cpp
	src/Parts/SlideInputView.cpp
	src/Parts/DateControl.cpp
	src/Parts/DateModel.cpp
	src/Parts/DateView.cpp
	src/Parts/BtnPilotLampControl.cpp
	src/Parts/BtnPilotLampModel.cpp
	src/Parts/BtnPilotLampView.cpp
	src/Parts/DigitalDisplayControl.cpp
	src/Parts/DigitalDisplayModel.cpp
	src/Parts/DigitalDisplayView.cpp
	src/Parts/FluidUnitControl.cpp
	src/Parts/FluidUnitModel.cpp
	src/Parts/FluidUnitView.cpp
	src/Parts/GifPicControl.cpp
	src/Parts/GifPicModel.cpp
	src/Parts/GifPicView.cpp
	src/Parts/KeypadControl.cpp
	src/Parts/KeypadModel.cpp
	src/Parts/KeypadView.cpp
	src/Parts/PilotLampControl.cpp
	src/Parts/PilotLampModel.cpp
	src/Parts/PilotLampView.cpp
	src/Parts/StaticStringControl.cpp
	src/Parts/StaticStringModel.cpp
	src/Parts/StaticStringView.cpp
	src/Parts/KeyBtnControl.cpp
	src/Parts/KeyBtnModel.cpp
	src/Parts/KeyBtnView.cpp
	src/Parts/DigitalInputControl.cpp
	src/Parts/DigitalInputModel.cpp
	src/Parts/DigitalInputView.cpp
	src/Parts/CharacterDisplayControl.cpp
	src/Parts/CharacterDisplayModel.cpp
	src/Parts/CharacterDisplayView.cpp
	src/Parts/ScaleView.cpp
	src/Parts/ScaleControl.cpp
	src/Parts/ScaleModel.cpp
	src/Parts/TrendChartControl.cpp
	src/Parts/TrendChartModel.cpp
	src/Parts/TrendChartView.cpp
	src/Parts/MultiStatusKeypadControl.cpp
	src/Parts/MultiStatusKeypadModel.cpp
	src/Parts/MultiStatusKeypadView.cpp
	src/Parts/MultiPilotLampView.cpp
	src/Parts/MultiPilotLampControl.cpp
	src/Parts/MultiPilotLampModel.cpp
	src/Parts/DashboardView.cpp
	src/Parts/DashboardModel.cpp
	src/Parts/DashboardControl.cpp
	src/Parts/VarStringControl.cpp
	src/Parts/VarStringModel.cpp
	src/Parts/VarStringView.cpp
	src/Parts/CharacterInputControl.cpp
	src/Parts/CharacterInputView.cpp
	src/Parts/CharacterInputModel.cpp
	src/Parts/ChineseDisplayControl.cpp
	src/Parts/ChineseDisplayModel.cpp
	src/Parts/ChineseDisplayView.cpp
	src/Parts/ChineseInputControl.cpp
	src/Parts/ChineseInputModel.cpp
	src/Parts/ChineseInputView.cpp
	src/Parts/PinYinPageControl.cpp
	src/Parts/PinYinPageModel.cpp
	src/Parts/PinYinPageView.cpp
	src/Parts/BuzzerView.cpp
	src/Parts/BuzzerModel.cpp
	src/Parts/BuzzerControl.cpp
	src/Parts/ReportFormControl.cpp
	src/Parts/ReportFormModel.cpp
	src/Parts/ReportFormView.cpp
	src/Parts/XYLineChartControl.cpp
	src/Parts/XYLineChartModel.cpp
	src/Parts/XYLineChartView.cpp
	src/Parts/AlarmDisplayView.cpp
	src/Parts/AlarmDisplayControl.cpp
	src/Parts/AlarmDisplayModel.cpp
	src/Parts/AlarmBarView.cpp
	src/Parts/AlarmBarControl.cpp
	src/Parts/AlarmBarModel.cpp
	src/Parts/RecipeChartControl.cpp
	src/Parts/RecipeChartModel.cpp
	src/Parts/RecipeChartView.cpp
	src/Parts/BtnFunctionView.cpp
	src/Parts/BtnFunctionControl.cpp
	src/Parts/BtnFunctionModel.cpp
	src/Parts/RegionFunctionControl.cpp
	src/Parts/RegionFunctionModel.cpp
	src/Parts/RegionFunctionView.cpp
	src/Parts/RecipeTransferControl.cpp
	src/Parts/RecipeTransferModel.cpp
	src/Parts/RecipeTransferView.cpp
	src/Parts/XYTrendView.cpp
	src/Parts/XYTrendControl.cpp
	src/Parts/XYTrendModel.cpp
	src/Parts/FanControl.cpp
	src/Parts/FanModel.cpp
	src/Parts/FanView.cpp
	src/Parts/TimerControl.cpp
	src/Parts/TimerModel.cpp
	src/Parts/TimerView.cpp
	src/Parts/BackLightControl.cpp
	src/Parts/BackLightModel.cpp
	src/Parts/BackLightView.cpp
	src/Parts/OperatorChartControl.cpp
	src/Parts/OperatorChartModel.cpp
	src/Parts/OperatorChartView.cpp
	src/Parts/TableControl.cpp
	src/Parts/TableModel.cpp
	src/Parts/TableView.cpp
	src/Parts/DropMenuAdd.cpp
	src/Parts/DropMenuControl.cpp
	src/Parts/DropMenuItem.cpp
	src/Parts/DropMenuModel.cpp
	src/Parts/DropMenuUnit.cpp
	src/Parts/DropMenuView.cpp
	src/Parts/MotorControl.cpp
	src/Parts/MotorModel.cpp
	src/Parts/MotorView.cpp
	src/Parts/SlideInputControl.cpp
	src/Parts/SlideInputModel.cpp
	src/Parts/SlideInputView.cpp
	src/Service/PcResourceService.cpp
	src/Storage/UIStorage.cpp
	src/TestDemo/GTestControl.cpp
	src/TestDemo/GTestModel.cpp
	src/TestDemo/GTestView.cpp
	src/TestDemo/TestBitVarControl.cpp
	src/TestDemo/TestBitVarModel.cpp
	src/TestDemo/TestBitVarView.cpp
	src/Utility/PermUtility.cpp
	src/Utility/ViewShowUtility.cpp
	src/Utility/PermUtility.cpp
	src/Utility/ViewShowUtility.cpp
	src/Utility/RotatePic.cpp
	src/Utility/CodeFormatUtility.cpp
	src/Utility/KeyInputUtility.cpp
	src/Utility/AdvancedGraphic.cpp
	src/Utility/GraphicDrawHandle.cpp
	src/Utility/RowTable.cpp
	src/Utility/TableBase.cpp
	src/Utility/NoticesUtility.cpp
	src/Utility/gifdec.c
	src/Utility/StringUtility.cpp
	src/Utility/PermUtility.cpp
	src/Utility/ViewShowUtility.cpp
	src/Utility/SysSetApi.cpp
	src/Utility/SysSetHandle.cpp




	)

