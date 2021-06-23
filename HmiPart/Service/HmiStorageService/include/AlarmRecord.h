#pragma once
#include "stdafx.h"
#include "BaseRecord.h"
#include <iostream>
#include <string>
#include "AlarmInfoRes.h"
using namespace std;

namespace Storage
{
	struct AlarmRecord : BaseRecord
	{
		//int GroupName;		//����
		//int GroupNo;			//����
		//uint64_t StartTick;//������ʼʱ��
		//string Title; //����������
		//uint32_t Times;//���������Ĵ���
		//uint64_t ResolveTick;//�����ָ�ʱ��
		//uint64_t CheckTick;//����ȷ��ʱ��
		//Project::AlarmLevel Level;	//��������
		AlarmRecord() {}
		AlarmRecord(AlarmRecord&& ar)noexcept
		{
			AlarmGroup = ar.AlarmGroup;
			AlarmNo = ar.AlarmNo;
			StartTick = ar.StartTick;
			Title = std::move(ar.Title);
			Times = ar.Times;
			ResolveTick = ar.ResolveTick;
			CheckTick = ar.CheckTick;
			Level = ar.Level;
		}
		AlarmRecord(const AlarmRecord& ar)
		{
			AlarmGroup = ar.AlarmGroup;
			AlarmNo = ar.AlarmNo;
			StartTick = ar.StartTick;
			Title = ar.Title;
			Times = ar.Times;
			ResolveTick = ar.ResolveTick;
			CheckTick = ar.CheckTick;
			Level = ar.Level;
		}
		AlarmRecord& operator=(AlarmRecord&& ar)
		{
			AlarmGroup = ar.AlarmGroup;
			AlarmNo = ar.AlarmNo;
			StartTick = ar.StartTick;
			Title = std::move(ar.Title);
			Times = ar.Times;
			ResolveTick = ar.ResolveTick;
			CheckTick = ar.CheckTick;
			Level = ar.Level;
			return *this;
		}
		AlarmRecord& operator=(const AlarmRecord& ar)
		{
			AlarmGroup = ar.AlarmGroup;
			AlarmNo = ar.AlarmNo;
			StartTick = ar.StartTick;
			Title = ar.Title;
			Times = ar.Times;
			ResolveTick = ar.ResolveTick;
			CheckTick = ar.CheckTick;
			Level = ar.Level;
			return *this;
		}

		uint16_t AlarmGroup;	// �������
		uint16_t AlarmNo;		// �������
		size_t Times;			// ���������Ĵ���
		size_t Level;			// �����ȼ�
		uint64_t StartTick;		// ������ʼʱ��	
		uint64_t ResolveTick;	// �����ָ�ʱ��
		uint64_t CheckTick;		// ����ȷ��ʱ��
		string Title;			// ����������
	};

}