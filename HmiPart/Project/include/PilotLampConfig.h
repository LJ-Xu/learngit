#pragma once
#include "MoveableDisplayUnit.h"
#include "StringStyle.h"
#include "StatusRes.h"
#include "ImageResId.h"
#ifdef __linux
#ifdef Status
#undef Status
#endif
#endif
namespace Project
{
	class PilotLampConfig : public MoveableDisplayUnit
	{
	public:
		StatusRes Text;					//��������
		vector<StringStyle> Csses;		//������ʽ
		DataVarId Status;				//��ǰ״̬
		int Flash;						//�Ƿ���˸	0:����˸
										//			1:ON״̬��˸
										//			2:OFF״̬��˸
		int Logic;						//��Ȧ״̬�����߼� 1:���߼� 0:���߼�
		float Speed;						//��˸�ٶ�(��������)
		vector<ImageResId> StatusKeys;		//ͼƬ��Դ
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Text, Csses, Status, Flash, Logic, Speed, StatusKeys,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
		/******************************************************/
		int CurrentFlash = 1;			//��ǰ��˸״̬	0:����
										//				1:ͼƬ
		int CurrentStatus = 0;			//��ŵ�ǰ״̬(1:ON/0:OFF״̬)
		bool HasTimer;					//�Ƿ��Ѿ�����˸״̬��
	};

}

