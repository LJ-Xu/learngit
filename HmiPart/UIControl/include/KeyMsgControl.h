/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeyMsgControl.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ���ڼ�����Ϣ�ؼ��Ŀ��Ƶ�ͷ�ļ�������������Ϣ���ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "KeyMsgView.h"
#include "DataVarInfo.h"
#include "InputNumShowUnit.h"
namespace UI
{
#define KEY_DEL		   8		//���˰���
#define KEY_ESC		   27		//�˳�����
#define KEY_CAPS	   42		//�л���Сд����
#define KEY_ENTER	   13		//�س�����
#define KEY_MINUS	   44		//ȡ�෴��
#define KEY_CLR		   127 		//���
#define KEY_DOT		   46		//С���㰴��
	

	class KeyMessage
	{
	public:
		int SrcWinNo;		//ԴĿ��windowNo
		int DstWinNo;		//Ŀ��windowNo
		Project::WinCtrlID DstCtrlId;
		Project::WinCtrlID SrcCtrlId;
		Project::WinCtrlID PinYinCtrlId;	//ƴ������keyId
		Project::WinCtrlID InputCtrlId;	//�����ctrlId
		int InputType;		//���������	0����ֵ
							//				1���ַ�
							//				2������
		int KeypadPosx;		//���̴���xλ��
		int KeypadPosy;		//���̴���yλ��

		float UpperLmt;		//����ֵ
		float LowerLmt;		//����ֵ
		string FmtStr;		//����������ʽ
		Project::AccuracyParam Accuracy;		//��ֵ����򾫶ȹ���
		string Value;		//���䰴��ֵ(����pinyin)
		string Chinese;		//�����ַ�
	};

	class KeyMsgControl : public BaseControl
	{
	public:
		KeyMsgControl() :KeyMsgControl(nullptr) {}
		KeyMsgControl(HMIPage* );
		void CreateView()override;

		/*******************************************************************************
		 * Name     : HandleKeyMessage
		 * Descript : ������յİ�����Ϣ
		 * Input    : keyValue - ��������
		 * Output   :
		 * Note	    :
		 *******************************************************************************/
		void HandleKeyMessage(int keyValue);

		void OpenKeyWindow(int lparam);
		void InitKeyMsg(int lparam);
		void HandleKeyChar(int lparam);
		void HandleKeyNum(int lparam);
		void HandleChangePinyin(int lparam);
		KeyMessage CurrentMsg;		//��ŵ�ǰ������Ϣ
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
	private:
		std::shared_ptr<KeyMsgModel> mode_;
	};
}

