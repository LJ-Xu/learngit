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

		/*************��������*****************/
		INT16 regType; //����  0:PSW PFW    
		UINT8 dataType; //INT64 UINT64  DOUBLE FLOAT INT32 UINT32 INT16 UINT16 INT8 
		UINT8 level; // 
		INT32  count; //WORD������,ΪRT_STRʱ��ʾ�ֵ�����
		INT32  rawAddr; //���ָ����Ҫ֪��ԭʼ�ĵ�ַ

		BaseVar* offset;//�Ƿ���ָ��	
		function<BOOL(BaseVar*)>cb;//�����첽�����ص�ʹ��
		void* hwd;//��֪ͨ��������
	//#ifdef DATA_PROCESS
		DataVarInfo* container;//�ص�������var�������������
		DataVarInfo* prev;//��һ��
		DataVarInfo* next;//��һ��
	//#endif
		int GetIntValue();

		template <typename T>
		T GetNumberValue(); //��ȡ��ֵ
		
		BYTE* GetStringValue(); //��ȡ�ַ���

		template <typename T>
		void SetNumberValue(T t); //������ֵ

		void SetStringValue(BYTE* data, INT32 len); //������ֵ

		/**************************************************
			 ���ݵĸ��ķŵ�UI�̣߳����ݶ�ȡʹ��Ҳ��UI�߳�
		 1.���ݱ仯����֪ͨ��hwdָ��Ŀؼ���û��hwd˵��û�а󶨵�����,��ʱӦ��ʹ��ͬ�����ķ�ʽ���ı���
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
	// �����ָ���ı���
	class VarDataVar : public DataVarInfo
	{
	public:
		INT32  rawAddr; //ԭʼ��ƫ�Ƶ�ַ
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
			// ��ȫһ���ľͺϲ���һ��
			if (IS_CONTAINER(this))
			{
				//�ǰ����ܶ���VAR
				data = outData.load().numData;
				t = *(T*)&data;
			}
			else
			{
				//�Ǳ�������VAR
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
		//���ݱ��֪ͨ���ɶ�����
		//����DataService�ķ�ָ��˴�����ͬ��Ҳ�����첽 ,��֮��Ӧ���������Ȼ��ͨ����set����֪ͨ
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

