#pragma once
#include "BaseLine.h"
#include "AlarmSeekUnit.h"
namespace Project
{
	enum AlarmSeekMode { AlarmSeekDATE, AlarmSeekTIME, AlarmSeekGROUP, AlarmSeekNUM, AlarmSeekLEVEL, AlarmSeekREG };

	class AlarmTableUnit : public AlarmSeekUnit
	{
	public:
		int BackColor;			//������ɫ
		int TitleBgColor;		//���ⱳ����ɫ

		bool UseFrame;			//�Ƿ�ʹ�����
		BaseLine FrameStyle;	//�����ʽ

		bool UseGrid;			//�Ƿ�ʹ������
		BaseLine GridStyle;		//������ʽ

		int AlarmTextColor;		//��������ʱ������ɫ
		int AlarmBackColor;		//��������ʱ������ɫ

		bool UseConfirm;		//�Ƿ�ʹ�ñ���ȷ��
		int ConfirmTextColor;	//����ȷ��ʱ������ɫ
		int ConfirmBackColor;	//����ȷ��ʱ������ɫ

		bool UseRecover;		//�Ƿ�ʹ�ñ����ָ�
		int RecoverTextColor;	//�����ָ�ʱ������ɫ
		int RecoverBackColor;	//�����ָ�ʱ������ɫ

		AlarmSeekMode SearchMode;		//��ѯ��ʽ
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(BackColor, TitleBgColor, UseFrame, FrameStyle, UseGrid, GridStyle,
				AlarmTextColor, AlarmBackColor, UseConfirm, ConfirmTextColor, ConfirmBackColor,
				UseRecover, RecoverTextColor, RecoverBackColor, SearchMode);
		}
	};

}

