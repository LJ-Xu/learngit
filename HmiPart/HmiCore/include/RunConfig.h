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
		bool IsDebug; //�Ƿ�Ϊdebug��ʽ����
	};
}
