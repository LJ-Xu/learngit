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
#include "OperatorRecord.h"
#ifdef WIN32
#include <setupapi.h>
#include <cfgmgr32.h>
#include "usbiodef.h"
#endif

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
			/*IsSampleSaveThrRun = false;
			if (nullptr!=ThrSaveSample)
			{
				if (ThrSaveSample->joinable())
					ThrSaveSample->join();
			}*/
		}
	public://保存主要接口
		void SaveSample(Project::SampleInfoRes& spIfRs);
		bool ReadySaveSample() {return IsSampleSaveAvaliable;}
		void SaveAlarm(Project::SaveFileRes& res);
		bool ReadySaveAlarm() { return IsAlarmSaveAvaliable; }
		void SaveOperate(Project::SaveFileRes& res);
		bool ReadySaveOperate() { return IsOperateSaveAvaliable; }
	public://常规可用接口
		static bool WriteNoRepeat(const char* fileName, char* buff, unsigned long long len);
		static bool WriteByCover(const char* fileName, char* buff, unsigned long long len);
		static FileSave* GetFileSaveTool();
		//导出到本地文件
		//filePath:包含最终文件名的完整路径
		//buf:保存内容
		//len:长度
		//keepDay:保存天数
		//savenorepeat:非零时每次保存都会生成新的文件，零时自动覆盖同一个文件
		int LocalExportFile(std::string filePath,char* buf, unsigned long long len,int keepDay,int savenorepeat=1);
		//添加删除信息到ini
		int AddDeleteTiming(std::string filePath, int keepDay);
		//读取ini文件中的文件列表
		int ReadTimingFilesFromIni();
		//生成并执行CMD以处理过期文件
		int ExecTimingList();
		//bytes:0有充分剩余空间1空间不足但可尝试覆盖2总空间完全不足
		static int IsDiskEnoughSpace(DWORD bytes,int StoreSpaceLack);
	private:
		//void HandleExportCSVdata(ExportCSVdata csvData);
		int GetRealChannelIdFromRecord(int channel, int group, int no);
		void FromSqlite2File(Project::SampleInfoRes& spIfRs);
		void FromSqlite2File(Project::SaveFileRes& res);
		void FromSqlite2OperateFile(Project::SaveFileRes& res);
		static int CopyIntegerToChar(char* ch, int value);
		static int CopyDataTypeStrToChar(char* dst, const Project::BaseVar* var);
		static string GetSavePath(int pathMode, Project::DataVarId& addrPath, int nameMode, string fileName, Project::DataVarId& addrName,int needlen, int StoreSpaceLack);

	private:
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
		std::queue<Project::SampleInfoRes*> SampleSaveList;

		Project::SaveFileRes* AlarmSaveConfig;*/

		std::vector<std::pair<std::string, int>> TimingFiles;
	};
}
