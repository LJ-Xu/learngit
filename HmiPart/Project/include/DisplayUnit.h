#pragma once
#include "Permission.h"
#include "BaseDisplayUnit.h"
#include "CerealCommon.hpp"

namespace Project
{
	class DisplayUnit :
		public BaseDisplayUnit
	{
	public:
		Permission Perm;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Perm);
		}

	};
}
