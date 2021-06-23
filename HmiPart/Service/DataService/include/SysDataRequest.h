#pragma once
#include "DataFrame.h"
namespace Project
{
	struct BaseVar;
}
namespace UI
{
	
	class SysDataRequest
	{
	public:
		static void Init();
		static bool IsInnerReq(DataRequest& daReq);
		static int  Get(DataRequest&);
		static int  CheckProcess(DataRequest&);
		static bool  ProcessFrameCB(int id,
			int eid,
			char* buf,
			int len);
	};
}