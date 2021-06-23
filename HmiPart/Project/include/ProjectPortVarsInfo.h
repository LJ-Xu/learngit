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
	//	vector<int> DirectVars;//直接指定的变量内容
	//	vector<int> IndirectVars;//需要间接指定的变量

	//	template<class Archive>
	//	void serialize(Archive & archive)
	//	{
	//		archive(DirectVars, IndirectVars);
	//	}
	//};
	//// 存储指定站号下的所有寄存器信息
	//struct PrjDevRegsInfo
	//{
	//	/*************用于找到对应的设备***************/
	//	char   Port;
	//	PointType Point;
	//	char StaNo;
	//	/*************具体信息内容*****************/
	//	map<int/*寄存器编号*/, PrjDevRegVars> Devs;

	//	template<class Archive>
	//	void serialize(Archive & archive)
	//	{
	//		archive(Port, Point, StaNo, Devs);
	//	}

	//};
	//// 端口对应的设备接口
	//struct PrjPortPoint
	//{
	//	string Name; // 设备名称
	//	PortType Port;//对应的端口
	//	PointType Point;//该端口下的接口   下载口下的接口 RS485 RS422 RS232
	//	PrjCommParam Param;//该接口的通信参数
	//	short ProtocolId;//设备的通信协议Id
	//	//short staNo;//设备站号  
	//	map<int/*站号*/, PrjDevRegsInfo>  Devs; //该接口下站号对应设备下所有的寄存器变量信息

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
		//int SIdx;//包含数据区域内容的起始VAR信息
		std::vector<DataVarId> VIds;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(RISize, Var, VIds);
		}
	};
	struct WindowVar
	{
		//int DirectIdx;   //直接指定的起始ID
		//int InDirectIdx; //间接指定的起始ID
		//int EIdx;        //末尾,包含
		unsigned int BufSize;     //大于4字节的buf大小
		//DataVarId SVID;  //开始不可组帧的vid 列表
		std::vector<FrameInfo> Frames;//帧
		std::vector<DataVarId> VIds;//不能组帧的直接变量
		//std::vector<DataVarId> InDirectIds;//不能组帧的直接变量
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(/*DirectIdx, InDirectIdx, EIdx, */BufSize, Frames, VIds);
		}
		   
	};
 
	
	struct ProjectPortVarsInfo
	{
		//// 每个port下可能有多个接口设备
		//std::vector<PrjPortPoint> Points; //设备节点 device ID 为索引号
		//std::map<int, PrjPortPoint> Points;
		// datavar的存储集合  ,WINID ->DataVarInfo集合
		std::map<int, WindowVar> WinVarInfos;   //如果搜索慢可以缓寸索引到WindowVar的关系

		//系统编译后的VARINFO结构,整个应用所有的DataVarInfo  直接指定+间接指定
		std::vector<DataVarInfo> VarInfos;

		//std::vector<int> VarIdxs;

		//动态的Var变量，主要用于临时存储
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
		 * Descript : 根据VarId获取VARINFO
		 * Input	: vid - DataVarId
		 * Output   : DataVarInfo* - 返回DataVarInfo
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
		 * Descript : 根据VarId获取VARINFO
		 * Input	: vid - DataVarId
		 * Output   : DataVarInfo* - 返回DataVarInfo
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
		 * Descript : 分配一个新的Var
		 * Input	: vid     - 添加后返回的ID
		 *			  colType - 类别，目前分两个 一个是内部寄存器  一个是外部寄存器
		 *            varType - 数据类别 一个是位相关，一个是字相关数据
		 * Output   : DataVarInfo - 返回添加的DataVarInfo
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
		 * Descript : 删除指定的DataVar信息
		 * Input	: vid - DataVarInfo的ID信息
		 * Output   : bool - ture:删除成功  false:失败
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
		 * Descript : 获取指定DataVarInfo的DataVarId
		 * Input	: DataVarInfo - DataVarInfo的信息
		 * Output   : DataVarId - DataVarInfo的ID信息
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
