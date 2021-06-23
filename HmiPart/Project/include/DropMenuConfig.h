#pragma once
#include "MoveableDisplayUnit.h"
#include "StatusRes.h"
#include "ImageResId.h"
#include "StringStyle.h"
#include "NoticesAction.h"
namespace Project
{
	struct MenuContontLst
	{
		int Index;			//����
		DOUBLE Value;			//��Ӧֵ
		StatusRes Content;	//��ǩ����
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Index, Value, Content);
		}
	};
	struct HMICommand
	{
		string Name;
		string Cmd;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Name, Cmd);
		}
	};
	class DropMenuConfig : public MoveableDisplayUnit
	{
	public:
		int MenuMode;							//0:�����˵� 1:�嵥ѡ��
		DataVarId ReadVar;						//��ȡд���ַ
		int DataStyle;							//���ݸ�ʽ
		int MenuNum = 0;						//������Ŀ����
		int PopMode;							//������ʽ	0������ 1������
		int ContentMode;						//����ģʽ 0���û��༭ 1������ģʽ
		int ContentHeight = 18;						//�߶�
		vector<MenuContontLst> MenuInfo;		//�˵����ݼ���Ӧֵ
		vector<ImageResId> PicKey;				//����ͼƬ
		int RowHeight;							//�м��
		int HscollWidth = 16;					//���������
		int SelectColor;						//ѡ����Ŀ��ɫ
		int BgColor;							//������ɫ
		int FrameColor;							//�߿���ɫ
		vector<StringStyle> MenuStyle;			//�˵���������
		HMICommand HMICommandItem;				//��������
		Point BtnPosition;						//����λ��
		NoticesAction ErrorNotice;				//����֪ͨ

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(MenuMode, ReadVar, DataStyle, MenuNum, PopMode,	MenuInfo, 
				ContentMode, HMICommandItem, BtnPosition, PicKey, RowHeight, ContentHeight,
				SelectColor, BgColor, FrameColor, MenuStyle, ErrorNotice, 
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}


