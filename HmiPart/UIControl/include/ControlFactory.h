#pragma once
#include "IControlFactory.h"
namespace UI
{
	class BaseControl;
	class BaseGControl;
	class ControlFactory : public IControlFactory
	{
	public:
		static std::unique_ptr<BaseControl> GetNewControlByNameEx(string name);
		std::unique_ptr<BaseControl> GetNewControlByName(string name);
		std::unique_ptr<BaseGControl> GetNewGControlByID(int ctrlEm);
		static std::unique_ptr<BaseGControl> NewGControlByID(int ctrlEm);
	private:

	};
}


