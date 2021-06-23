#include "IPermission.h"
namespace UI
{
	IPermission* IPermission::ins_;
	IPermission* IPermission::Ins()
	{
		return ins_;
	}
	void IPermission::SetIns(IPermission* ins)
	{
		ins_ = ins;
	}

}
