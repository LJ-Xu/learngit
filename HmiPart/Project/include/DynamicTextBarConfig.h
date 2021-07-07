#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
#include "StringStyle.h"
#include "ImageResId.h"

namespace Project
{
	//��ʾ����
	struct  DynamicShowContext
	{
		std::vector<std::pair<std::string, int>> TextList;
		int ShowMode;//��ʾģʽ 0��������ʾ 1����Ȧ����
		DataVarId TriggerAddr;//���Ƶ�ַ
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(TextList, ShowMode, TriggerAddr);
		}
	};
	class DynamicTextBarConfig : public MoveableDisplayUnit
	{
	public:
		std::vector < DynamicShowContext> contextList;
		StringStyle Font;//�̶�����
		int MsgInstance;//ÿ����Ϣ���
		int RollSpeed;//�����ٶ� ��ÿ0.1S����������ֵ��
		bool ShowFrame;//�Ƿ���ʾ�߿�
		BaseLine FrameStyle;//�߿���ʽ
		bool ShowFillin;//��ʾ���
		int FadeoutPecent;//͸���Ȱٷֱȣ������ʱ�Ȳ�����
		int FillColor;//�����ɫ

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(X, Y, VOffX, VOffY, Perm, Width, Height, CtrlName, DataCtlSign, contextList, Font, MsgInstance, RollSpeed, ShowFrame, FrameStyle, ShowFillin, FadeoutPecent, FillColor);
		}
	};
}