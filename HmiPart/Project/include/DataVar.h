#pragma once
#include "stdafx.h"
#include "BaseVar.h"
#include "DataVarInfo.h"

#if 0
namespace Project
{
	enum RegTypeEM :UINT8
	{
		RT_BIT,
		RT_INT8,
		RT_UINT8,
		RT_INT16,
		RT_UINT16,
		RT_INT32,
		RT_UINT32,
		RT_INT64,
		RT_UINT64,
		RT_FLOAT,
		RT_DOUBLE,

		RT_HEX,
		RT_BCD,

		RT_STR,
		RT_MAX,

	};
	enum PriorityEM :UINT8
	{
		PR_LOW,
		PR_NORMAL,
		PR_HIGH,

	};

	class DataVarInfo : public BaseVar
	{
	public:

		/*************参数部分*****************/
		INT16 regType; //类型  0:PSW PFW    
		UINT8 dataType; //INT64 UINT64  DOUBLE FLOAT INT32 UINT32 INT16 UINT16 INT8 
		UINT8 level; // 
		INT32  count; //WORD的数量,为RT_STR时表示字的数量
		INT32  rawAddr; //间接指定需要知道原始的地址

		BaseVar* offset;//是否间接指定	
		function<BOOL(BaseVar*)>cb;//用于异步操作回调使用
		void* hwd;//被通知的主体句柄
	//#ifdef DATA_PROCESS
		DataVarInfo* container;//重叠的区域var放在这个链表下
		DataVarInfo* prev;//上一个
		DataVarInfo* next;//下一个
	//#endif
		int GetIntValue();

		template <typename T>
		T GetNumberValue(); //获取数值
		
		BYTE* GetStringValue(); //获取字符串

		template <typename T>
		void SetNumberValue(T t); //设置数值

		void SetStringValue(BYTE* data, INT32 len); //设置数值

		/**************************************************
			 数据的更改放到UI线程，数据读取使用也在UI线程
		 1.数据变化后发送通知给hwd指向的控件，没有hwd说明没有绑定到界面,此时应该使用同步锁的方式更改变量
		 **************************************************/
		std::atomic<OutData> outData;

		void SetData(VarStatusCode code, DataInfo* dataInfo)
		{

			if (dataInfo->outData.numData != outData.load().numData)
			{
				outData.store(dataInfo->outData);
				//notify Data Change
			}
		}
		INT32 SeriaBytes(BYTE* buf)
		{
			memcpy(buf, this, sizeof(BYTE*));
			memcpy(buf + sizeof(BYTE*), this, (INT32)&((DataVarInfo*)0)->offset);
			return (INT32)&((DataVarInfo*)0)->offset + sizeof(BYTE*);
		}
	};
	// 带间接指定的变量
	class VarDataVar : public DataVarInfo
	{
	public:
		INT32  rawAddr; //原始的偏移地址
	};
#define IS_CONTAINER(var) ((var)->container&&((var)->container->addr >= (var)->addr && \
									(var)->container->count <= (var)->count))
#define IS_SUBITEM(var) ((var)->container&&((var)->container->addr < (var)->addr || \
									(var)->container->count > (var)->count))
	template<typename T>
	inline T DataVarInfo::GetNumberValue()
	{


		if (container)
		{
			INT64 data;
			T t;
			// 完全一样的就合并成一个
			if (IS_CONTAINER(this))
			{
				//是包含很多子VAR
				data = outData.load().numData;
				t = *(T*)&data;
			}
			else
			{
				//是被包含的VAR
				data = container->outData.load().numData;
				t = *(T*)(((BYTE*)&data) + (addr - container->addr));
			}
			return t;

		}
		else
		{
			T* ptr = reinterpret_cast<T>(&outData.load().numData);
			return *ptr;
		}

	}
	inline int DataVarInfo::GetIntValue()
	{
		return 1;
	}
	template<typename T>
	inline void DataVarInfo::SetNumberValue(T t)
	{
		outData.store(t);
		//数据变更通知，由读开启
		//调用DataService的发指令，此处可以同步也可以异步 ,读之后应该立马读，然后通过读set进行通知
		//DataService::Ins().
	}
	inline void DataVarInfo::SetStringValue(BYTE* data, INT32 len)
	{
		memcpy(outData.load().dataBuf.pbuf, data, len);
	}
	inline BYTE* DataVarInfo::GetStringValue()
	{
		return outData.load().dataBuf.pbuf;
	}
	struct DataVarId
	{
	public:
		int winId;
		int ctrlId;
		DataVarInfo* GetDataVarInfo()
		{
			DataVarInfo *tmp = new DataVarInfo();
			return tmp;
		}
	};
}
#endif

