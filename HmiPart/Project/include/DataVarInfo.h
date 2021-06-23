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
		
		//short Winno;  //所属窗体ID
		WinCtrlID CtrlId;   //控件ID
		/*********数据或者数据指针***********/
		int Data; //当数据内容大于4字节的时候为指针
		/*******************/
		DataVarId Sub;//子集，主要用于合并重合的DataVarInfo ,,填写负数代表无子节点
		DataVarId FNext;//用于组帧逻辑：帧的元素之间链表，，非组帧之间的链表
		/**********用于间接指定的通知功能************/
		DataVarId IndctVid;      //varID --间接指定所用		
		int RawAddr;//间接指定变量的原始  >=0：间接指定变量
		/**********END************/
		short  DataExType;
		char   DataFlag; //0:数据还未同步进来  1：数据已同步进来
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(DevId,StaNo, RegType, Data, DataType,Addr, Addr1,Count,CtrlId, Sub, FNext, IndctVid,RawAddr, DataExType);
		}


		/************辅助函数**************/
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
