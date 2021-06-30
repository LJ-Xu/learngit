#include "FileSave.h"
#include "Logger.h"
#include <iostream>
#include <algorithm>
#ifdef WIN32
#include <io.h>
#endif
#include <fstream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef WIN32
#include <direct.h>
#endif
#include "System.h"
#include "SampleStorage.h"
#include "AlarmStorage.h"
#include "UIData.h"
#ifndef WIN32
#include <unistd.h>
#define _stat stat
#define _S_IFDIR  S_IFDIR
#define _mkdir mkdir
#define HANDLE unsigned long
#include <sys/statfs.h>  
#include <sys/types.h>
#include <stdio.h> 
#include<dirent.h>	
#endif
//#include "../../../UIControl/include/UIData.h"
//#include "../../DataService/include/DataApi.h"
#include "UIData.h"
#include "DataApi.h"
#define LINELENGTH 128
using namespace std;
namespace Storage
{
	static FileSave* FileSaveTool = nullptr;
	//创建路径
	bool CreateFolder(string strFilePath)
	{
		string::size_type  pos(0);
		while (true)
		{
			if ((pos = strFilePath.find("/", pos)) != string::npos)
			{
				strFilePath.replace(pos, 1, "\\");
			}
			else
			{
				break;
			}
		}
		string::size_type rFirstPos = strFilePath.rfind("\\");
		if (rFirstPos == string::npos)
			return true;
		if (strFilePath.size() != (rFirstPos + 1))   /* 如果转换后的路径不是以\\结束时候，往末尾添加\\，处理的格式统一为D:\\1\\2\\3\\ */
		{
			//检测最后一个是否为文件名
			string strTemp = strFilePath.substr(rFirstPos, strFilePath.size());
			if (string::npos != strTemp.find("."))
			{
				//最后一个不是文件夹名
				strFilePath = strFilePath.substr(0, rFirstPos + 1);
			}
			else
			{
				//最后一个是文件夹名字
				strFilePath += "\\";
			}
		}
		else
		{
			strFilePath += "\\";
		}
		string::size_type startPos(0);
		string::size_type endPos(0);
		while (true)
		{
			if ((endPos = strFilePath.find("\\", startPos)) != string::npos)
			{
				string strFolderPath = strFilePath.substr(0, endPos);
				startPos = endPos + string::size_type(1);
				if (strFolderPath.rfind(":") == (strFolderPath.size() - 1))
				{
					//跳过只有系统盘的路径的情况，如：D:
					continue;
				}
				struct _stat fileStat = { 0 };
				if (_stat(strFolderPath.c_str(), &fileStat) == 0)
				{
					//文件存在，判断其为目录还是文件
					if (!(fileStat.st_mode & _S_IFDIR))
					{
						//不是文件夹，则创建失败退出返回
						return false;
					}
				}
				else
				{
					//文件夹不存在，则进行创建
#ifdef WIN32
					if (-1 == _mkdir(strFolderPath.c_str()))
#else
					if (-1 == _mkdir(strFolderPath.c_str(), 0766))
#endif // WIN32
					{
						return false;
					}
				}
				continue;
			}
			break;
		}
		return true;
	}
	//获取文件夹下文件名
	void getFile(const string& path, string file)
	{
#ifdef WIN32
		//文件句柄
		long   hFile = 0;
		//文件信息
		struct _finddata_t fileinfo;
		string p;
		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
		{
			do
			{
				//如果是目录,迭代之
				//如果不是,加入列表
				if ((fileinfo.attrib &  _A_SUBDIR))
				{
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
						getFile(p.assign(path).append("\\").append(fileinfo.name), file);
				}
				else
				{
					file= p.assign(path).append("\\").append(fileinfo.name);
					return;
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
#else
		DIR* d = opendir(path.c_str());
		if (d == NULL)
		{
			file = "";
			return;
		}
		struct dirent* entry;
		if ((entry == readdir(d)) != NULL)
		{
			file.append(entry->d_name);
		}
		closedir(d);
#endif
	}

	//重复的文件名则保存为*(1~N).*这样的格式
	bool FileSave::WriteNoRepeat(const char* fileName,char* buff,unsigned long long len)
	{
		FileSave::GetFileSaveTool()->ExecTimingList();
		string strFileName(fileName);
		if (!CreateFolder(fileName))
		{
			return false;
		}
		int newIdx = 1;
		char idxCh[10];
		string tempFN = strFileName;
		while (ifstream(tempFN).good())
		{
			tempFN = strFileName;
			string::size_type size = strFileName.find_last_of(".");
			snprintf(idxCh, 10, "(%d)", newIdx++);
			tempFN.insert(size, idxCh);
		}
		ofstream ofs;
		ofs.open(tempFN.c_str(), ios::out | ios::binary);
		if (!ofs.is_open())
			return false;
		ofs.write(buff, len);
		ofs.close();
		return true;
	}
	bool FileSave::WriteByCover(const char * fileName, char * buff, unsigned long long len)
	{
		FileSave::GetFileSaveTool()->ExecTimingList();
		string strFileName(fileName);
		if (!CreateFolder(fileName))
		{
			return false;
		}
		ofstream ofs;
		ofs.open(strFileName.c_str(), ios::out | ios::binary);
		if (!ofs.is_open())
			return false;
		ofs.write(buff, len);
		ofs.close();
		return true;
	}
	FileSave::FileSave()
	{
		ReadTimingFilesFromIni();
		StartKeepSave();
	}
	FileSave * FileSave::GetFileSaveTool()
	{
		if (!FileSaveTool)
			FileSaveTool = new FileSave();
		return FileSaveTool;
	}
	int FileSave::LocalExportFile(std::string filePath, char* buf, unsigned long long len, int keepDay, int savenorepeat)
	{
		if (!filePath.compare(""))
			return 0;
		if (savenorepeat)
		{
			if (!WriteNoRepeat(filePath.c_str(), buf, len))
				return 0;
		}
		else
		{
			if (!WriteByCover(filePath.c_str(), buf, len))
				return 0;
		}
		AddDeleteTiming(filePath, keepDay);
		return 1;
	}
	int FileSave::AddDeleteTiming(std::string filePath, int keepDay)
	{
		//std::lock_guard<std::mutex> lck(Mutex);
		if (keepDay > 0)
		{
			//添加到内存
			TimingFiles.push_back(std::make_pair(filePath,keepDay));
			//添加到文件
			ofstream ofs;
			ofs.open(TimingCleanCSV, ios::out | ios::binary|ios::app);
			if (!ofs.is_open())
				return false;
			ofs << filePath << "=" << to_string(keepDay)<<endl;
			ofs.close();
			return 0;
		}
		return 0;
	}
	int FileSave::ReadTimingFilesFromIni()
	{
		//std::lock_guard<std::mutex> lck(Mutex);
		ifstream ifs;
		ifs.open(TimingCleanCSV, ios_base::in);
		if (!ifs.is_open())
		{
			return 0;
		}
		char bufline[256];
		while (ifs.getline(bufline,256))
		{
			std::string tempstr(bufline);
			int eqidx = tempstr.find_first_of('=');
			TimingFiles.push_back(std::make_pair(tempstr.substr(0,eqidx-1), atoi(tempstr.substr(eqidx+1).c_str())));
		}
		return (int)TimingFiles.size();
	}
	int FileSave::ExecTimingList()
	{
		//std::lock_guard<std::mutex> lck(Mutex);
		char bufcmd[256];
		char bufres[64];
		for (auto itor = TimingFiles.begin(); itor != TimingFiles.end(); ++itor)
		{
			//memset(bufcmd, 0, 256);
			//memset(bufres, 0, 64);
			//snprintf(bufcmd, 256, "forfiles /m \"%s\" /d -%d /c \"cmd / c del / f / s / q @file\"", itor->first.c_str(), itor->second);
			//HANDLE hReadPipe = NULL;
			//HANDLE hWritePipe = NULL;
			////SECURITY_ATTRIBUTES sA = { 0 };
			//BOOL bRet = false;
			////STARTUPINFO si = { 0 };
			////PROCESS_INFORMATION pi = { 0 };

			//sA.bInheritHandle = true;
			//sA.nLength = sizeof(sA);
			//sA.lpSecurityDescriptor = NULL;

			//bRet = ::CreatePipe(&hReadPipe, &hWritePipe, &sA, 0);
			//if (false == bRet)
			//{
				//LOG_ERROR("CREATEPIPE error in FileSave::ExecTimingList()!");
				//return 0;
			//}
			//si.cb = sizeof(si);
			//si.hStdError = hWritePipe;
			//si.hStdOutput = hWritePipe;
			//si.wShowWindow = SW_HIDE;
			//si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
			//bRet = ::CreateProcess(NULL, bufcmd, NULL, NULL, true, 0, NULL, NULL, &si, &pi);
			//if (false == bRet)
			//{
				//LOG_ERROR("CreateProcess error in FileSave::ExecTimingList()!");
				//return 0;
			//}
			//::WaitForSingleObject(pi.hThread, INFINITE);
			//::WaitForSingleObject(pi.hProcess, INFINITE);
			//::RtlZeroMemory(bufres, 64);
			//::ReadFile(hReadPipe, bufres, 64, NULL, NULL);
			////根据执行结果处理当前项(执行成功后清除)

			//::CloseHandle(pi.hThread);
			//::CloseHandle(pi.hProcess);
			//::CloseHandle(hWritePipe);
			//::CloseHandle(hReadPipe);
		}
		return 1;
	}
	void FileSave::FromSqlite2File(Project::SampleInfoRes & spIfRs)
	{
		bool DoSave = false;
		switch (spIfRs.SampleStoreInfo.StoreFileInfo.StoreTrigMode)
		{
		case Project::StoreTrigMode::STM_ByCount:
		{
			if (SampleStorage::Ins()->GetAllCountByNo(spIfRs.SimpleGroupName, spIfRs.SimpleGroupNo, spIfRs.SampleStoreInfo.StoreFileInfo.SavedFlag) >= (spIfRs.SampleStoreInfo.StoreFileInfo.SaveTrigCount))
			{
				DoSave = true;
			}
			break;
		}
		case Project::StoreTrigMode::STM_ByAddr:
		{
			if (NULL_VID_VALUE != spIfRs.SampleStoreInfo.StoreFileInfo.SaveTrigAddr.Vid)
			{
				if (UI::DataApi::AppBit(spIfRs.SampleStoreInfo.StoreFileInfo.SaveTrigAddr) == true)
				{
					DoSave = true;
				}
			}
			break;
		}
		default:
			return;
		}
		if (!DoSave)
			return;
		string filePath;
		vector<Storage::SampleRecord> vecSpRec;
		vecSpRec = SampleStorage::Ins()->QueryByGroupByTime(spIfRs.SimpleGroupName, spIfRs.SimpleGroupNo, spIfRs.SampleStoreInfo.StoreFileInfo.SavedFlag);
		if (!vecSpRec.size())
			return;
		unsigned long long buflen = (vecSpRec.size() + 1)*LINELENGTH;
		char* buff = new char[buflen];
		memset(buff, 0, buflen);
		char* tembuf = buff;
		int id = 1;
		for (char* chfmt = spIfRs.SampleStoreInfo.StoreFileInfo.SaveFormat; *chfmt != 0; ++chfmt)
		{
			switch (*chfmt)
			{
			case 0://
				break;
			case 1://序号
				memcpy(tembuf, "序号,", 5);
				tembuf += 5;
				break;
			case 2://日期
				memcpy(tembuf, "日期,", 5);
				tembuf += 5;
				break;
			case 3://时间
				memcpy(tembuf, "时间,", 5);
				tembuf += 5;
				break;
			case 4://地址
				memcpy(tembuf, "地址,", 5);
				tembuf += 5;
				break;
			case 5://采集数据
				memcpy(tembuf, "采集数据,", 9);
				tembuf += 9;
				break;
			}
		}
		//--tembuf;
		memcpy(tembuf, "\n", 2);
		tembuf += 2;
		for (size_t i = 0; i < vecSpRec.size(); ++i)
		{
			string strDT = System::GetDateTimeToString(vecSpRec[i].Date/1000);
			for (char* chfmt = spIfRs.SampleStoreInfo.StoreFileInfo.SaveFormat; *chfmt != 0; ++chfmt)
			{
				int channel = GetRealChannelIdFromRecord(vecSpRec[i].Channel, spIfRs.SimpleGroupName, spIfRs.SimpleGroupNo) - 1;
				if (channel > spIfRs.SimpleChannelLst.size())
				{
					LOG_ERROR("Wrong Channel Id:%d in FileSave::FromSqlite2File(&).", vecSpRec[i].Channel);
					continue;
				}
				DataVarId vid = spIfRs.SimpleChannelLst[channel].SampleAddr;
				switch (*chfmt)
				{
				case 1://序号
					tembuf += CopyIntegerToChar(tembuf, id++);
					memset(tembuf++, 44, 1);
					//sprintf(buff, "%s%d,",buff, i+1);
					break;
				case 2://日期
				{
					//string str = System::GetDateToString(vecSpRec[i].Date);
					string str = strDT.substr(0,strDT.find_first_of(" ") - 1);
					memcpy(tembuf, str.c_str(), str.size());
					tembuf += str.size();
					memset(tembuf++, 44, 1);
					break;
				}
				case 3://时间
				{
					string str = strDT.substr(strDT.find_first_of(" ")+1);
					memcpy(tembuf, str.c_str(), str.size());
					tembuf += str.size();
					memset(tembuf++, 44, 1);
					break;
				}
				case 4://地址
					//目前认为通道总是从1开始
					tembuf += CopyDataTypeStrToChar(tembuf, UI::DataApi::GetDataVarInfo(vid));
					memset(tembuf++, 44, 1);
					break;
				case 5://采集数据
					std::string str = UI::DataApi::AppString(vid);
					if (0< str.size()&&(str.size() <= sizeof(DWORD)))
					{
						int* dgt = new int();
						memcpy(dgt, str.c_str(), str.size());
						snprintf(tembuf, 5, "%d", *dgt);
						tembuf += sizeof(int);
						delete dgt;
					}
					else if(str.size() > sizeof(DWORD))
					{
						memcpy(tembuf, str.c_str(), str.size());
						tembuf += str.size();
					}
					memset(tembuf++, 44, 1);
					break;
				}
			}
			//--tembuf;
			memcpy(tembuf, "\n", 2);
			tembuf += 2;
		}
		filePath = GetSavePath(spIfRs.SampleStoreInfo.StoreFileInfo.StoreLocation, spIfRs.SampleStoreInfo.StoreFileInfo.StorePosVarId, spIfRs.SampleStoreInfo.StoreFileInfo.FileNameMode, spIfRs.SampleStoreInfo.StoreFileInfo.FileName, spIfRs.SampleStoreInfo.StoreFileInfo.FileNameDataVar, buflen, spIfRs.SampleStoreInfo.StoreFileInfo.StoreSpaceLack);
		if (LocalExportFile(filePath, buff, buflen, spIfRs.SampleStoreInfo.StoreFileInfo.IsFileSaveTimeLimit?spIfRs.SampleStoreInfo.StoreFileInfo.SaveDays:0))
		{
			spIfRs.SampleStoreInfo.StoreFileInfo.SavedCount += vecSpRec.size();
			spIfRs.SampleStoreInfo.StoreFileInfo.SavedFlag = vecSpRec[vecSpRec.size() - 1].Date;
		}
		//delete[]buff;
	}
	void FileSave::FromSqlite2File(Project::SaveFileRes & res)
	{
		/*bool DoSave = false;
		switch (res.StoreTrigMode)
		{
		case Project::StoreTrigMode::STM_ByCount:
		{
			if (AlarmStorage::Ins()->GetAllCountByNo(spIfRs.SimpleGroupName, spIfRs.SimpleGroupNo, spIfRs.SampleStoreInfo.StoreFileInfo.SavedFlag) >= (spIfRs.SampleStoreInfo.StoreFileInfo.SaveTrigCount))
			{
				DoSave = true;
			}
			break;
		}
		case Project::StoreTrigMode::STM_ByAddr:
		{
			if (NULL_VID_VALUE != spIfRs.SampleStoreInfo.StoreFileInfo.SaveTrigAddr.Vid)
			{
				if (UI::DataApi::AppBit(spIfRs.SampleStoreInfo.StoreFileInfo.SaveTrigAddr) == true)
				{
					DoSave = true;
				}
			}
			break;
		}
		default:
			return;
		}
		if (!DoSave)
			return;*/
		string filePath;
		vector<Storage::AlarmRecord> vecAlmRec;
		vecAlmRec = AlarmStorage::Ins()->QueryAll();
		if (!vecAlmRec.size())
			return;
		unsigned long long buflen = (vecAlmRec.size() + 1)*LINELENGTH;
		char* buff = new char[buflen];
		memset(buff, 0, buflen);
		char* tembuf = buff;
		//int id = 1;
		for (char* chfmt = res.SaveFormat; *chfmt != 0; ++chfmt)
		{
			switch (*chfmt)
			{
			case 0://
				break;
			case 1://序号
				memcpy(tembuf, "序号,", 5);
				tembuf += 5;
				break;
			case 2://报警触发日期
				memcpy(tembuf, "报警触发日期,", 13);
				tembuf += 13;
				break;
			case 3://报警触发时间
				memcpy(tembuf, "报警触发时间,", 13);
				tembuf += 13;
				break;
			case 4://报警信息
				memcpy(tembuf, "报警信息,", 9);
				tembuf += 9;
				break;
			case 5://报警次数
				memcpy(tembuf, "报警次数,", 9);
				tembuf += 9;
				break;
			case 6://确认时间
				memcpy(tembuf, "确认时间,", 9);
				tembuf += 9;
				break;
			case 7://报警恢复时间
				memcpy(tembuf, "报警恢复时间,", 13);
				tembuf += 13;
				break;
			}
		}
		//--tembuf;
		memcpy(tembuf, "\n", 2);
		tembuf += 2;
		for (size_t i = 0; i < vecAlmRec.size(); ++i)
		{
			string strSDT = System::GetDateTimeToString(vecAlmRec[i].StartTick / 1000);
			for (char* chfmt = res.SaveFormat; *chfmt != 0; ++chfmt)
			{
				switch (*chfmt)
				{
				case 1://序号
					tembuf += CopyIntegerToChar(tembuf, i+1);
					memset(tembuf++, 44, 1);
					break;
				case 2://报警触发日期
				{
					string str = strSDT.substr(0, strSDT.find_first_of(" ") - 1);
					memcpy(tembuf, str.c_str(), str.size());
					tembuf += str.size();
					memset(tembuf++, 44, 1);
					break;
				}
				case 3://报警触发时间
				{
					string str = strSDT.substr(strSDT.find_first_of(" ") + 1);
					memcpy(tembuf, str.c_str(), str.size());
					tembuf += str.size();
					memset(tembuf++, 44, 1);
					break;
				}
				case 4://报警信息
					strcpy(tembuf, "\"");
					tembuf += 2;
					memcpy(tembuf, vecAlmRec[i].Title.c_str(), vecAlmRec[i].Title.size());
					tembuf += vecAlmRec[i].Title.size();
					strcpy(tembuf, "\"");
					tembuf += 2;
					memset(tembuf++, 44, 1);
					break;
				case 5://报警次数
					snprintf(tembuf, 6, "%d", vecAlmRec[i].Times);
					tembuf += sizeof(int);
					memset(tembuf++, 44, 1);
					break;
				case 6://确认时间
				{
					string str = "";
					if (vecAlmRec[i].CheckTick)
					{
						str = System::GetDateTimeToString(vecAlmRec[i].CheckTick / 1000);
						memcpy(tembuf, str.c_str(), str.size());
						tembuf += str.size();
						memset(tembuf++, 44, 1);
					}
					else
					{
						memset(tembuf++, 44, 1);
					}
					break;
				}
				case 7://报警恢复时间
				{
					string str = "";
					if (vecAlmRec[i].CheckTick)
					{
						str = System::GetDateTimeToString(vecAlmRec[i].ResolveTick / 1000);
						memcpy(tembuf, str.c_str(), str.size());
						tembuf += str.size();
						memset(tembuf++, 44, 1);
					}
					else
					{
						memset(tembuf++, 44, 1);
					}
					break;
				}
				}
			}
			//--tembuf;
			memcpy(tembuf, "\n", 2);
			tembuf += 2;
		}
		filePath = GetSavePath(res.StoreLocation, res.StorePosVarId, res.FileNameMode, res.FileName, res.FileNameDataVar, buflen,res.StoreSpaceLack);
		LocalExportFile(filePath, buff, buflen, res.IsFileSaveTimeLimit ? res.SaveDays : 0,0);
		IsAlarmNewTrig = false;
	}
	void FileSave::InsertSampleResInQueue(Project::SampleInfoRes * spIfRs)
	{
		if (!spIfRs->SampleStoreInfo.IsSave)
		{
			return;
		}
		SampleSaveList.push(spIfRs);
	}
	void FileSave::TryTrigSave(Project::SaveFileRes * res)
	{
		IsAlarmNewTrig = true;
		if (IsAlarmSaveThrRun)
			return;
		IsAlarmSaveThrRun = true;
		AlarmSaveConfig = res;
		////???????
		ThrSaveAlarm = new std::thread([&]
		{
			while (IsAlarmSaveThrRun)
			{
				if(IsAlarmNewTrig)
					FromSqlite2File(*AlarmSaveConfig);
#ifdef WIN32
				Sleep(1500);
#else
                usleep(1500000);
#endif
			}
		}
		);
		ThrSaveAlarm->detach();
	}
	void FileSave::StartKeepSave()
	{
		IsSampleSaveThrRun = true;
		ThrSaveSample = new std::thread([&]
		{
			while (IsSampleSaveThrRun)
			{
#ifdef WIN32
				Sleep(500);
#else
                usleep(500000);
#endif
				while (!SampleSaveList.empty())
				{
					auto itor = SampleSaveList.front();
					FromSqlite2File(*itor);
					SampleSaveList.pop();
				}
			}
		}
		);
		ThrSaveSample->detach();
	}

#ifdef WIN32
	BOOL get_disk_space(char driver, unsigned long long& allSpace, unsigned long long& freeSpace)
	{
		BOOL ret = false;
		DWORD dwSector;
		DWORD dwBytesInSec;
		DWORD dwCluster;
		DWORD dwFreeCluster;
		string sDriver;
		sDriver += driver;
		sDriver += ":\\";

		ret = GetDiskFreeSpace(sDriver.c_str(), &dwSector, &dwBytesInSec, &dwFreeCluster, &dwCluster);
		allSpace = 0;
		freeSpace = 0;

		allSpace = dwSector;
		allSpace *= dwBytesInSec;
		allSpace *= dwCluster;

		freeSpace = dwSector;
		freeSpace *= dwBytesInSec;
		freeSpace *= dwFreeCluster;

		return ret;
	}
#else
#endif

	int FileSave::IsDiskEnoughSpace(DWORD srcbytes, int StoreSpaceLack)
	{
		unsigned long long as, fs;
#ifdef WIN32
		string strcurdir = System::GetCurDir();
		char dv = strcurdir[0];// .substr(0, strcurdir.find(":"));
		get_disk_space(dv, as, fs);
#else
		struct statfs diskInfo;

		statfs("/app/run/", &diskInfo);
		unsigned long long blocksize = diskInfo.f_bsize;    //每个block里包含的字节数  
		as = blocksize * diskInfo.f_blocks;   //总的字节数，f_blocks为block的数目  
		printf("Total_size = %llu B = %llu KB = %llu MB = %llu GB\n",
			as, as >> 10, as >> 20, as >> 30);

		unsigned long long freeDisk = diskInfo.f_bfree * blocksize; //剩余空间的大小  
		fs = diskInfo.f_bavail * blocksize;   //可用空间大小  
		printf("Disk_free = %llu MB = %llu GB\nDisk_available = %llu MB = %llu GB\n",
			freeDisk >> 20, freeDisk >> 30, fs >> 20, fs >> 30);
#endif
		if (fs > srcbytes)
			return 0;
		else if (as <= srcbytes)
			return 2;
		else
			return 1;
	}

	int FileSave::GetRealChannelIdFromRecord(int channel, int group, int no)
	{
		//return channel ^ (group << 16 | no << 24);
		return channel & 0xff;
	}
	int FileSave::CopyIntegerToChar(char * ch, int value)
	{
		char digit[20];
		sprintf(digit, "%d", value);
		memcpy(ch, digit, strlen(digit));
		return strlen(digit);
	}
	int FileSave::CopyDataTypeStrToChar(char * dst, const Project::BaseVar * var)
	{
		if (!var)
			return 0;
		int pos = 0;
		switch (var->RegType)
		{
		case Project::LocalRegType::TP_PSB:
			memcpy(dst, "PSB", 3);
			pos += 3;
			break;
		case Project::LocalRegType::TP_SPSB:
			memcpy(dst, "SPSB", 4);
			pos += 4;
			break;
		case Project::LocalRegType::TP_PSW:
			memcpy(dst, "PSW", 3);
			pos += 3;
			break;
		case Project::LocalRegType::TP_SPSW:
			memcpy(dst, "SPSW", 4);
			pos += 4;
			break;
		case Project::LocalRegType::TP_PFW:
			memcpy(dst, "PFW", 3);
			pos += 3;
			break;
		case Project::LocalRegType::TP_SPFW:
			memcpy(dst, "SPFW", 4);
			pos += 4;
			break;
		default:
			break;
		}
		pos += CopyIntegerToChar(dst + pos, var->Addr);
		return pos;
	}
	string FileSave::GetSavePath(int pathMode, Project::DataVarId & addrPath, int nameMode, string fileName, Project::DataVarId & addrName,int needlen, int StoreSpaceLack)
	{
		std::string filePath = System::GetCurDir().append("\\");
		switch (pathMode)
		{
		case 1://Hmi
			filePath.append("HMI\\");
			break;
		case 2://SD
			filePath.append("SDCARD\\");
			break;
		case 3://USB
			filePath.append("USB\\");
			break;
		case 4:
			if (NULL_VID_VALUE != addrPath.Vid)
			{
				std::string strName = UI::DataApi::AppString(addrPath);
				if (strName.size())
				{
					filePath.append(strName).append("\\");
				}
				else
				{
					filePath.append("InvalidFilePathInDataVar").append("\\");
				}
			}
			break;
		}
		int spacemark = IsDiskEnoughSpace(needlen,StoreSpaceLack);
		if (spacemark==1)
		{
			switch (StoreSpaceLack)
			{
			case 0://停止保存
				return "";
				break;
			case 1://覆盖旧纪录
				string stroldfilename="";
				getFile(filePath, stroldfilename);
				if (!stroldfilename.compare(""))
					return "";
				else
					filePath.append(stroldfilename);
				break;
			}
			return filePath;
		}
		else if (spacemark == 2)
		{
			return "";
		}
		else
		{
			switch (nameMode)
			{
			case 1://固定文件名
				filePath.append(fileName);
				break;
			case 2://日期指定
				filePath.append(System::GetCurrentDateToString());
				break;
			case 3://动态文件名
				if (NULL_VID_VALUE != addrName.Vid)
				{
					filePath.append(UI::DataApi::AppString(addrName));
					//filePath.append("\\");
				}
				break;
			}
			filePath.append(".csv");
		}
		
		return filePath;
	}
}
