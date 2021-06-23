#pragma once
#include "DataVarInfo.h"
 
namespace UI
{
	class Observer
	{
	public:
		virtual void Update(Project::DataVarId id) = 0;
		 
	};
}