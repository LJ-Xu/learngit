#pragma once
#include <string>
#include "stdafx.h"
#include "BaseRecord.h"
namespace Storage
{
	struct SampleRecord:BaseRecord
	{
		int Channel;		// �ɼ�ͨ�����
		DDWORD Data;		// �ɼ���������
		XJDataType Type;	// ����������Ϣ
		DDWORD Date;		// �ɼ�����ʱ��
	};

}