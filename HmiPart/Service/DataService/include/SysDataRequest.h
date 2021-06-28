#pragma once
#include "DataFrame.h"
#include "Semaphore.h"
namespace Project
{
	struct BaseVar;
}
namespace UI
{
	enum RequestStatue:char
	{ RS_OK=0,RS_TIMEOUT=-2,RS_CONTINUE=-1,};
	class SysDataRequest
	{
	public:
		static void Init();
		static bool IsInnerReq(DataRequest& daReq);
		static RequestStatue  Get(DataRequest&);
		static int  CheckProcess(DataRequest&);
		static void ResumeTimeout();
		static bool  ProcessFrameCB(int id,
			int eid,
			char* buf,
			int len);
	private:
		
	};
}