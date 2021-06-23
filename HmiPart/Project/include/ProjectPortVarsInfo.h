#pragma once
#include "CerealCommon.hpp"
#include "DataVarInfo.h"
#include "ProjectDevice.h"
#include<map>
using namespace std;
namespace Project
{
 

 //
	//struct PrjDevRegVars
	//{
	//	vector<int> DirectVars;//ֱ��ָ���ı�������
	//	vector<int> IndirectVars;//��Ҫ���ָ���ı���

	//	template<class Archive>
	//	void serialize(Archive & archive)
	//	{
	//		archive(DirectVars, IndirectVars);
	//	}
	//};
	//// �洢ָ��վ���µ����мĴ�����Ϣ
	//struct PrjDevRegsInfo
	//{
	//	/*************�����ҵ���Ӧ���豸***************/
	//	char   Port;
	//	PointType Point;
	//	char StaNo;
	//	/*************������Ϣ����*****************/
	//	map<int/*�Ĵ������*/, PrjDevRegVars> Devs;

	//	template<class Archive>
	//	void serialize(Archive & archive)
	//	{
	//		archive(Port, Point, StaNo, Devs);
	//	}

	//};
	//// �˿ڶ�Ӧ���豸�ӿ�
	//struct PrjPortPoint
	//{
	//	string Name; // �豸����
	//	PortType Port;//��Ӧ�Ķ˿�
	//	PointType Point;//�ö˿��µĽӿ�   ���ؿ��µĽӿ� RS485 RS422 RS232
	//	PrjCommParam Param;//�ýӿڵ�ͨ�Ų���
	//	short ProtocolId;//�豸��ͨ��Э��Id
	//	//short staNo;//�豸վ��  
	//	map<int/*վ��*/, PrjDevRegsInfo>  Devs; //�ýӿ���վ�Ŷ�Ӧ�豸�����еļĴ���������Ϣ

	//	template<class Archive>
	//	void serialize(Archive & archive)
	//	{
	//		archive(Name, Port, Point, Param, ProtocolId, Devs);
	//	}
	//};

	//struct RegTypeInfo
	//{
	//	int Start;
	//	int End;
	//	// start == end empty

	//	template<class Archive>
	//	void serialize(Archive & archive)
	//	{
	//		archive(Start, End);
	//	}
	//};
 
	struct FrameInfo
	{
		char RISize;//RegItemSize
		BaseVar Var;//
		//int SIdx;//���������������ݵ���ʼVAR��Ϣ
		std::vector<DataVarId> VIds;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(RISize, Var, VIds);
		}
	};
	struct WindowVar
	{
		//int DirectIdx;   //ֱ��ָ������ʼID
		//int InDirectIdx; //���ָ������ʼID
		//int EIdx;        //ĩβ,����
		unsigned int BufSize;     //����4�ֽڵ�buf��С
		//DataVarId SVID;  //��ʼ������֡��vid �б�
		std::vector<FrameInfo> Frames;//֡
		std::vector<DataVarId> VIds;//������֡��ֱ�ӱ���
		//std::vector<DataVarId> InDirectIds;//������֡��ֱ�ӱ���
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(/*DirectIdx, InDirectIdx, EIdx, */BufSize, Frames, VIds);
		}
		   
	};
 
	
	struct ProjectPortVarsInfo
	{
		//// ÿ��port�¿����ж���ӿ��豸
		//std::vector<PrjPortPoint> Points; //�豸�ڵ� device ID Ϊ������
		//std::map<int, PrjPortPoint> Points;
		// datavar�Ĵ洢����  ,WINID ->DataVarInfo����
		std::map<int, WindowVar> WinVarInfos;   //������������Ի���������WindowVar�Ĺ�ϵ

		//ϵͳ������VARINFO�ṹ,����Ӧ�����е�DataVarInfo  ֱ��ָ��+���ָ��
		std::vector<DataVarInfo> VarInfos;

		//std::vector<int> VarIdxs;

		//��̬��Var��������Ҫ������ʱ�洢
		std::vector<DataVarInfo> TmpVarInfos;

		int DataCount(DataVarId vid)
		{
			if (vid.Vid<0 || vid.Vid>VarInfos.size())return 0;
			DataVarInfo& info = VarInfos[vid.Vid];
			if (info.DataType == VarDataType::DT_Bit)
				return 1;
			return info.Count;
		}


		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(WinVarInfos, VarInfos);
		}
		void InitData(std::string jstr);

		WindowVar* GetWinVar(int winno)
		{
			if (WinVarInfos.find(winno) == WinVarInfos.end())
			{
				return nullptr;
			}
			return &WinVarInfos[winno];
		}
		//int  GetVIDByIdx(int idx)
		//{
		//	//return idx;
		//	return VarIdxs[idx];
		//}
		/*******************************************************************************
		 * Name	    : GetDataVarInfo
		 * Descript : ����VarId��ȡVARINFO
		 * Input	: vid - DataVarId
		 * Output   : DataVarInfo* - ����DataVarInfo
		 * Note	    :
		 *******************************************************************************/
		
		DataVarInfo* GetDataVarInfo(int id)
		{

			if (id == Project::DataVarId::NullId.Vid || id < 0)

				return nullptr;
#if 1
			if (VarInfos.size() <= (size_t)id)
			{
				DataVarInfo* info = &TmpVarInfos[id - VarInfos.size()];
				//if (info->IsIndirectVar())
				//{
				//	int finId = 0;
				//	DataVarInfo* ptrinfo = GetDataVarInfo(info->IndctVid.Vid);
				//	if (ptrinfo)
				//	{
				//		if (info->Count <= 4)
				//		{
				//			finId = GetDataVarInfo(info->IndctVid.Vid)->Data;
				//		}
				//		else
				//		{
				//			finId = *(int*)(GetDataVarInfo(info->IndctVid.Vid)->Data);
				//		}
				//	}
				//	
				//	//info = GetDataVarInfo(id + finId);
				//	info->Addr = info->RawAddr + finId;
				//}
				return info;
			}
			else
			{
				DataVarInfo* info = &VarInfos[id];
				/*if (info->IsIndirectVar())
				{
					int finId = 0; 
					DataVarInfo* ptrinfo = GetDataVarInfo(info->IndctVid.Vid);
					if (ptrinfo)
					{
						if (info->Count <= 4)
						{
							finId = ptrinfo->Data;
						}
						else
						{
							finId = *(int*)(ptrinfo->Data);
						}
					}
					info->Addr = info->RawAddr + finId;
				}*/
				return info;
			}
#else
			if (VarInfos.size() <= id)
			{
				return &TmpVarInfos[id - VarInfos.size()];
			}
			else
			{
				return &VarInfos[id];
			}
#endif
		}
		DataVarInfo* GetDataVarInfo(DataVarId& vid)
		{

			return GetDataVarInfo(vid.Vid);
		}
		/*******************************************************************************
		 * Name	    : GetDataVarInfo
		 * Descript : ����VarId��ȡVARINFO
		 * Input	: vid - DataVarId
		 * Output   : DataVarInfo* - ����DataVarInfo
		 * Note	    :
		 *******************************************************************************/
		WinCtrlID GetCtrlID(DataVarId& vid)
		{

			DataVarInfo* info = GetDataVarInfo(vid);
			if(info)
				return info->CtrlId;
			return WinCtrlIDEmpty;

			 
		}
		/*******************************************************************************
		 * Name	    : NewDataVarInfo
		 * Descript : ����һ���µ�Var
		 * Input	: vid     - ��Ӻ󷵻ص�ID
		 *			  colType - ���Ŀǰ������ һ�����ڲ��Ĵ���  һ�����ⲿ�Ĵ���
		 *            varType - ������� һ����λ��أ�һ�������������
		 * Output   : DataVarInfo - ������ӵ�DataVarInfo
		 * Note	    :
		 *******************************************************************************/
		DataVarInfo* NewDataVarInfo(DataVarId& vid)
		{
			size_t idx;
			DataVarInfo* ptrInfo=nullptr;
			for (idx = 0; idx < TmpVarInfos.size(); idx++)
			{
				if (TmpVarInfos[idx].Count == 0)
				{
					ptrInfo = &TmpVarInfos[idx];
					break;
				}
					
			}
			if (TmpVarInfos.size() == idx)
			{				
				idx = TmpVarInfos.size();
				TmpVarInfos.push_back(DataVarInfo());
				ptrInfo = &TmpVarInfos[idx];
				idx += VarInfos.size();
			}				
			vid.Vid = idx;
			return ptrInfo;
		}
		/*******************************************************************************
		 * Name	    : RemoveDataVarInfo
		 * Descript : ɾ��ָ����DataVar��Ϣ
		 * Input	: vid - DataVarInfo��ID��Ϣ
		 * Output   : bool - ture:ɾ���ɹ�  false:ʧ��
		 * Note	    :
		 *******************************************************************************/
		bool RemoveDataVarInfo(DataVarId& vid)
		{
			DataVarInfo* info = GetDataVarInfo(vid);
			if (info != nullptr)
			{
				info->Count = 0;
				return true;
			}
			return false;
		}

		/*******************************************************************************
		 * Name	    : GetDataVarId
		 * Descript : ��ȡָ��DataVarInfo��DataVarId
		 * Input	: DataVarInfo - DataVarInfo����Ϣ
		 * Output   : DataVarId - DataVarInfo��ID��Ϣ
		 * Note	    :
		 *******************************************************************************/
		DataVarId GetDataVarId(DataVarInfo* info)
		{
			DataVarId vid;
			if (VarInfos.size() > 0)
			{
				if (info >= &VarInfos[0] && info <= &VarInfos[VarInfos.size() - 1])
				{
					vid.Vid = ((unsigned int)info - (unsigned int)&VarInfos[0])/sizeof(DataVarInfo);
					return vid;
				}
			}
			if (TmpVarInfos.size() > 0)
			{
				if (info >= &TmpVarInfos[0] && info <= &TmpVarInfos[TmpVarInfos.size() - 1])
				{
					vid.Vid = ((unsigned int)info - (unsigned int)&TmpVarInfos[0])/ sizeof(DataVarInfo);
					return vid;
				}
			}
			return DataVarId::NullId;
		}
	};
}
