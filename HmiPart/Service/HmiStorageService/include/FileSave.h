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
#include <thread>
#ifdef WIN32
#include <windows.h> 
#endif
#include <string>
#include "SampleRecord.h"
#include "SampleInfoRes.h"
#include "AlarmRecord.h"
#include "AlarmInfoRes.h"
#include "OperatorRecord.h"
#ifdef WIN32
#include <setupapi.h>
#include <cfgmgr32.h>
#include "usbiodef.h"
#endif
#include <condition_variable>
#pragma comment(lib, "Setupapi.lib")
namespace Storage
{
	enum FileType:char
	{
		FileType_Sample = 0,
		FileType_Alarm,
		FileType_Operate
	};

#define TimingCleanCSV "FilesTimingClean.ini"
	class FileSave
	{
	private:
		FileSave();
	public:
		~FileSave() {
			RunTask = false;
			if (nullptr!= SaveFileTask)
			{
				if (SaveFileTask->joinable())
					SaveFileTask->join();
			}
		}
		static FileSave* GetFileSaveTool();
	public://��Ҫ��������߼��ӿ�
		void SaveSample(Project::SampleInfoRes* spIfRs);
		//bool ReadySaveSample() {return IsSampleSaveAvaliable;}
		void InitAlarm(Project::SaveFileRes* res);
		//bool ReadySaveAlarm() { return IsAlarmSaveAvaliable; }
		void InitOperate(Project::SaveFileRes* res);
		//bool ReadySaveOperate() { return IsOperateSaveAvaliable; }
		void DoSave()
		//����ļ��Ƿ��Ѵ��ڣ�ͬ���ļ��Ѵ���ʱ����true,����false
		bool IsFileExact(std::string filePath);
		//����csv�ļ�������
		//listcol:����Ϣ
		//strbuf:ȥ��������Ϣ������ݱ���
		bool LoadCSVData(FileType type,Project::SaveFileRes* res);
		bool Export2SampleBin(vector<SampleRecord>& records);
		//�����������ļ�
		//filePath:���������ļ���������·��
		//buf:��������
		//len:����
		//keepDay:��������
		//savenorepeat:����ʱÿ�α��涼�������µ��ļ�����ʱ�Զ�����ͬһ���ļ�
		int LocalExportFile(std::string filePath, char* buf, unsigned long long len, int keepDay, int savenorepeat = 1);
		//���ɾ����Ϣ��ini
		int AddDeleteTiming(std::string filePath, int keepDay);
		//��ȡini�ļ��е��ļ��б�
		int ReadTimingFilesFromIni();
		//���ɲ�ִ��CMD�Դ�������ļ�
		int ExecTimingList();
	public://���澲̬���ýӿ�
		//���ļ�β��ӣ������ظ��������Ϣ��
		static bool WriteAddOn(const char* fileName, char* buff, unsigned long long len);
		//ÿ��д�붼�����ļ�����
		static bool WriteByCover(const char* fileName, char* buff, unsigned long long len);
		//����·��
		static bool CreateFolder(string strFilePath);
		//��ȡ�ļ������ļ���
		static void GetFile(const string& path, string file);
		//bytes:0�г��ʣ��ռ�1�ռ䲻�㵫�ɳ��Ը���2�ܿռ���ȫ����
		static int IsDiskEnoughSpace(DWORD bytes,int StoreSpaceLack);
		static string GetSavePath(int pathMode, Project::DataVarId& addrPath, int nameMode, string fileName, Project::DataVarId& addrName);
		static std::vector<BYTE> ParseAlarmCol(const std::string& strheadline);
		static std::vector<BYTE> ParseSampleCol(const std::string& strheadline);
	private:
		//void HandleExportCSVdata(ExportCSVdata csvData);
		int GetRealChannelIdFromRecord(int channel);
		void FromSqlite2File(Project::SampleInfoRes& spIfRs);
		void FromSqlite2File(Project::SaveFileRes& res);
		void FromSqlite2OperateFile(Project::SaveFileRes& res);
		static int CopyIntegerToChar(char* ch, int value);
		static int CopyDataTypeStrToChar(char* dst, const Project::BaseVar* var);
		//static string GetSavePath(int pathMode, Project::DataVarId& addrPath, int nameMode, string fileName, Project::DataVarId& addrName,int needlen, int StoreSpaceLack);
		void StartKeepSave();

	private:
		std::thread* SaveFileTask;
		atomic<bool> RunTask;
		std::mutex Mutex;
		std::condition_variable TaskDispatcher;
		std::queue<Project::SampleInfoRes*> SampleSaveList;
		Project::SaveFileRes* AlarmSaveList;
		Project::SaveFileRes* OperateSaveList;

		atomic<bool> IsSampleSaveAvaliable;
		atomic<bool> IsAlarmSaveAvaliable;
		atomic<bool> IsOperateSaveAvaliable;
		/*bool IsSampleSaveThrRun;
		bool IsAlarmSaveThrRun;
		bool IsAlarmNewTrig;
		std::thread* ThrSaveSample;
		std::thread* ThrSaveAlarm;
		std::mutex MutexSample;
		std::mutex MutexAlarm;
		std::condition_variable SampleEventLock;
		std::condition_variable AlarmEventLock;

		Project::SaveFileRes* AlarmSaveConfig;*/

		std::vector<std::pair<std::string, int>> TimingFiles;
	};
}
