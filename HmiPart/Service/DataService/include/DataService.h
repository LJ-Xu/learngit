#pragma once
#include "stdafx.h"
#include "HMIProject.h"
#include "ProjectPortVarsInfo.h"
#include "IDaAppCB.h"
#include "Observer.h"
#include "DataCache.h"
#include <thread>
#include "Semaphore.h"
#include "OutTimeClock.h"
#include <atomic>

namespace HMI
{
	class DataClient;
}
namespace UI
{
#define DATA_BASE_SIZE (sizeof(DWORD))
	typedef int(*DataRecvCB)(BYTE* ddata, int len);
	class DataFrame;
	class DataService :public HMI::IDaAppCB
	{
	public:
		DataService() :sysDataOK_(false)
		{}
	public:
		void Init(Project::HMIProject*);
		void Start();
		void Exit();
		~DataService();
		void Attatch(Observer * p);
		bool MacroTrigger(const char* funcname);
		bool MoveData(const char* cmd);

		bool OperatRecipeRecord(const char* recipeinfo, Project::RecipeAction action);
		bool CSVdataExport(const char* csvinfo);
		bool CSVdataImport(const char* csvinfo);

		bool PostMove(Project::BaseVar& svar, Project::BaseVar& dvar, int regcount);
		bool PostRead(Project::BaseVar& var, int id);
		bool PostWrite(Project::BaseVar& var, const BYTE* data, int len);
		bool RemovePage(int no);
		bool AddPage(int no);
		std::shared_ptr<DataCache> DataCacheRef()
		{
			return dataCache_;
		}
	private:
		void LockSend()
		{
			mtx_.lock();
		}
		void UnLockSend()
		{
			mtx_.unlock();
		}
		std::mutex mtx_;
		std::unique_ptr<DataFrame> dataFrame_;
		std::shared_ptr<DataCache> dataCache_;
		std::unique_ptr<HMI::DataClient> client_;
		volatile bool sysDataOK_;

		thread td_;
		bool runFlag_ = false;
		
		Project::ProjectPortVarsInfo* vars_ = nullptr;


		private:
		friend void Run(void* param);
		

		/*Observer* obsrv_;
		vector<WinData*> winData_;*/


		void ReadCB(int id, char* buf, int len);
		void FrameReadCB(int id, int eid, char* buf, int len);


	};
}
