#pragma once
#include "MoveableDisplayUnit.h"
#include "BtnUnit.h"
#include "HImage.h"
namespace Project
{
	struct StatusAction {
		int Id;		//״̬
		string Val;	//����ֵ
		ImageResId PicKey; //״̬��Ӧ���
		StringStyle StrStyles;//������ʽ
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Id, Val, PicKey, StrStyles);
		}
	};

	class MultiStatusKeypadConfig : public MoveableDisplayUnit
	{
	public:
		vector<StatusAction> Action;		//��������
		int DataVarType;					//0 : λ�Ĵ���
											//1 : �ּĴ���
		int BitNum;
		vector<DataVarId> RegVars;			//�Ĵ���Vid
		int StatusCnt;						//״̬����
		StatusRes Txt;						//��Ӧ����
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Action, DataVarType, RegVars, StatusCnt, Txt, BitNum,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}


