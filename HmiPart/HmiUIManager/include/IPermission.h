#pragma once
#include "stdafx.h"

//#define HMI_PC_RENDER
namespace Project
{
	struct HMIProject;
}
namespace UI
{
	struct SysUser
	{
		string Name;//用户名称
		int Region;//用户权限范围

	};
	class IPermission
	{
	public:
		static IPermission* Ins();
		static void SetIns(IPermission*);
		virtual bool UserLogin(string uname, string passwd)=0;
		virtual bool HasPerm(int requestRegion) = 0;
		virtual SysUser* CurUser() = 0;
	private:
		static IPermission* ins_;
	};

}