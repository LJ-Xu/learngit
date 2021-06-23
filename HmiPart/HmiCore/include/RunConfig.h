#pragma once
namespace Core
{
	struct RunConfig
	{
		RunConfig() {}
		RunConfig(const char* path)
		{
			Path = path;
		}
		const char* Path;
		bool IsDebug; //是否为debug方式运行
	};
}
