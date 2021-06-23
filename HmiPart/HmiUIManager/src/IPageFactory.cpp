#include "IPageFactory.h"
namespace UI
{
	IPageFactory* IPageFactory::ins_;
	IPageFactory* IPageFactory::Ins()
	{
		return ins_;
	}
	void IPageFactory::SetIns(IPageFactory* ins)
	{
		ins_ = ins;
	}
}
