#pragma once
#include "MoveableDisplayUnit.h"
#include "BtnUnit.h"
#include "KeypadConfig.h"
namespace Project
{
	class GBtnPilotLampConfig : public BtnUnit
	{
	public:
		bool RwDiffAddrUsed;
		BtnAction Action;		// ��ON  ��OFF  ȡ��  ˲ʱON
		int Flash;				//�Ƿ���˸	0:����˸
								//			1:ON״̬��˸
								//			2:OFF״̬��˸
		int Logic;				//��Ȧ״̬�����߼� 0:���߼� 1:���߼�
		float Speed;				//��˸�ٶ�(ms)
		bool UseSameAddr;		//��дʹ��ͬ��ַ
		DataVarId ReadVar;		//��ȡ��ַ״̬

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(RwDiffAddrUsed, Action, Flash, Logic, Speed, UseSameAddr,
				ReadVar, StatusCnt, PicKey, Txt, StrStyles, WriteVar,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
		/******************************************************/
		int CurrentFlash = 1;			//��ǰ��˸״̬	0:����
										//				1:ͼƬ
		//int CurrentStatus = 0;			//��ŵ�ǰ״̬(1:ON/0:OFF״̬)
		bool HasTimer;					//�Ƿ��Ѿ�����˸״̬��
	};
}


