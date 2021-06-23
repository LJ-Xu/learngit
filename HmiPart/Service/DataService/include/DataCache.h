#pragma once
#include "stdafx.h"
#include "ProjectPortVarsInfo.h"
#include "HMIProject.h"
#include "Observer.h"
namespace UI
{
	class LocalSysDataCache;
	struct WinData;
	class DataCache
	{
	public :
 
		void Init(Project::ProjectPortVarsInfo* vars);

		int Cache(Project::DataVarId vid, DWORD val);

		int Cache(Project::DataVarId vid, char* buf,int len);

		int Get(Project::DataVarId vid, DWORD& val);

		int Get(Project::DataVarId vid, char* buf, int len);
		//ȡ�ɱ�����,��count=0ʱ��offsetΪ������ʼ��ȡλ,һֱȡ������Ϊֹ
		//int Get(Project::DataVarId vid, int& len, char* buf=nullptr);

		int Get(Project::BaseVar& var, char* buf, int len);

		void AddWin(int winno);

		void RemoveWin(int winno);

		void TD_SetFrameData(int wid, int idx, char* data, int len);

		void TD_SetData(int vid, char* data, int len);
 
		void Attatch(Observer * p);
	private:
		void InitWinData(WinData* wd);
		std::mutex mtx_;
		Observer* obsrv_ = nullptr;
		Project::ProjectPortVarsInfo* vars_ = nullptr;
		WinData* GetWinDataByVid(int vid);
		WinData* GetWinDataByWinNo(int winno);
		vector<unique_ptr<WinData>> winDatas_;//�������ݻ���

	};
	struct WinData
	{
		int WinNo;
		Project::WindowVar* WinVar;
		//����д�Ͷ����� ����������
		std::unique_ptr<char[]> Buf;// ����4�ֽڵ�
		bool IsOffsetOk(unsigned int data)
		{
			return data < WinVar->BufSize;
		}
	};
}