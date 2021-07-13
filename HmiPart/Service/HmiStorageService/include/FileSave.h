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
	public://主要保存相关逻辑接口
		void SaveSample(Project::SampleInfoRes* spIfRs);
		//bool ReadySaveSample() {return IsSampleSaveAvaliable;}
		void InitAlarm(Project::SaveFileRes* res);
		//bool ReadySaveAlarm() { return IsAlarmSaveAvaliable; }
		void InitOperate(Project::SaveFileRes* res);
		//bool ReadySaveOperate() { return IsOperateSaveAvaliable; }
		void DoSave()
		//检测文件是否已存在，同名文件已存在时返回true,否则false
		bool IsFileExact(std::string filePath);
		//加载csv文件的内容
		//listcol:列信息
		//strbuf:去除了列信息后的数据本体
		bool LoadCSVData(FileType type,Project::SaveFileRes* res);
		bool Export2SampleBin(vector<SampleRecord>& records);
		//导出到本地文件
		//filePath:包含最终文件名的完整路径
		//buf:保存内容
		//len:长度
		//keepDay:保存天数
		//savenorepeat:非零时每次保存都会生成新的文件，零时自动覆盖同一个文件
		int LocalExportFile(std::string filePath, char* buf, unsigned long long len, int keepDay, int savenorepeat = 1);
		//添加删除信息到ini
		int AddDeleteTiming(std::string filePath, int keepDay);
		//读取ini文件中的文件列表
		int ReadTimingFilesFromIni();
		//生成并执行CMD以处理过期文件
		int ExecTimingList();
	public://常规静态可用接口
		//在文件尾添加（不会重复添加列信息）
		static bool WriteAddOn(const char* fileName, char* buff, unsigned long long len);
		//每次写入都覆盖文件内容
		static bool WriteByCover(const char* fileName, char* buff, unsigned long long len);
		//创建路径
		static bool CreateFolder(string strFilePath);
		//获取文件夹下文件名
		static void GetFile(const string& path, string file);
		//bytes:0有充分剩余空间1空间不足但可尝试覆盖2总空间完全不足
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
