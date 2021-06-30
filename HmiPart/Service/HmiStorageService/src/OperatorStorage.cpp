/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : OperatorStorage.cpp
 * Author   : TangYao
 * Date     : 2021/02/24
 * Descript : 数据采集接口
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "OperatorStorage.h"
#include "HMIProject.h"

namespace Storage
{
	OperatorStorage * OperatorStorage::instance_ = nullptr;

	OperatorStorage * OperatorStorage::Ins() {
		static std::once_flag flag;
		call_once(flag, [&]() {
			instance_ = new OperatorStorage();
		});
		return instance_;
	}

	int OperatorStorage::Trigger(string userName, string devName, size_t win, string ctrlName, int action, Project::BaseVar var, double orgVal, double newVal, XJDataType tp) {
		OperatorRecord record;
		// 设置当前日期
		record.Date = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		// 设置当前时间
		record.Time = record.Date;
		// 设置用户名称
		record.UserName = userName;
		// 设置窗口编号
		record.Window = win;
		// 设置对象名称
		record.CtrlName = ctrlName;
		// 设置操作记录
		switch (action)
		{
		case OperatorAction::OA_SET:		//设置位或值
		{
			if (var.DataType == Project::VarDataType::DT_Bit)
			{
				record.Action = "Set" + (int)newVal ? "ON" : "OFF";
				record.Information = "Bit Set " + (int)newVal ? "ON" : "OFF";
			}
			else
			{
				record.Action = "Set Word";
				if(tp.Type == Project::VarNumberType::NT_Float)
					record.Information = "Write " + to_string(orgVal) + "->" + to_string(newVal);
				else
					record.Information = "Write " + to_string((int)orgVal) + "->" + to_string((int)newVal);
			}
			break;
		}
		case OperatorAction::OA_TOGGLE:		//位取反
		{
			record.Action = "Toggle";
			record.Information = "Bit Set ";
			record.Information += (int)orgVal ? "ON->" : "OFF->";
			record.Information += (int)newVal ? "ON" : "OFF";
			break;
		}
		case OperatorAction::OA_CONST:
		{
			record.Action = "Write Const Value";
			if (tp.Type == Project::VarNumberType::NT_Float)
				record.Information = "Write " + to_string(newVal);
			else
				record.Information = "Write " + to_string((int)newVal);
			//record.Information = "Write " + to_string(newVal);
			break;
		}
		default:
			break;
		}
		// 设置操作地址
		string regName;
		switch (var.RegType)
		{
		case Project::LocalRegType::TP_PSB:
			regName = "PSB";
			break;
		case Project::LocalRegType::TP_SPSB:
			regName = "SPSB";
			break;
		case Project::LocalRegType::TP_PSW:
			regName = "PSW";
			break;
		case Project::LocalRegType::TP_SPSW:
			regName = "SPSW";
			break;
		case Project::LocalRegType::TP_PFW:
			regName = "PFW";
			break;
		case Project::LocalRegType::TP_SPFW:
			regName = "SPFW";
			break;
		default:
			break;
		}
		record.Address = devName + " : " + regName + to_string(var.Addr);
		// 添加操作记录
		return OperatorStorageService::Ins()->InsertOperatorRecord(record);
	}

	int OperatorStorage::Trigger(string userName, string devName, size_t win, string ctrlName, Project::BaseVar var, char * orgVal, char * newVal)
	{
		OperatorRecord record;
		// 设置当前日期
		record.Date = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		// 设置当前时间
		record.Time = record.Date;
		// 设置用户名称
		record.UserName = userName;
		// 设置窗口编号
		record.Window = win;
		// 设置对象名称
		record.CtrlName = ctrlName;
		// 设置操作记录
		record.Action = "Write String";
		string orgstring = "", newstring = "";
		std::stringstream orgss, newss;
		for (size_t i = 0; i < var.Count; i++)
		{
			const std::string hex = "0123456789ABCDEF";
			//for (std::string::size_type i = 0; i < str.size(); ++i)
			orgss << hex[(unsigned char)orgVal[i] >> 4] << hex[(unsigned char)orgVal[i] & 0xf] << " ";
			newss << hex[(unsigned char)newVal[i] >> 4] << hex[(unsigned char)newVal[i] & 0xf] << " ";
			//char orgbuf[3], newbuf[3];;
			//sprintf(orgbuf, "%02x", orgVal[i]);
			//orgstring += orgbuf;
			//sprintf(newbuf, "%02x", newVal[i]);
			//newstring += newbuf;
		}
		orgstring = orgss.str();
		newstring = newss.str();

		record.Information = "Write " + orgstring + "->" + newstring;
		// 设置操作地址
		int count = var.Count / 2;
		string regName = "PSW";
		switch (var.RegType)
		{
		case Project::LocalRegType::TP_PSB:
			regName = "PSB";
			break;
		case Project::LocalRegType::TP_SPSB:
			regName = "SPSB";
			break;
		case Project::LocalRegType::TP_PSW:
			regName = "PSW";
			break;
		case Project::LocalRegType::TP_SPSW:
			regName = "SPSW";
			break;
		case Project::LocalRegType::TP_PFW:
			regName = "PFW";
			break;
		case Project::LocalRegType::TP_SPFW:
			regName = "SPFW";
			break;
		default:
			break;
		}
		record.Address = devName + " : " + regName + to_string(var.Addr) + "(" + to_string(count) + "words" + ")";
		// 添加操作记录
		return OperatorStorageService::Ins()->InsertOperatorRecord(record);
	}

	int OperatorStorage::Trigger(string userName, size_t win, string ctrlName, int action, int startWin, int endWin) {
		OperatorRecord record;
		// 设置当前日期
		record.Date = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		// 设置当前时间
		record.Time = record.Date;
		// 设置用户名称
		record.UserName = userName;
		// 设置窗口编号
		record.Window = win;
		// 设置对象名称
		record.CtrlName = ctrlName;
		// 设置操作记录
		switch (action)
		{
		case OperatorAction::OA_PREVWIN:
			record.Action = "Return To Prev Window";
			break;
		case OperatorAction::OA_NEXTWIN:
			record.Action = "Go To Next Windown";
			break;
		default:
			break;
		}
		// 添加操作记录
		record.Address = "Full Screen Window";
		// 设置操作信息
		record.Information = "Window " + to_string(startWin) + "->" + to_string(endWin);
		return OperatorStorageService::Ins()->InsertOperatorRecord(record);
	}

	int OperatorStorage::Trigger(string userName, string devName, size_t win, string ctrlName, int action, Project::BaseVar var, string describe, int count)
	{
		OperatorRecord record;
		// 设置当前日期
		record.Date = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		// 设置当前时间
		record.Time = record.Date;
		// 设置用户名称
		record.UserName = userName;
		// 设置窗口编号
		record.Window = win;
		// 设置对象名称
		record.CtrlName = ctrlName;
		// 设置操作记录
		switch (action)
		{
		case OperatorAction::OA_UPLOAD:
		{
			record.Action = "Upload recipe";
			// 设置操作信息
			record.Information = "上传" + describe;
			break;
		}
		case OperatorAction::OA_DOWNLOND:
		{
			record.Action = "Download recipe";
			// 设置操作信息
			record.Information = "下载" + describe;
			break;
		}
		default:
			break;
		}
		// 添加操作记录
		string regName = "PSW";
		switch (var.RegType)
		{
		case Project::LocalRegType::TP_PSB:
			regName = "PSB";
			break;
		case Project::LocalRegType::TP_SPSB:
			regName = "SPSB";
			break;
		case Project::LocalRegType::TP_PSW:
			regName = "PSW";
			break;
		case Project::LocalRegType::TP_SPSW:
			regName = "SPSW";
			break;
		case Project::LocalRegType::TP_PFW:
			regName = "PFW";
			break;
		case Project::LocalRegType::TP_SPFW:
			regName = "SPFW";
			break;
		default:
			break;
		}
		record.Address = devName + " : " + regName + to_string(var.Addr) + "(" + to_string(count) + "words" + ")";
		return OperatorStorageService::Ins()->InsertOperatorRecord(record);
	}

	static map<string, string> FuncNameMap = {
	{ "SetCoil",       "设置线圈" },
	{ "SetData",	   "设置数据" },
	{ "Arithmetic",    "四则运算" },
	{ "SendData",      "数据传输" },
	{ "SwitchScreen",  "画面切换" },
	{ "OpenWin",	   "调用窗口" },
	{ "CloseWin",      "关闭窗口" },
	{ "ImportCSVdata", "导入CSV"  },
	{ "ExportCSVdata", "导出CSV"  },
	{ "DownloadRecipe","上传配方" },
	{ "UploadRecipe",  "下载配方" },
	{ "CallbackFunc",  "函数调用" },
	{ "PrintScreen",   "画面打印" },
	};
	int OperatorStorage::Trigger(string userName, size_t win, string ctrlName, int action, vector<string>& describe)
	{
		OperatorRecord record;
		// 设置当前日期
		record.Date = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		// 设置当前时间
		record.Time = record.Date;
		// 设置用户名称
		record.UserName = userName;
		// 设置窗口编号
		record.Window = win;
		// 设置对象名称
		record.CtrlName = ctrlName;
		// 设置操作记录
		switch (action)
		{
		case OperatorAction::OA_PRESS:
			record.Action = "Press";
			break;
		case OperatorAction::OA_RELEASE:
			record.Action = "Release";
			break;
		default:
			break;
		}
		// 设置操作信息
		for (size_t i = 0; i < describe.size(); i++)
		{
			record.Information += FuncNameMap[describe[i]];
			if (i != describe.size() - 1)
				record.Information += "、";
		}
		record.Address = "";
		// 添加操作记录
		return OperatorStorageService::Ins()->InsertOperatorRecord(record);
	}

	int OperatorStorage::CleanRcd()
	{
		return OperatorStorageService::Ins()->DeleteOperatorRecords();
	}

	void OperatorStorage::Rcd(RecordEnum recdEm, char* uname, char* addrname, 
								char* ctrlname, char* comt, char* data)
	{
	
	}

	void OperatorStorage::Rcd(RecordEnum recdEm, unsigned int uid, unsigned int vid,
								unsigned int ctrlid, DDWORD oldval, DDWORD newval)
	{
	
	}

	void OperatorStorage::QueryByTime(DDWORD stime, DDWORD etime, vector<OperatorRecord> & records)
	{
		OperatorStorageService::Ins()->SelectOperatorRecordsByTime(stime, etime, records);
	}

	void OperatorStorage::QueryByDate(DDWORD sdate, DDWORD edate, vector<OperatorRecord> & records)
	{
		OperatorStorageService::Ins()->SelectOperatorRecordsByDate(sdate, edate, records);
	}

	void OperatorStorage::QueryByUser(char* name, vector<OperatorRecord> & records)
	{
		OperatorStorageService::Ins()->SelectOperatorRecordsByUser(name, records);
	}
 
}
