#pragma once
#include "MacroRes.h"
#include "MoveableDisplayUnit.h"
#include <vector>
#include "Point.h"
#include "ImageResId.h"
namespace Project
{
	struct Pipe
	{
	public:
		int PipeWidth;		//���ܿ��
		int PipeColor;		//������ɫ
		int FrameWidth;		//�߿���
		int FrameColor;		//�߿���ɫ
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(PipeWidth, PipeColor, FrameWidth, FrameColor);
		}
	};

	class Slider
	{
	public:
		int SliderType;				//0:����;1:��ͷ
		int SliderWidth;			//���
		int SliderLength;			//����
		int SliderIntervalWidget;	//���
		int ArrowAngle = 6;				//��ͷ��б��
		int SliderColorON;			//ON״̬������ɫ
		int SliderColorOFF;			//OFF״̬������ɫ

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(SliderType, SliderWidth, SliderLength, SliderIntervalWidget,
				ArrowAngle, SliderColorON, SliderColorOFF);
		}
	};

	class FluidUnitConfig : public MoveableDisplayUnit
	{
	public:
		int ActionMode;					//����ģʽ 0:һֱ������1:��λ����
		int FlowCondition;				//�������� 0:ON 1:OFF
		DataVarId FlowVarId;			//��ȡ��ַ
		int FlowDirection;				//���� 0:���� 1:����
		double FlowSpeed;				//�����ٶ� 1-25���ȼ���ӦΪ1.5s-60ms
		DataVarId SpeedVarId;
		int PointNum;					//�������
		vector<Point> Coordinates;		//�����
		Pipe PipeParam;					//���ڹܵ�����
		Slider SliderParam;				//���ڻ�������

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ActionMode, FlowCondition, FlowVarId, FlowDirection, FlowSpeed, 
				SpeedVarId,	Coordinates, PipeParam, SliderParam, 
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}

