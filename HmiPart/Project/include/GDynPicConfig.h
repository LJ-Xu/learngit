#pragma once
#include "MoveableDisplayUnit.h"
#include <string>
#include "ImageFormat.h"
#include "BaseLine.h"
#include "ImageResId.h"
namespace Project
{
	enum PicSwitchMode:char
	{
		Mode_OneWay = 0,	//����
		Mode_GondBack		//����
	};

	enum PicSwitchSeq:char
	{
		Seq_Order = 0,		//˳��
		Seq_Reverse,		//����
		Seq_Random			//���
	};
	class GDynPicConfig : public MoveableDisplayUnit
	{
	public:
		int CycleTime;						//ͼƬչʾһȦ������(����)
		DataVarId CycleTimeVarId;			//ͼƬչʾһȦ�����ڣ��Ĵ���ָ����
		PicSwitchMode SwitchMode;			//ͼƬ�л�ģʽ
		PicSwitchSeq SwitchSeq;				//ͼƬ�л�ģʽ
		std::string SwitchOrder;			//ͼƬ�л�˳��
		vector<ImageFormat>ImageFmtList;	//ͼƬ����
		vector<ImageResId>OriginalImages;	//ԭʼͼƬ����
		int PicNum;							//ͼƬ����
		bool UseStartSign;					//�Ƿ�ʹ�ÿ�ʼ�ź�
		bool UseStopSign;					//�Ƿ�ʹ�ý����ź�
		DataVarId StartSignVarId;			//��ʼ�ź�vid
		DataVarId StopSignVarId;			//�����ź�vid
		int StartTrigCondition;				//��ʼ�ź����� 0:������ 1:�½���
		int EndTrigCondition;				//�����ź����� 0:������ 1:�½���


		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(CycleTime, CycleTimeVarId, SwitchMode, SwitchSeq, SwitchOrder, OriginalImages, EndTrigCondition,
				ImageFmtList, PicNum, UseStartSign, UseStopSign, StartSignVarId, StopSignVarId,
				VOffX, VOffY, Width, Height, Perm, X, Y, IsEnable, CtrlName, StartTrigCondition
				, IsRecord, CtrlName);
		}

		int CurrentPic = 0;					//��ǰͼƬ
		int Direction = 0;					//����������		
	};
}


