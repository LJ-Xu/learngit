#pragma once
#include "IStorageService.h"

namespace StorageService
{
	class DataServiceFactory
	{
	public:
		Hmi::StorageService::IStorageService* GetStorageService();
	};
}

