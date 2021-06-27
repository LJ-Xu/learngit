#pragma once
#include <vector>
#include <memory>

#include "ModuleAssemble.h"
#include "DataApi.h"
#include "Observer.h"

namespace UI
{
	class UIDataService
	{
	public:	
		static UIDataService& Ins();
		void Attatch(Observer * obsrv)
		{
			DataApi::Attatch(obsrv);
		}

		bool Init(Observer* obsrv, Project::HMIProject* prj)
		{
	
			vars_ = &prj->Vars;
			DataApi::Init(prj);
			DataApi::Attatch(obsrv);
			return true;
		}
		void Start()
		{
			DataApi::Start();
		}
		void Stop()
		{
			DataApi::Stop();
		}
		XJDataType GetDataType(Project::DataVarId vid)
		{
			XJDataType tp = {0,0};
			Project::DataVarInfo* info = vars_->GetDataVarInfo(vid);
			if (info)
			{
				tp.Cls = info->DataType;
				tp.Type = info->DataExType;
			}
			return tp;
		}
		int GetDataCounts(Project::DataVarId vid)
		{
			Project::DataVarInfo* info = vars_->GetDataVarInfo(vid);
			if (info)
				return info->Count;
			else
				return 0;
		}
		template<class T>
		T GetNumber(Project::DataVarId vid)
		{
			T val = 0;
			Project::DataVarInfo* info = vars_->GetDataVarInfo(vid);
			if(info)
				DataApi::GetDataBytes(vid, (char*)&val,MIN(info->Count, sizeof(val)));
			return val;
		}
		DDWORD GetNumber(Project::DataVarId vid)
		{
			DDWORD val = 0;
			Project::DataVarInfo* info = vars_->GetDataVarInfo(vid);
			if(info)
				DataApi::GetDataBytes(vid, (char*)&val, MIN(info->Count, sizeof(val)));
			return val;
		}
		bool GetBit(Project::DataVarId vid)
		{
			return DataApi::GetBit(vid);
		}
		void SetBit(Project::DataVarId vid, bool val)
		{
			DataApi::SetBit(vid,val);
		}
		bool GetVarDatas(Project::DataVarId vid, int count, DataRecvCB cb = nullptr)
		{
			return DataApi::GetVarDatas(vid, count);
		}

		int MoveDataVars(Project::DataVarId svid, Project::DataVarId dvid, int regcount)
		{
			return DataApi::MoveDataVars(svid, dvid, regcount);
		}

		template<class T>
		void SetNumber(Project::DataVarId vid, T data)
		{
			DataApi::SetDataBytes(vid, (char*)&data, sizeof(T));
		}
		void SetNumber(Project::DataVarId vid,DDWORD val)
		{
			DataApi::SetDataBytes(vid, (char*)&val, sizeof(DDWORD));
		}
		void  GetDataBytes(Project::DataVarId vid, BYTE* ddata, int len)
		{
			DataApi::GetDataBytes(vid, ddata, len);
		}
		void  SetDataBytes(Project::DataVarId vid, const BYTE* sdata, int len)
		{
			DataApi::SetDataBytes(vid, sdata, len);
		}
		void  GetString(Project::DataVarId vid, char* str, int maxnum)
		{
			DataApi::GetDataBytes(vid, str, maxnum);
		}
		string  GetString(Project::DataVarId vid)
		{
			Project::DataVarInfo* info = vars_->GetDataVarInfo(vid);
			if (info)
			{
				unique_ptr<char> chars(new char[info->Count]);
				DataApi::GetDataBytes(vid, chars.get(), info->Count);
				return string(chars.get());
			}
			else
				return string();
		}
		void  SetString(Project::DataVarId vid, const char* str)
		{
			DataApi::SetDataBytes(vid, str, strlen(str)+1);
		}


 


		


		void AddWin(int winno)
		{
			DataApi::OnPageOpen(winno);
		}
		void RemoveWin(int winno)
		{
			DataApi::OnPageClose(winno);
		}
		private:
			Project::ProjectPortVarsInfo* vars_ = nullptr;

	};

}


