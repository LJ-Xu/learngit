#pragma once
#include "MoveableDisplayUnit.h"
#include "Point.h"
#include "ImageResId.h"
namespace Project
{
	enum PopMode {ONCE,CYCLE};
	class AlarmWinConfig : public MoveableDisplayUnit
	{
	public:
		int WinNO;				//���ڱ��
		string WinName;			//��������
		int GroupName;			//������
		int GroupNo;			//���
		PopMode ModePop;		//����һ��/����
		int CycleTime;			//��������
		bool AutoClose;			//����������رմ���
		bool AloneMode;			//¢��ģʽ
		bool CloseBtnMode;		//ʹ�ùرհ�ť

		int BgMode;				//0:ʹ�ñ�����ɫ	1:ʹ�ñ���ͼƬ
		int BgColor;			//������ɫ
		ImageResId BgPicKey;	//����ͼƬ
	};
}



