#pragma once
#include "MoveableDisplayUnit.h"
#include "StringStyle.h"
#include "StatusRes.h"
#include "ImageResId.h"
#include "JudgeCondition.h"

namespace Project
{
	struct GetNumWay
	{
		int Mode;				//��ȡ��ʽ	0:����
								//			1:�Ĵ�����ȡ
		double Value;
		DataVarId RegVar;		//�ԱȼĴ���Vid

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Mode, Value, RegVar);
		}
	};
	enum Logic
	{
		NONE,
		AND,
		OR
	};
	struct StatusCondition
	{
		int Type;					//0:��Χ	1:λ
		/*��Χ����*/
		Conditions ConditionA;		//����A
		Conditions ConditionB;		//����B
		Logic LogicCondition;		//AB֮������
		GetNumWay DataA;			//����A
		GetNumWay DataB;			//����B
		/*λ����*/
		int Offset;					//ƫ����
		int Trigger;				//0:ON  1:OFF	(0-15)
		/*����*/
		bool Flash;					//�Ƿ���˸
		float Speed;					//��˸�ٶ�

		StringStyle Css;			//������ʽ
		ImageResId StatusKey;		//ͼƬ��Դ
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Type, ConditionA, ConditionB, LogicCondition, Offset,
				Trigger, Flash, Speed, DataA, DataB, Css, StatusKey);
		}
	};
	class MultiPilotLampConfig : public MoveableDisplayUnit
	{
	public:
		int DataVarType;					//�Ĵ���ģʽ		0���ּĴ���
											//					1��λ�Ĵ���

		vector<DataVarId> RegVars;			//λ���ּĴ���Vid(���һ������)
		int StatusCnt;						//״̬����
		int BitNum;							//��λ���λ��
		int DataFormat;						//0:�з��� 1:�޷���
		StatusRes Text;						//��������

		vector<StatusCondition> Action;		//״̬��Ӧ
		StatusCondition ErrState;			//����״̬
		int InputErr;						//�Ƿ����봦��ʽ 0:��ʾ����״̬ 1:��ʾ��ǰ״̬ 
		bool UseErrNotice;					//�Ƿ�ʹ�ô���֪ͨ
		DataVarId InputErrVarRef;			//����֪ͨ�Ĵ���

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Text, StatusCnt, BitNum, Action, DataVarType, RegVars,
				ErrState, InputErr, InputErrVarRef, DataFormat, UseErrNotice,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}

	};

}

