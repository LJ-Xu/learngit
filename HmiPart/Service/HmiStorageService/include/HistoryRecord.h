#pragma once
#include <string>
#include "stdafx.h"
#include "ProjectPortVarsInfo.h"
#include "BaseRecord.h"
namespace Storage
{
	enum RecordEnum
	{
		HR_DATA_TOGGLE,//����
		HR_DATA_SET,  //����
		HR_WINDOW_CLOSE,//����ر�
		HR_WINDOW_OPEN, //�����
	};
	struct HistoryRecord : BaseRecord
	{
		RecordEnum RecdTp;//��¼���
		char Name[15];
		Project::DataVarId VarId;//������Ϣ
		Project::WinCtrlID WCId ;
		DDWORD val1;
		DDWORD val2;
	};

}