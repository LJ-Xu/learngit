#pragma once
#include <string>
#include "stdafx.h"
#include "BaseRecord.h"
namespace Storage
{
	struct SampleRecord:BaseRecord
	{
		int Channel;		// 采集通道编号
		DDWORD Data;		// 采集数据内容
		XJDataType Type;	// 数据类型信息
		DDWORD Date;		// 采集数据时间
	};

}