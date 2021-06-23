#pragma once
#include "AlarmStorage.h"
#include "OperatorStorage.h"
#include "SampleStorage.h"
#include "IDataService.h"
namespace UI
{
	class UIStorage
	{
	public:
		static void Alarm_Add();
		static void History_Add(Storage::RecordEnum rcdEM, Project::DataVarId varid,
					Project::WinCtrlID ctrlid, DDWORD oldData);
		static void Sample_Add();
	};
}