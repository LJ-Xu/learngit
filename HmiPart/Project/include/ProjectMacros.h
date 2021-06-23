#pragma once
#include "CerealCommon.hpp"
namespace Project
{
	using namespace std;
	struct PrjMacro
	{
		string type;// "C" "Python"

		string name; //函数名

		string context;//函数内容


		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(type, name, context);
		}
	};
	struct ProjectMacros
	{
		map<string, PrjMacro> macros;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(macros);
		}
		void InitData(std::string jstr);

	};
}