#pragma once
#include "stdafx.h"
#include "DataVarInfo.h"
#include "UIDataService.h"
#include "DataFormate.h"
using namespace Project;
namespace UI
{
	class UIData
	{
	public:
		template<typename T>
		static T Number(Project::DataVarId& vid)
		{
			XJDataType tp = UIDataService::Ins().GetDataType(vid);
			DDWORD da = UIDataService::Ins().GetNumber(vid);
			return Utility::DataFormate::GetNumber<T>(da, tp);
			// return UIDataService::Ins().GetNumber<T>(vid);
		}
		template<typename T>
		static void Number(Project::DataVarId& vid, T val)
		{
			XJDataType tp = UIDataService::Ins().GetDataType(vid);
			DDWORD da = Utility::DataFormate::SetNumber<T>(val, tp);
			UIDataService::Ins().SetNumber(vid, da);
			// UIDataService::Ins().SetNumber(vid, val);
		}
		static bool Bit(Project::DataVarId& vid);

		static void Bit(Project::DataVarId& vid, bool val);
	};

#define SYS_PSW_TIME_DEC				TP_SPSW,16				//ʮ����ϵͳʱ��
#define SYS_PSW_TIME_HEX				TP_SPSW,23				//ʮ������ϵͳʱ��
#define SYS_PSW_CURRENT_WINNO			TP_SPSW,30				//��ǰ���ں�
#define SYS_PSW_INPUTKEY_LOWERLMT		TP_SPSW,100				//���������ʾ���ޣ�ռ��4�Ĵ�����8btye��
#define SYS_PSW_INPUTKEY_UPPERERLMT		TP_SPSW,104				//���������ʾ���ޣ�ռ��4�Ĵ�����8btye��
#define SYS_PSW_INPUTKEY_CURVAL			TP_SPSW,108,16*2		//�������ʵʱ��ʾ����
#define SYS_PSW_PINYIN_CURVAL			TP_SPSW,124,8*2			//��ǰƴ��
#define SYS_PSW_LOGINName				TP_SPSW,132,8*2			//��ŵ�½�û���
#define SYS_PSW_LOGINPwd				TP_SPSW,140,8*2			//��ŵ�½����
#define SYS_PSW_LOGINFlag				TP_SPSW,148				//��½����״̬��־λ 0��δ���� 1����½�ɹ� 2����½ʧ�� 3��ע���ɹ� 4��ע��ʧ��
#define SYS_PSW_LoginedName				TP_SPSW,150,8*2			//��ǰ�Ե�½�û�
#define SYS_PSW_ConfirmOperate			TP_SPSW,158				//����ȷ�� 0:��ʱ 1:���� 2:ȷ�ϲ��� 3��ȡ������
#define SYS_PSW_OperateCtrlID           TP_SPSW,160				//ִ�в���ȷ�ϵĿؼ�ID
#define SYS_PSW_ConfirmTime				TP_SPSW,162				//����ȷ�ϵȴ�ʱ��
#define SYS_PSW_RecipeIndex				TP_SPSW,256				//�䷽����

#define SYS_PINYIN_PAGENUM				25900			//ƴ��������
#define SYS_FULLKEY_PAGENUM				25007			//ȫƴ���̱��

#define SYS_PSB_HASLOGINUSER			TP_SPSB,0		//�û�Ȩ�޵�¼��־λ
#define SYS_PSB_COMMPORT_FAIL			TP_SPSB,23		//ͨѶʧ��
#define SYS_PSB_DATASAMPLE_NOTICE		TP_SPSB,100		//�ɼ����ݸ���֪ͨ
#define SYS_PSB_ALARMRECORD_NOTICE		TP_SPSB,101		//�������ݸ���֪ͨ
#define SYS_PSB_LOGINUSER				TP_SPSB,102		//login������־λ
#define SYS_PSB_LOGOUTUSER				TP_SPSB,103		//logout������־λ
#define SYS_PSB_OperatorRECORD_NOTICE	TP_SPSB,104		//������¼���ݸ���֪ͨ
#define SYS_PSB_BEEP_DISABLE			TP_SPSB,105		//�������Ƿ�ʹ�� ON����ʹ�� OFF��ʹ��

//#define SYS_PSW_OperatorRECORD_CONTRONL	TP_SPSB,106		//������¼���Ʊ�־λ(Ŀǰû��)
#define SYS_PSB_ReciprRECORD_NOTICE		TP_SPSB,106		//�䷽��¼����֪ͨ��ȡ��
#define SYS_PSB_CSV_NOTICE				TP_SPSB,107		//CSV���ڵ��뵼��֪ͨ 0:δ�ڽ��� 1:������
#define SYS_PSB_ClearFlag				TP_SPSB,204		//���SPSB7��8��־λ����һ�����غ��ϵ���־λ��

}
