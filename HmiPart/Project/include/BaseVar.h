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
	struct DataState  //����8�ֽڶ���
	{
		int Tick; //�����ϴθ��µ�ʱ��
		VarStatusCode Code; //��ȡ���ݷ��ص�״̬�� ͬ�� �첽 ����  
		char Priority; //���ȼ�
		char Repeat;// �Ƿ�һ���Ի����ظ���ȡ
		char Cache; //�Ƿ񻺴�
	};
	struct DataBuffer
	{
		char* Pbuf;//����  ָ�� PSW   buf��ַƫ��   8�ֽ�����	
		int   Size;
	};
	union OutData
	{
		DataBuffer DataBuf; //�����������
		long long  NumData;
	};
	struct DataInfo
	{
		union OutData OutData;
		BaseVar* Var;
	};

	struct BaseVar
	{ 
		short  DevId; // �豸ID������
		short  StaNo; //վ��
		short  RegType; //�Ĵ��� 
		short  DataType; //�Ĵ�������   BIT WORD E3BYTES DDWORD E5BYTES
		// dataType==BIT ��ʱ��CountΪλ��ƫ��ֵ��Ϊ����ʱ��ʾ�ֽڸ���
		short  Addr1; 
		short  Count; //�ֽ�����  ->����regType��ȡ�����Ĵ����ֽ���
		int    Addr;  //��ַƫ�� 
		
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
		VarDataType DataType;			//��������
		VarNumberType DataFmt;			//���ݸ�ʽ
		int RegCount;					//�Ĵ�������
		std::string ColTitle;			//����

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(DataType, DataFmt, RegCount, ColTitle);
		}
	};
}
