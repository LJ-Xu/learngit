#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
#include "StringStyle.h"
#include "ImageResId.h"

namespace Project
{
	class MotorConfig : public MoveableDisplayUnit
	{
	public:
		////���󣨵�ַ���ݣ�
		DataVarId DataCtlSign; //��ȡ��ַ:�����ź�
		int CtlLogic;//�߼��������߼� 0 Ϊ���߼� 1Ϊ���߼�
		///��˸
		bool UseBlink;//������˸
		int BlinkState;//��˸״̬:0ΪOn״̬��˸��1ΪOff״̬��˸
		float BlinkFrequency;//��˸Ƶ��

		////���
		ImageResId KeyImgOn;//���on״̬��ͼƬ
		ImageResId KeyImgOff;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive( X, Y, VOffX, VOffY, Perm, Width, Height, CtrlName, DataCtlSign, CtlLogic, UseBlink, BlinkState, BlinkFrequency, KeyImgOn, KeyImgOff);
		}
	public:
		int CurrentStatus_;
	};
}