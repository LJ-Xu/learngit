#pragma once
#include <string>
#include "stdafx.h"
#include "ProjectPortVarsInfo.h"
#include "BaseRecord.h"
namespace Storage
{
	enum RecordEnum
	{
		HR_DATA_TOGGLE,//����
		HR_DATA_SET,  //����
		HR_WINDOW_CLOSE,//����ر�
		HR_WINDOW_OPEN, //�����
	};

	enum OperatorAction :int
	{
		OA_SET,		// ����ֵ
		OA_TOGGLE,	// λȡ��
		OA_CONST,	// ���ó���
		OA_STRING,	// �����ַ���
		OA_PREVWIN,	// ��ת��һ����
		OA_NEXTWIN,	// ��ת��һ����
		OA_UPLOAD,	//�ϴ��䷽
		OA_DOWNLOND,	//�����䷽
		OA_PRESS,		//����
		OA_RELEASE		//�ͷ�
	};

	struct OperatorRecord : BaseRecord
	{
		size_t ID;			// ��¼���
		uint64_t Date;		// ��������
		uint64_t Time;		// ����ʱ��
		string UserName;	// �û�����
		string Class;		// ����Ȩ��
		size_t Window;		// ��������
		string CtrlName;	// �ؼ�����
		string Comment;		// ��������
		string Action;		// ��������
		string Address;		// ������ַ
		string Information;	// ������Ϣ

		/*
		RecordEnum RecdTp;//��¼���
		//char Name[15];//�û�����
		unsigned int Tick;//s

		int UID;//�û�ID
		Project::DataVarId VarId;//������Ϣ
		Project::WinCtrlID WCId ;
		DDWORD val1;//old val
		DDWORD val2;//new val
		*/
	};

}