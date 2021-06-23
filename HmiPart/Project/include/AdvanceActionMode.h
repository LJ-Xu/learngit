#pragma once
#include "JudgeCondition.h"
#include "DataVarInfo.h"
namespace Project
{
	class AdvanceActionMode
	{
	public:
		int Trigmode;						//��������ģʽ 0:��Ȧ���� 1:��ֵ�߼�
		int BitTrigCondition;				//��Ȧ������ʽ	0�������� 1���½���  2�����ش���
		Conditions WordTrigCondition;		//��ֵ�߼�����
		double WordTrigValue;				//��ֵ�߼��Ա�ֵ
		DataVarId TrigDatavrIdRef;			//������ַ(λ����ֵ)
		bool Precoilstatus = false;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Trigmode, BitTrigCondition, WordTrigCondition, WordTrigValue, TrigDatavrIdRef);
		}
	};
}

