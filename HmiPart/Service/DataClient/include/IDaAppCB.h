#pragma once
namespace HMI
{
	class IDaAppCB
	{
	public:
		virtual void ReadCB(int id, char* buf, int len) {}
		virtual void FrameReadCB(int id, int eid, char* buf, int len) {}
	};
}