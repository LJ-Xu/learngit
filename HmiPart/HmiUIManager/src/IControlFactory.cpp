#include "IControlFactory.h"
namespace UI
{
	IControlFactory* IControlFactory::ins_;
	IControlFactory* IControlFactory::Ins()
	{
		return ins_;
	}
	void IControlFactory::SetIns(IControlFactory* ins)
	{
		ins_ = ins;
	}

}
