#pragma once
using namespace std;
namespace Project
{
	class BaseBuilderCls
	{
		virtual char* Serialize() = 0;
		virtual void* Deserialize(char*) = 0;
	};

}

