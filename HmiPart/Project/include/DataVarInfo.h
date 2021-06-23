#pragma once
#include "rapidjson/document.h"
#include <string>
#include "BaseVar.h"
#include "CtrlID.h"
#include "rapidjson/filereadstream.h"
#include "CerealCommon.hpp"
namespace Project
{
	struct DataVarId
	{
		static DataVarId NullId;
		enum CollectType
		{
			Inner, Outer
		};

#define NULL_VID_VALUE 0x7fffffff

		unsigned int Vid;
		DataVarId() { Vid = NULL_VID_VALUE; }
		DataVarId(int id)
		{
			Vid = id;
		}
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Vid);
		}
		bool Cmp(DataVarId & varid)
		{
			if (Vid == varid.Vid)
				return true;
			return false;
		}
		bool IsNegative()
		{
			return Vid >>31;
		}
		DataVarId Abs()
		{
			return DataVarId(Vid^(1<<31));
		}
		bool operator==(const DataVarId& v)
		{
			return Vid ==v.Vid;
		}
		bool operator!=(const DataVarId& v)
		{
			return Vid != v.Vid;
		}
		bool operator>(const DataVarId& v)
		{
			return Vid > v.Vid;
		}
		bool operator<(const DataVarId& v)
		{
			return Vid < v.Vid;
		}

		void Parse(rapidjson::Value& jsonObj);
		static void Parse(std::vector<DataVarId>& vector, rapidjson::Value& jsonObj)
		{
			std::vector<DataVarId>().swap(vector);
			for (unsigned i = 0; i < jsonObj.Size(); i++)
			{
				DataVarId* Id = new DataVarId();
				Id->Parse(jsonObj[i]);
				vector.push_back(*Id);
			}
		}
		static bool Cmp(std::vector<DataVarId>& vector, DataVarId & varid)
		{
			for (unsigned i = 0; i < vector.size(); i++)
			{
				if (vector[i].Cmp(varid))
					return true;
			}
			return false;
		}

		
	};
	struct UseVarId
	{
		DataVarId VarId;
		bool IsNull;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(VarId, IsNull);
		}

		void Parse(rapidjson::Value& jsonObj)
		{
			if (jsonObj.HasMember("VarID") && jsonObj["VarID"].IsObject())
			{
				if (jsonObj["VarID"].HasMember("VID"))
					VarId.Vid = jsonObj["VarID"]["VID"].GetInt();
				if (jsonObj["VarID"].HasMember("IsNull"))
					IsNull = jsonObj["VarID"]["IsNull"].GetBool();
			}
		}

	};
	//typedef int IndirectIdx;
	struct DataVarDataUnio
	{
		long long value;
		struct
		{
			char* Str;
			int Len;
		}StringParam;
		struct
		{
			char* Buf;
			int  Len;
		}BytesParam;

	};
	struct IndirectVarExInfo;

	

	
	/*
	struct CtrlID
	{
		int id;
		CtrlID() {};
		CtrlID(int id) { this->id = id; };
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(id);o
		}
	};*/

	struct DataVarInfo  : public BaseVar
	{
		
		//short Winno;  //��������ID
		WinCtrlID CtrlId;   //�ؼ�ID
		/*********���ݻ�������ָ��***********/
		int Data; //���������ݴ���4�ֽڵ�ʱ��Ϊָ��
		/*******************/
		DataVarId Sub;//�Ӽ�����Ҫ���ںϲ��غϵ�DataVarInfo ,,��д�����������ӽڵ�
		DataVarId FNext;//������֡�߼���֡��Ԫ��֮������������֮֡�������
		/**********���ڼ��ָ����֪ͨ����************/
		DataVarId IndctVid;      //varID --���ָ������		
		int RawAddr;//���ָ��������ԭʼ  >=0�����ָ������
		/**********END************/
		short  DataExType;
		char   DataFlag; //0:���ݻ�δͬ������  1��������ͬ������
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(DevId,StaNo, RegType, Data, DataType,Addr, Addr1,Count,CtrlId, Sub, FNext, IndctVid,RawAddr, DataExType);
		}


		/************��������**************/
		int GetWinno()
		{
			return GetWinNo(CtrlId);
		}
		bool IsIndirectVar()
		{
			if (IndctVid.Vid != NULL_VID_VALUE && RawAddr >= 0)
				return true;
			else
				return false;
		}
		bool IsFake()
		{
			return CtrlId==-1;
		}
		bool MakeFake()
		{
			return CtrlId = -1;
		}
 
		bool IsBottomVar()
		{
			//return sub==DataVarId::NullId || sub.vid < 0;
			return Sub.IsNegative();
		}
		bool GetTopVarIdByBottom(DataVarId& varId)
		{
			if (IsBottomVar())
			{
				varId.Vid = Sub.Vid&NULL_VID_VALUE;
				return true;
			}				
			else
				return false;
		}
		 
/*
		bool operator>(const DataVarInfo& val)
		{
			if (devId > val.devId)
				return true;
			if (devId == val.devId)
			{
				if (staNo > val.staNo)
					return true;
				if (staNo == val.staNo)
				{
					if (regType > val.regType)
						return true;
 
				}
			}
			return false;
		}
		bool operator<(const DataVarInfo& val)
		{
			if (devId < val.devId)
				return true;
			if (devId == val.devId)
			{
				if (staNo < val.staNo)
					return true;
				if (staNo == val.staNo)
				{
					if (regType < val.regType)
						return true;
 
				}
			}
			return false;
		}
		bool operator!=(const DataVarInfo& val)
		{

		}*/
	};
}
