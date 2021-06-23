#pragma once
#include "MoveableDisplayUnit.h"
#include <string>
#include "BaseLine.h"
#include "ImageResId.h"
#include "MultiPilotLampConfig.h"

namespace Project
{
	class FanConfig : public MoveableDisplayUnit
	{
	public:
		int ActionMode;						//����ģʽ 0:һֱ��ת 1:�ܼĴ�������
		int RegMode;						//�Ĵ���ģʽ 0:�� 1:λ
		DataVarId ActTrigVar;				//����ģʽ�Ĵ�����ַ
		int RotateDir;						//��ת���� 0:���� 1:����
		double CycleTime;					//�л�ͼƬ���ٶ� ���100ms������1000ms
		DataVarId CycleTimeVarId;			//�л�ͼƬ���ٶȣ��Ĵ���ָ����
		vector<ImageResId>PicKeys;			//ͼƬ����
		int BitTrigCondition;				//λ������ʽ	0��ON 1��OFF 
		Conditions WordTrigCondition;		//��ֵ�߼�����
		double WordTrigValue;				//��ֵ�߼��Ա�ֵ
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ActionMode, RegMode, ActTrigVar, RotateDir, CycleTime,
				CycleTimeVarId, PicKeys, BitTrigCondition, WordTrigCondition, WordTrigValue,
				VOffX, VOffY, Width, Height, Perm, X, Y, IsEnable, CtrlName, IsRecord, CtrlName);
		}
	};
}


