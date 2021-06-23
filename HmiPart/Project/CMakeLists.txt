#aux_source_directory(./src src_files)
#add_library(Project STATIC ${src_files})

add_library(Project STATIC 
	src/AlarmInfoRes.cpp
	src/BaseBuilderCls.cpp
	src/BtnFunctionRes.cpp
	src/DataVarInfo.cpp
	src/HMIProject.cpp
	src/ImageResId.cpp
	src/ProjectDataTables.cpp
	src/ProjectInfo.cpp
	src/ProjectMacros.cpp
	src/ProjectPages.cpp
	src/ProjectPortVarsInfo.cpp
	src/ProjectResources.cpp
	src/ProjectSetting.cpp
	src/SampleInfoRes.cpp
	src/StatusRes.cpp
	src/StringStyle.cpp
	src/TrendChannel.cpp
	src/ReportChannel.cpp
	src/NoticesAction.cpp
	src/ScaleInfo.cpp
	src/XYLineChannel.cpp
	src/RecipeInfoRes.cpp
	src/AdvanceFuncRes.cpp
	src/DataTableUnit.cpp
	src/ProjectDevice.cpp
	src/ProjectSystemSet.cpp
	src/OperatorChartConfig.cpp
	src/OperatorRecordConfig.cpp
	src/JudgeCondition.cpp

	
	)
