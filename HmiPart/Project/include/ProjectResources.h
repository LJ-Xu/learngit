#pragma once
#include "CerealCommon.hpp"
namespace Project
{
	using namespace std;
	union ResItemParam
	{
		struct
		{
			short Dep;//位深
			short W; //宽度
			short H; //高度
		}Pic;
		struct
		{
			short Baud;
		}Sound;
	};
	enum ResFileType
	{
		File,Image,Sound,
	};
	struct ResItem
	{
		char FileType;//文件类别
		int Size;//资源的大小
		int Offset;//数据偏移的地址
		char Param[sizeof(ResItemParam)];
		//资源的参数

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(FileType, Size, Offset, Param);
		}
		ResItemParam* GetResItemParam()
		{
			return (ResItemParam*)Param;
		}
		
	
	};

	struct ProjectResources
	{
		map<string, int> ResKeyIdxs;
		std::vector<ResItem> ResMap;//key->value
		unsigned int Data;//所有资源数据的指针
		int Size;//资源数据的总大小
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ResMap, Data, ResKeyIdxs);
		}
		char* GetDataPtr()
		{
			return (char*)Data;
		}
		void SetData(char* data,int size)
		{
			this->Data = (unsigned int)data;
			this->Size = size;
		}
		void InitData(std::string jstr);
		ResItem* GetResourceItem(string key)
		{
			if (ResKeyIdxs.find(key) == ResKeyIdxs.end())
				return nullptr;
			return &ResMap[ResKeyIdxs[key]];
		}

		char* GetResourceData(ResItem* res)
		{
			if (!Data)
				return nullptr;
			return (char*)(Data + res->Offset);
		}
	};
}