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
		int FmtVer;//��ʽ�汾��
		int PCVer;//��λ��������汾
		std::string Author;
		std::string Name;
		std::string Commit;
		std::string CTime;//����ʱ��
		std::string LastTime;//�ϴ��޸�ʱ��
		std::string CompileTime;//����ʱ��
		std::string Screenmodel;//�ͺ�
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
 

