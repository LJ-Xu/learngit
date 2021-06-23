#pragma once
#include "MoveableDisplayUnit.h"
#include "ImageResId.h"
#include "BaseLine.h"
//#include "DataInfo.h"
namespace Project
{
	//enum DataCodeRule {
	//	DataASCII,
	//	DataUTF_8,
	//	DataUTF_16,
	//	DataGB2312
	//};
	struct DataCell
	{
		int CellType;						//������� 0:��ַ��� 1:�ı�
		string Text;						//�ı��ַ���

		bool IsEditable;					//�Ƿ�ɱ༭
		int AddrType;						//��ַ���� 0:��ֵ	1:�ַ���

		DataVarId DataAddrVar;				//��ȡ�Ĵ�����ַ
		bool IsFillZero;					//�Ƿ�ʹ��ǰ��0
		int IntegerNum;
		int DecimalNum;

		int RegCount;						//�Ĵ�����
		int Rule;							//�����ʽ

		bool IsNull;						//�Ƿ�Ϊ��
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(CellType, Text, IsEditable, AddrType, IsNull,
				DataAddrVar, IntegerNum, IntegerNum, DecimalNum, RegCount, Rule);
		}

		void Parse(rapidjson::Value& jsonObj);
		static void Parse(std::vector<DataCell>& vector, rapidjson::Value& jsonObj);
	};
	class DataTableUnit :public MoveableDisplayUnit
	{
	public:
		int Appearance;				//�����ʽ 0��ʹ��ͼƬ��ʽ 1��ʹ���Զ������
		ImageResId Key;				//���ͼƬ

		int TableBgColor;			//�б�����ɫ
		int TitleBgColor;			//���ⱳ����ɫ
		bool IsUseBorder;			//�Ƿ�ʹ�����
		BaseLine TableBorderStyle;	//�������ʽ
		BaseLine GridBorderStyle;	//��������ʽ
		bool IsRowDividLine;		//�Ƿ�ʹ����������
		bool IsColDividLine;		//�Ƿ�ʹ����������

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Appearance, Key, TableBgColor, TitleBgColor, IsUseBorder,
				TableBorderStyle, GridBorderStyle, IsRowDividLine, IsColDividLine );
		}
	};

}
