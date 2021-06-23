#include "stdafx.h"
#include "GStickChartModel.h"
#include <FL/Enumerations.H>

#include "JsonComm.h"
using namespace rapidjson;
namespace UI
{
	void GStickChartModel::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());

		if (json.HasParseError())	//�жϽ����Ƿ���ȷ
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
			return;
		}

		StickChartConfig.InitBaseData(json);
		////���󣨵�ַ���ݣ�

		//����
		if (json.HasMember("BarVar") && json["BarVar"].IsObject())
			StickChartConfig.BarVar.Parse(json["BarVar"]);
		////��Χ����Χ��
		///���ֵ
		//���ֵ(����)
		if (json.HasMember("MaxValue"))
			StickChartConfig.MaxValue = json["MaxValue"].GetInt();
		//���ֵ(��ַ)
		if (json.HasMember("MaxData") && json["MaxData"].IsObject())
			StickChartConfig.MaxData.Parse(json["MaxData"]);
		//���ֵ�Ƿ��ɼĴ���ָ�� 
		if (json.HasMember("UseMaxAddr"))
			StickChartConfig.UseMaxAddr = json["UseMaxAddr"].GetBool();

		///��Сֵ
		//��Сֵ��������
		if (json.HasMember("MinValue"))
			StickChartConfig.MinValue = json["MinValue"].GetInt();
		//��Сֵ(��ַ)
		if (json.HasMember("MinData") && json["MinData"].IsObject())
			StickChartConfig.MinData.Parse(json["MinData"]);
		//��Сֵ�Ƿ��ɼĴ���ָ��
		if (json.HasMember("UseMinAddr"))
			StickChartConfig.UseMinAddr = json["UseMinAddr"].GetBool();

		///Ŀ������
		//Ŀ��ֵ��������
		if (json.HasMember("DstValue"))
			StickChartConfig.DstValue = json["DstValue"].GetInt();
		//Ŀ��ֵ(��ַ)
		if (json.HasMember("DstValueAddr") && json["DstValueAddr"].IsObject())
			StickChartConfig.DstValueAddr.Parse(json["DstValueAddr"]);
		//ʹ�üĴ���ָ��Ŀ��ֵ
		if (json.HasMember("UseDstField"))
			StickChartConfig.UseDstField = json["UseDstField"].GetBool();
		//��Χ��������
		if (json.HasMember("DstRange"))
			StickChartConfig.DstRange = json["DstRange"].GetInt();
		//��Χ(��ַ)
		if (json.HasMember("DstRangeAddr") && json["DstRangeAddr"].IsObject())
			StickChartConfig.DstRangeAddr.Parse(json["DstRangeAddr"]);
		//ʹ�üĴ���ָ��Ŀ��ֵ
		if (json.HasMember("UseDstRangeByAddr"))
			StickChartConfig.UseDstRangeByAddr = json["UseDstRangeByAddr"].GetBool();
		//���������ɫ
		if (json.HasMember("DstColor"))
			StickChartConfig.DstColor = json["DstColor"].GetInt();

		///��Χ����
		//���÷�Χ����
		if (json.HasMember("UseRangeWarn"))
			StickChartConfig.UseRangeWarn = json["UseRangeWarn"].GetBool();

		//�Ͼ���ֵ��������
		if (json.HasMember("UpperWarnValue"))
			StickChartConfig.UpperWarnValue = json["UpperWarnValue"].GetInt();
		//�Ͼ���ֵ(��ַ)
		if (json.HasMember("UpperWarnAddr") && json["UpperWarnAddr"].IsObject())
			StickChartConfig.UpperWarnAddr.Parse(json["UpperWarnAddr"]);
		//�Ĵ���ָ���Ͼ���ֵ
		if (json.HasMember("UseUpperWarnAddr"))
			StickChartConfig.UseUpperWarnAddr = json["UseUpperWarnAddr"].GetBool();
		//ǰ��ɫ
		if (json.HasMember("UpperWarnFillinColor"))
			StickChartConfig.UpperWarnFillinColor = json["UpperWarnFillinColor"].GetInt();

		
		//�¾���ֵ��������
		if (json.HasMember("LowerWarnValue"))
			StickChartConfig.LowerWarnValue = json["LowerWarnValue"].GetInt();
		//�¾���ֵ(��ַ)
		if (json.HasMember("LowerWarnAddr") && json["LowerWarnAddr"].IsObject())
			StickChartConfig.LowerWarnAddr.Parse(json["LowerWarnAddr"]);
		//�Ĵ���ָ���¾���ֵ
		if (json.HasMember("UseLowerWarnAddr"))
			StickChartConfig.UseLowerWarnAddr = json["UseLowerWarnAddr"].GetBool();
		//ǰ��ɫ
		if (json.HasMember("LowerWarnFillinColor"))
			StickChartConfig.LowerWarnFillinColor = json["LowerWarnFillinColor"].GetInt();

		////���
		//��ǰѡ�����״��0ֱ����1����
		if (json.HasMember("Shape"))
			StickChartConfig.Shape = json["Shape"].GetInt();

		if (json.HasMember("IsUserDefineBar"))
		{
			StickChartConfig.IsUserDefineBar = json["IsUserDefineBar"].GetBool();
			if (json.HasMember("BarAreaRectanglePointX") )
			{
				StickChartConfig.BarAreaRectanglePointX = json["BarAreaRectanglePointX"].GetInt(); 
			}
			if (json.HasMember("BarAreaRectanglePointY"))
			{
				StickChartConfig.BarAreaRectanglePointY = json["BarAreaRectanglePointY"].GetInt();
			}

			if (json.HasMember("BarAreaRectangleSizeWidth"))
			{
				StickChartConfig.BarAreaRectangleSizeWidth = json["BarAreaRectangleSizeWidth"].GetInt();
			}

			if (json.HasMember("BarAreaRectangleSizeHeight"))
			{
				StickChartConfig.BarAreaRectangleSizeHeight = json["BarAreaRectangleSizeHeight"].GetInt();
			}
		}


		///ֱ��
		//����0�ϣ�1�£�2��3��
		if (json.HasMember("BarDirection"))
			StickChartConfig.BarDirection = json["BarDirection"].GetInt();

		///����
		//���⻷����
		if (json.HasMember("ArcInnerCirclePercent"))
			StickChartConfig.ArcInnerCirclePercent = json["ArcInnerCirclePercent"].GetInt();
		//��ʼ�Ƕ�
		if (json.HasMember("ArcStartAngle"))
			StickChartConfig.ArcStartAngle = json["ArcStartAngle"].GetInt();
		//�յ�Ƕ�
		if (json.HasMember("ArcSweepAngle"))
			StickChartConfig.ArcSweepAngle = json["ArcSweepAngle"].GetInt();
		//����0˳ʱ�룻1��ʱ��
		if (json.HasMember("ArcDirection"))
			StickChartConfig.ArcDirection = json["ArcDirection"].GetInt();

		///�߿�
		//��ʾ�߿�
		if (json.HasMember("ShowFrame"))
			StickChartConfig.ShowFrame = json["ShowFrame"].GetBool();
		//ǰ��ɫ
		if (json.HasMember("ColorShowFrame"))
			StickChartConfig.ColorShowFrame = json["ColorShowFrame"].GetInt();

		///����
		//��ʾ����ɫ
		if (json.HasMember("ShowBackground"))
			StickChartConfig.ShowBackground = json["ShowBackground"].GetBool();
		//����ɫ
		if (json.HasMember("ColorBack"))
			StickChartConfig.ColorBack = json["ColorBack"].GetInt();

		///���
		//���ɫ
		if (json.HasMember("FIllinColor"))
			StickChartConfig.FIllinColor = json["FIllinColor"].GetInt();

		///ͼ�����
		////ͼ�����
		if (json.HasMember("UseFillinStyle"))
			StickChartConfig.UseFillinStyle = json["UseFillinStyle"].GetBool();
		//��ʽ
		if (json.HasMember("FillinStyle"))
			StickChartConfig.FillinStyle = json["FillinStyle"].GetInt();
		//ǰ��ɫ
		if (json.HasMember("ColorFillinStyle"))
			StickChartConfig.ColorFillinStyle = json["ColorFillinStyle"].GetInt();

		///����
		//����
		if (json.HasMember("UseFillinShade"))
			StickChartConfig.UseFillinShade = json["UseFillinShade"].GetBool();
		//��ʽ
		if (json.HasMember("ShadeStyle"))
			StickChartConfig.ShadeStyle = json["ShadeStyle"].GetInt();
		//ǰ��ɫ
		if (json.HasMember("ColorShadeStyle"))
			StickChartConfig.ColorShadeStyle = json["ColorShadeStyle"].GetInt();
		//͸����
		if (json.HasMember("FadeoutPercent"))
			StickChartConfig.FadeoutPercent = json["FadeoutPercent"].GetInt();


		////�̶�����
		///�̶�
		//��ʾ�̶�
		if (json.HasMember("ShowGraduate"))
			StickChartConfig.ShowGraduate = json["ShowGraduate"].GetBool();
		//�̶����ͼ�����λ��
		if (json.HasMember("RelatedGraduateSeat"))
			StickChartConfig.RelatedGraduateSeat = json["RelatedGraduateSeat"].GetInt();
		//���̶���
		if (json.HasMember("MainGraduateCount"))
			StickChartConfig.MainGraduateCount = json["MainGraduateCount"].GetInt();
		//���̶ȳ���
		if (json.HasMember("MainGraduateLen"))
			StickChartConfig.MainGraduateLen = json["MainGraduateLen"].GetInt();
		//���̶���
		if (json.HasMember("SecondaryGraduateCount"))
			StickChartConfig.SecondaryGraduateCount = json["SecondaryGraduateCount"].GetInt();
		//���̶ȳ���
		if (json.HasMember("SecondaryGraduateLen"))
			StickChartConfig.SecondaryGraduateLen = json["SecondaryGraduateLen"].GetInt();

		///�̶���
		//�̶��߳ߴ�
		if (json.HasMember("LineColor"))
			StickChartConfig.LineColor = json["LineColor"].GetInt();
		//�̶�����ʽ
		if (json.HasMember("LineStyle"))
			StickChartConfig.LineStyle = json["LineStyle"].GetInt();
		//�̶��߿��
		if (json.HasMember("LineWidget"))
			StickChartConfig.LineWidget = json["LineWidget"].GetInt();


		///���ֱ��
		//��ʾ���ֱ��
		if (json.HasMember("ShowGraduateSign"))
			StickChartConfig.ShowGraduateSign = json["ShowGraduateSign"].GetBool();
		//����λ
		if (json.HasMember("IntegralDigit"))
			StickChartConfig.IntegralDigit = json["IntegralDigit"].GetInt();
		//С��λ
		if (json.HasMember("DecimalDigit"))
			StickChartConfig.DecimalDigit = json["DecimalDigit"].GetInt();
		//�̶�����
		if (json.HasMember("TypeFaceGraduate") && json["TypeFaceGraduate"].IsObject())
			StickChartConfig.TypeFaceGraduate.Parse(json["TypeFaceGraduate"]);

		///�ٷֱ�
		if (json.HasMember("ShowPercent"))
			StickChartConfig.ShowPercent = json["ShowPercent"].GetBool();
		//�̶�����
		if (json.HasMember("TypeFacePecent") && json["TypeFacePecent"].IsObject())
			StickChartConfig.TypeFacePecent.Parse(json["TypeFacePecent"]);
		//��ʾ����
		if (json.HasMember("ShowAxis"))
			StickChartConfig.ShowAxis = json["ShowAxis"].GetBool();
		if (json.HasMember("BarRes"))
			StickChartConfig.BarRes.Parse(json["BarRes"]);

		if (json.HasMember("LinearGradientRes"))
			StickChartConfig.LinearGradientRes.Parse(json["LinearGradientRes"]);
	}

	void GStickChartModel::InitTestData()
	{
		StickChartConfig.X = 30;
		StickChartConfig.Y = 30;
		StickChartConfig.Width = 150;
		StickChartConfig.Height = 150;
		StickChartConfig.IsEnable = false;
		StickChartConfig.CtrlName = "StickChart0";


		////��Χ����Χ��
		///���ֵ
		//���ֵ(����)
		StickChartConfig.MaxValue = 100;
		//���ֵ(��ַ)
		//m_StickChartConfig.MaxData.Parse(json["MaxData"]);
		//���ֵ�Ƿ��ɼĴ���ָ�� 
		StickChartConfig.UseMaxAddr = false;

		///��Сֵ
		//��Сֵ��������
		StickChartConfig.MinValue = 0;
		//��Сֵ(��ַ)
		//m_StickChartConfig.MinData.Parse(json["MinData"]);
		//��Сֵ�Ƿ��ɼĴ���ָ��
		StickChartConfig.UseMinAddr = false;


		//m_StickChartConfig.BarVar.Parse(json["BarVar"]);
		///Ŀ������
		//Ŀ��ֵ��������
		StickChartConfig.DstValue = 50;
		//Ŀ��ֵ(��ַ)
		//m_StickChartConfig.DstValueAddr.Parse(json["DstValueAddr"]);
		//ʹ�üĴ���ָ��Ŀ��ֵ
		StickChartConfig.UseDstField = false;
		//��Χ��������
		StickChartConfig.DstRange = 0;
		//��Χ(��ַ)
		//m_StickChartConfig.DstRangeAddr.Parse(json["DstRangeAddr"]);
		//ʹ�üĴ���ָ��Ŀ��ֵ
		StickChartConfig.UseDstRangeByAddr = false;
		//���������ɫ
		StickChartConfig.DstColor = 16711680;
		///��Χ����
		//���÷�Χ����
		StickChartConfig.UseRangeWarn = false;

		//�Ͼ���ֵ��������
		StickChartConfig.UpperWarnValue = 90;
		//�Ͼ���ֵ(��ַ)
		//m_StickChartConfig.UpperWarnAddr.Parse(json["UpperWarnAddr"]);
		//�Ĵ���ָ���Ͼ���ֵ
		StickChartConfig.UseUpperWarnAddr = false;
		StickChartConfig.UpperWarnFillinColor = 255;
		//�¾���ֵ��������
		StickChartConfig.LowerWarnValue = 10;
		//�¾���ֵ(��ַ)
		//m_StickChartConfig.LowerWarnAddr.Parse(json["LowerWarnAddr"]);
		//�Ĵ���ָ���¾���ֵ
		StickChartConfig.UseLowerWarnAddr = false;

		StickChartConfig.LowerWarnFillinColor = 65535;
		////���
		//��ǰѡ�����״��0ֱ����1����
		//m_StickChartConfig.Shape = 0;

		///ֱ��
		//����0�ϣ�1�£�2��3��
		//m_StickChartConfig.BarDirection = 0;

		///����
		//���⻷����
		StickChartConfig.ArcInnerCirclePercent = 15;
		//��ʼ�Ƕ�
		StickChartConfig.ArcStartAngle = 0;
		//�����Ƕ�
		StickChartConfig.ArcSweepAngle = 360;
		//����0˳ʱ�룻1��ʱ��
		StickChartConfig.ArcDirection = 0;

		///�߿�
		//��ʾ�߿�
		StickChartConfig.ShowFrame = true;
		//ǰ��ɫ
		StickChartConfig.ColorShowFrame = 0;

		///����
		//��ʾ����ɫ
		StickChartConfig.ShowBackground = true;
		//����ɫ
		StickChartConfig.ColorBack = 16777215;

		///���
		//���ɫ
		StickChartConfig.FIllinColor = 15128749;

		///ͼ�����
		////ͼ�����
		StickChartConfig.UseFillinStyle = false;
		//��ʽ
		StickChartConfig.FillinStyle = 1;
		//ǰ��ɫ
		StickChartConfig.ColorFillinStyle = 0;

		///����
		//����
		StickChartConfig.UseFillinShade = false;
		//��ʽ
		StickChartConfig.ShadeStyle = 0;
		//ǰ��ɫ
		StickChartConfig.ColorShadeStyle = 16775408;
		//͸����
		StickChartConfig.FadeoutPercent = 50;


		////�̶�����
		///�̶�
		//��ʾ�̶�
		StickChartConfig.ShowGraduate = true;
		//�̶����ͼ�����λ��
		StickChartConfig.RelatedGraduateSeat = 0;////
		//���̶���
		StickChartConfig.MainGraduateCount = 5;
		//���̶ȳ���
		StickChartConfig.MainGraduateLen = 6;
		//���̶���
		StickChartConfig.SecondaryGraduateCount = 3;
		//���̶ȳ���
		StickChartConfig.SecondaryGraduateLen = 3;

		///�̶���
		//�̶�����ɫ
		StickChartConfig.LineColor = 0;
		//�̶�����ʽ
		StickChartConfig.LineStyle = 0;
		//�̶��߿��
		StickChartConfig.LineWidget = 1;


		///���ֱ��
		//��ʾ���ֱ��
		StickChartConfig.ShowGraduateSign = true;
		//����λ
		StickChartConfig.IntegralDigit = 3;
		//С��λ
		StickChartConfig.DecimalDigit = 2;
		//�̶�����
		StickChartConfig.TypeFaceGraduate.Colors = 0;
		StickChartConfig.TypeFaceGraduate.Font.Name = "Arial";
		StickChartConfig.TypeFaceGraduate.Font.Size = 12;
		StickChartConfig.TypeFaceGraduate.Alpha = 255;
		StickChartConfig.TypeFaceGraduate.Align = 0;

		///�ٷֱ�
		StickChartConfig.ShowPercent = true;
		//�̶�����
		StickChartConfig.TypeFaceGraduate.Colors = 0;
		StickChartConfig.TypeFaceGraduate.Font.Name = "Arial";
		StickChartConfig.TypeFaceGraduate.Font.Size = 12;
		StickChartConfig.TypeFaceGraduate.Alpha = 255;
		StickChartConfig.TypeFaceGraduate.Align = 0;
		//��ʾ����
		StickChartConfig.ShowAxis = true;

	}

}