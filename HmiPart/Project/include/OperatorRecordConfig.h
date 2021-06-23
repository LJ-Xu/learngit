#pragma once
#include "AlarmDisplayConfig.h"
#include "MoveableDisplayUnit.h"
//#include "StringStyle.h"
#include "AlarmInfoRes.h"
#include <time.h>
using namespace std;
namespace Project
{
	struct OperationRecordPrint
	{
		bool IsEnableRecordSetPrint;			//�Ƿ����ô�ӡ����
		int PrintDirection;						//��ӡ���� 0:ˮƽ 1:��ֱ
		StringStyle PrintFontStyle;				//��ӡ��������
		int PrintRange;							//��ӡ��Χ 0:ʱ�� 1:����
		DDWORD PrintStartTime;					//��ʼʱ��
		DDWORD PrintEndTime;					//����ʱ��
		int PrintCount;							//��ӡ������������	
		DataVarId ControlAddress;				//���ƴ�ӡ��ַPSB
		int PrintCond;							//��ӡ���� 0:������ 1:�½���
		bool IsDisplayPrintDate;				//�Ƿ���ʾ��ӡʱ��
		TimeMode PrintTimeFormate;				//��ӡʱ��ʱ���ʽ
		DateMode PrintDateFormate;				//��ӡʱ�����ڸ�ʽ
		double Scaling;							//�������ű���
		vector<string> PrintItems;				//��ӡ��Ŀ
		int PrintSortMode;						//���� 0:ʱ��˳�� 1:ʱ������
		DateMode PrintItemDateFormate;			//��ӡ����ʱ���ʽ
		TimeMode PrintItemTimeFormate;			//��ӡ�������ڸ�ʽ

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(IsEnableRecordSetPrint, PrintDirection, PrintFontStyle, PrintRange, PrintStartTime, PrintCond,
				PrintEndTime, PrintCount, ControlAddress, IsDisplayPrintDate, PrintTimeFormate, PrintDateFormate,
				Scaling, PrintItems, PrintSortMode, PrintItemDateFormate, PrintItemTimeFormate);
		}
		void Parse(rapidjson::Value& jsonObj);

	};
	class OperatorRecordConfig : public MoveableDisplayUnit
	{
	public:
		bool Enable;							//�Ƿ����ò�����¼¼��
		DataVarId CmdIdRef;						//����Ĵ���
		DataVarId ResultIdRef;					//����Ĵ���
		DataVarId ControlIdRef;					//���ƼĴ��� 0:�ر� 1:����
		SaveFileRes SaveLst;					//������¼¼����Ϣ����
		OperationRecordPrint PrintSet;			//��ӡ����
		DataVarId RecordNoticeVarId;			//������¼֪ͨ�Ĵ���

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Enable, CmdIdRef, ResultIdRef, ControlIdRef, SaveLst, RecordNoticeVarId,
				PrintSet, VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}


