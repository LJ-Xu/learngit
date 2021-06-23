#pragma once
#include "CerealCommon.hpp"
namespace Project
{
	using namespace std;
	union ResItemParam
	{
		struct
		{
			short Dep;//λ��
			short W; //���
			short H; //�߶�
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
		char FileType;//�ļ����
		int Size;//��Դ�Ĵ�С
		int Offset;//����ƫ�Ƶĵ�ַ
		char Param[sizeof(ResItemParam)];
		//��Դ�Ĳ���

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
		unsigned int Data;//������Դ���ݵ�ָ��
		int Size;//��Դ���ݵ��ܴ�С
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