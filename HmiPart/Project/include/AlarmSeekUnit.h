#pragma once
#include "DataVarInfo.h"
#include <time.h>
#include "MoveableDisplayUnit.h"
#include "AlarmInfoRes.h"
namespace Project
{
	//enum SeekMode { DATE, TIME, GROUP, NUM, LEVEL, REG };
	class AlarmSeekUnit : public MoveableDisplayUnit
	{
	public:
		bool EnableSearch;				//�Ƿ�ʹ�ò�ѯ����
		//SeekMode SearchMode;			//��ѯ��ʽ

		DataVarId SeekCtrlVar;			//���Ʋ�ѯ�Ĵ��� ON״̬��ѯģʽ
		/*���Ĵ���*/
		DataVarId SeekModeVar;			//���Ʋ�ѯ��ʽ�Ĵ���

		/*������*/
		DDWORD SearchDate;
		vector<DataVarId> SearchDateVarId;
		/*��ʱ��*/
		DDWORD SearchTimeStart;
		DDWORD SearchTimeEnd;
		vector<DataVarId> SearchTimeVar;
		/*������*/
		int SearchGroup;
		DataVarId SearchGroupVar;
		/*�����*/
		int SearchNo;
		DataVarId SearchNoVar;
		/*����������*/
		AlarmLevel SearchLevel;		
		DataVarId SearchLevelVar;

		/*��ͨ��*/
		int SearchChannelNo;
		DataVarId SearchChannelNoVar;

		/*���ؼ���*/
		string SearchKey;
		DataVarId SearchKeyVar;

		/*������*/
		double SearchData;
		DataVarId SearchDataVar;

		bool RegMode = false;
		vector<DataVarId> SearchRegVarId;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(EnableSearch, SeekModeVar, SeekCtrlVar, SearchDate, SearchDateVarId,
				SearchTimeStart, SearchTimeEnd, SearchTimeVar, SearchGroup, SearchGroupVar,
				SearchNo, SearchNoVar, SearchLevel, SearchLevelVar, SearchChannelNo, 
				SearchChannelNoVar, SearchKey, SearchKeyVar, SearchData, SearchDataVar,
				RegMode, SearchRegVarId);
		}

	};
}


