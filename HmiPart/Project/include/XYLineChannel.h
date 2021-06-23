#pragma once
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "DataVarInfo.h"
#include "BaseLine.h"
#include <vector>
namespace Project
{
	enum XYLineMode :char { Line, Xshadow, Yshadow, CurveLine, Dot ,LineDot	};
	enum DotStyle :char { FilledRect, FilledTriangle, FilledCricle, Rect, Triangel, Cricle, Fork };
	class XYLineChannel 
	{
	public:
		int Index;
		bool IsXYDataFromSameSource;	//XY�Ƿ�ʹ��ͬ�Ĵ���
		DataVarId StartXAddr;			//X������ʼ�Ĵ���
		DataVarId StartYAddr;			//Y������ʼ�Ĵ���
		XYLineMode CurveStyle;			//������ʽ �ߡ����ߡ��㡢xͶӰ��yͶӰ
		int LineColor;					//����ɫ
		int LineSize;					//�߿�
		int LineStyle;					//����ʽ
		int DrawDotColor;				//����ɫ
		int DrawDotSize;				//���С
		DotStyle DrawDotStyle;			//����ʽ
		DataVarId SampleControlVarId;	//�ɼ�����
		int ControlCond;				//�������� 0:ON 1:OFF 
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Index, IsXYDataFromSameSource, StartXAddr, StartYAddr, CurveStyle, LineColor,
				LineSize, LineStyle, DrawDotColor, DrawDotSize, DrawDotStyle, SampleControlVarId, ControlCond);
		}

		void Parse(rapidjson::Value& jsonObj);
		static void Parse(std::vector<XYLineChannel>& vector, rapidjson::Value& jsonObj);
	};
}

