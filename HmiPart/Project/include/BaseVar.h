#pragma once
#include <mutex>
#include <atomic>
#include <string.h>
#include "CtrlID.h"

namespace Project
{
	
	enum LocalRegType :char
	{
		TP_PSB = 0, TP_SPSB, TP_PSW, TP_PFW, TP_SPSW, TP_SPFW, TP_COUNT
	};
	enum LocalRegDataType :short
	{
		RTP_BIT, RTP_WORD
	};
	enum VarStatusCode :char
	{
		VarState_OK, VarState_WAITING
	};
	enum VarDataType : short
	{
		DT_Bit = 1,
		DT_Byte = 2,
		DT_WORD = 4,
		DT_DWORD = 8,
		DT_DDWORD = 16,
		DT_String = 32,
		DT_Bytes = 64,
		DT_WORDS = 128,
		DT_DWORDS = 256,
		DT_DDWORDS = 512,
	};
	enum VarNumberType : char
	{
		NT_BCD,
		NT_Hex,
		NT_Signed,
		NT_Unsigned,
		NT_Float
	};
	enum RecipeAction { RecipeDelete, RecipeInsert, RecipeSet, DBToPLC, RecipeDown	};
	struct BaseVar;
	struct DataState  //必须8字节对齐
	{
		int Tick; //数据上次更新的时间
		VarStatusCode Code; //读取数据返回的状态码 同步 异步 错误  
		char Priority; //优先级
		char Repeat;// 是否一次性还是重复获取
		char Cache; //是否缓存
	};
	struct DataBuffer
	{
		char* Pbuf;//数据  指向 PSW   buf地址偏移   8字节以内	
		int   Size;
	};
	union OutData
	{
		DataBuffer DataBuf; //输出数据内容
		long long  NumData;
	};
	struct DataInfo
	{
		union OutData OutData;
		BaseVar* Var;
	};

	struct BaseVar
	{ 
		short  DevId; // 设备ID索引号
		short  StaNo; //站号
		short  RegType; //寄存器 
		short  DataType; //寄存器类型   BIT WORD E3BYTES DDWORD E5BYTES
		// dataType==BIT 的时候，Count为位的偏移值，为其他时表示字节个数
		short  Addr1; 
		short  Count; //字节数量  ->根据regType获取单个寄存器字节数
		int    Addr;  //地址偏移 
		
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(DevId, StaNo, RegType, DataType, Addr1, Count, Addr);
		}
		bool   IsInnerVar()
		{
			return DevId==0;
		}
		bool   IsSysRegVar()
		{
			return (DevId == 0 && (RegType == Project::TP_SPSW ||
				RegType == Project::TP_SPFW ||
				RegType == Project::TP_SPSB));
		}
		int GetBufSize()
		{
			if (DataType == DT_Bit)
				return (Count + 7) / 8;
			return Count;
		}
 
	};
	class ColDataTypeInfo
	{
	public:
		VarDataType DataType;			//数据类型
		VarNumberType DataFmt;			//数据格式
		int RegCount;					//寄存器个数
		std::string ColTitle;			//列名

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(DataType, DataFmt, RegCount, ColTitle);
		}
	};
}
