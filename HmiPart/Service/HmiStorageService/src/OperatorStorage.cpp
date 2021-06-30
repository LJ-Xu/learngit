/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : OperatorStorage.cpp
 * Author   : TangYao
 * Date     : 2021/02/24
 * Descript : ���ݲɼ��ӿ�
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
		// ���õ�ǰ����
		record.Date = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		// ���õ�ǰʱ��
		record.Time = record.Date;
		// �����û�����
		record.UserName = userName;
		// ���ô��ڱ��
		record.Window = win;
		// ���ö�������
		record.CtrlName = ctrlName;
		// ���ò�����¼
		switch (action)
		{
		case OperatorAction::OA_SET:		//����λ��ֵ
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
		case OperatorAction::OA_TOGGLE:		//λȡ��
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
		// ���ò�����ַ
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
		// ��Ӳ�����¼
		return OperatorStorageService::Ins()->InsertOperatorRecord(record);
	}

	int OperatorStorage::Trigger(string userName, string devName, size_t win, string ctrlName, Project::BaseVar var, char * orgVal, char * newVal)
	{
		OperatorRecord record;
		// ���õ�ǰ����
		record.Date = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		// ���õ�ǰʱ��
		record.Time = record.Date;
		// �����û�����
		record.UserName = userName;
		// ���ô��ڱ��
		record.Window = win;
		// ���ö�������
		record.CtrlName = ctrlName;
		// ���ò�����¼
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
		// ���ò�����ַ
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
		// ��Ӳ�����¼
		return OperatorStorageService::Ins()->InsertOperatorRecord(record);
	}

	int OperatorStorage::Trigger(string userName, size_t win, string ctrlName, int action, int startWin, int endWin) {
		OperatorRecord record;
		// ���õ�ǰ����
		record.Date = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		// ���õ�ǰʱ��
		record.Time = record.Date;
		// �����û�����
		record.UserName = userName;
		// ���ô��ڱ��
		record.Window = win;
		// ���ö�������
		record.CtrlName = ctrlName;
		// ���ò�����¼
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
		// ��Ӳ�����¼
		record.Address = "Full Screen Window";
		// ���ò�����Ϣ
		record.Information = "Window " + to_string(startWin) + "->" + to_string(endWin);
		return OperatorStorageService::Ins()->InsertOperatorRecord(record);
	}

	int OperatorStorage::Trigger(string userName, string devName, size_t win, string ctrlName, int action, Project::BaseVar var, string describe, int count)
	{
		OperatorRecord record;
		// ���õ�ǰ����
		record.Date = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		// ���õ�ǰʱ��
		record.Time = record.Date;
		// �����û�����
		record.UserName = userName;
		// ���ô��ڱ��
		record.Window = win;
		// ���ö�������
		record.CtrlName = ctrlName;
		// ���ò�����¼
		switch (action)
		{
		case OperatorAction::OA_UPLOAD:
		{
			record.Action = "Upload recipe";
			// ���ò�����Ϣ
			record.Information = "�ϴ�" + describe;
			break;
		}
		case OperatorAction::OA_DOWNLOND:
		{
			record.Action = "Download recipe";
			// ���ò�����Ϣ
			record.Information = "����" + describe;
			break;
		}
		default:
			break;
		}
		// ��Ӳ�����¼
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
	{ "SetCoil",       "������Ȧ" },
	{ "SetData",	   "��������" },
	{ "Arithmetic",    "��������" },
	{ "SendData",      "���ݴ���" },
	{ "SwitchScreen",  "�����л�" },
	{ "OpenWin",	   "���ô���" },
	{ "CloseWin",      "�رմ���" },
	{ "ImportCSVdata", "����CSV"  },
	{ "ExportCSVdata", "����CSV"  },
	{ "DownloadRecipe","�ϴ��䷽" },
	{ "UploadRecipe",  "�����䷽" },
	{ "CallbackFunc",  "��������" },
	{ "PrintScreen",   "�����ӡ" },
	};
	int OperatorStorage::Trigger(string userName, size_t win, string ctrlName, int action, vector<string>& describe)
	{
		OperatorRecord record;
		// ���õ�ǰ����
		record.Date = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		// ���õ�ǰʱ��
		record.Time = record.Date;
		// �����û�����
		record.UserName = userName;
		// ���ô��ڱ��
		record.Window = win;
		// ���ö�������
		record.CtrlName = ctrlName;
		// ���ò�����¼
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
		// ���ò�����Ϣ
		for (size_t i = 0; i < describe.size(); i++)
		{
			record.Information += FuncNameMap[describe[i]];
			if (i != describe.size() - 1)
				record.Information += "��";
		}
		record.Address = "";
		// ��Ӳ�����¼
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
