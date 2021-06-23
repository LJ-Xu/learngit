#pragma once
#include "IPageFactory.h"
namespace UI
{
	class PageFactory : public IPageFactory
	{
	public:
		std::unique_ptr<UI::HMIPage> NewPage(short catogray, int pageid, int x, int y, int w, int h, const char* title=0);
	private:

	};
}
