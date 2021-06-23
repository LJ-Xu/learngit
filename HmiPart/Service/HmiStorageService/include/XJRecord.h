#pragma once
#include "BaseRecord.h"
#include "AlarmRecord.h"
#include "OperatorRecord.h"
#include "SampleRecord.h"
namespace UI
{

	class XJRecord
	{
	public:
		void MakeSample(Storage::SampleRecord& recd, int Channel, XJData& data);
		void MakeAlarm(Storage::AlarmRecord& recd, int id);
		void MakeOperator(Storage::OperatorRecord& recd, Storage::RecordEnum rectp,
			Project::DataVarId vid, Project::WinCtrlID ctrlid, DDWORD& d1, DDWORD& d2);
	};

}


