//#ifdef WIN32
//#include <windows.h> 
//#include <setupapi.h>
//#include <cfgmgr32.h>
//#include "usbiodef.h"
//#endif
//#include <string>
//#include "SampleRecord.h"
//#include "SampleInfoRes.h"
//#include "AlarmRecord.h"
//#include "AlarmInfoRes.h"
//#include <thread>
#ifdef WIN32
#include <windows.h> 
#endif
#include <string>
#include "SampleRecord.h"
#include "SampleInfoRes.h"
#include "AlarmRecord.h"
#include "AlarmInfoRes.h"
#ifdef WIN32
#include <setupapi.h>
#include <cfgmgr32.h>
#include "usbiodef.h"
#endif
#include <thread>

#pragma comment(lib, "Setupapi.lib")
namespace Storage
{
#define TimingCleanCSV "FilesTimingClean.ini"
	class FileSave
	{
	private:
		FileSave();
	public:
		~FileSave() {
			IsSampleSaveThrRun = false;
			if (nullptr!=ThrSaveSample)
			{
				if (ThrSaveSample->joinable())
					ThrSaveSample->join();
			}
		}
		static bool WriteNoRepeat(const char* fileName, char* buff, unsigned long long len);
		static bool WriteByCover(const char* fileName, char* buff, unsigned long long len);
		static FileSave* GetFileSaveTool();
		//�����������ļ�
		int LocalExportFile(std::string filePath,char* buf, unsigned long long len,int keepDay,int savenorepeat=1);
		//���ɾ����Ϣ��ini
		int AddDeleteTiming(std::string filePath, int keepDay);
		//��ȡini�ļ��е��ļ��б�
		int ReadTimingFilesFromIni();
		//���ɲ�ִ��CMD�Դ�������ļ�
		int ExecTimingList();
		//
		void InsertSampleResInQueue(Project::SampleInfoRes* spIfRs);
		//void InsertAlarmResInQueue(Project::AlarmInfoRes* almIfRs);
		void TryTrigSave(Project::SaveFileRes* res);
		void StartKeepSave();
		//0�г��ʣ��ռ�1�ռ䲻�㵫�ɳ��Ը���2�ܿռ���ȫ����
		static int IsDiskEnoughSpace(DWORD bytes,int StoreSpaceLack);
	private:
		//void HandleExportCSVdata(ExportCSVdata csvData);
		int GetRealChannelIdFromRecord(int channel, int group, int no);
		void FromSqlite2File(Project::SampleInfoRes& spIfRs);
		void FromSqlite2File(Project::SaveFileRes& res);
		static int CopyIntegerToChar(char* ch, int value);
		static int CopyDataTypeStrToChar(char* dst, const Project::BaseVar* var);
		static string GetSavePath(int pathMode, Project::DataVarId& addrPath, int nameMode, string fileName, Project::DataVarId& addrName,int needlen, int StoreSpaceLack);

	private:
		bool IsSampleSaveThrRun;
		bool IsAlarmSaveThrRun;
		bool IsAlarmNewTrig;
		std::thread* ThrSaveSample;
		std::thread* ThrSaveAlarm;
		std::mutex Mutex;
		std::queue<Project::SampleInfoRes*> SampleSaveList;

		Project::SaveFileRes* AlarmSaveConfig;

		std::vector<std::pair<std::string, int>> TimingFiles;
	};
}
