#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "ProjectPages.h"
#include "CerealCommon.hpp"
namespace Project
{
	struct ProjectInfo
	{
		int FmtVer;//格式版本号
		int PCVer;//上位机的软件版本
		std::string Author;
		std::string Name;
		std::string Commit;
		std::string CTime;//创建时间
		std::string LastTime;//上次修改时间
		std::string CompileTime;//编译时间
		std::string Screenmodel;//型号
		Size DrawBoardSize;
		

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(FmtVer, PCVer, Author, Name, Commit, CTime,
				LastTime, CompileTime, Screenmodel, DrawBoardSize);
		}
		void InitData(std::string jstr);

	};
}
 

