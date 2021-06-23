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
		bool IsXYDataFromSameSource;	//XY是否使用同寄存器
		DataVarId StartXAddr;			//X数据起始寄存器
		DataVarId StartYAddr;			//Y数据起始寄存器
		XYLineMode CurveStyle;			//曲线样式 线、点线、点、x投影、y投影
		int LineColor;					//线颜色
		int LineSize;					//线宽
		int LineStyle;					//线样式
		int DrawDotColor;				//点颜色
		int DrawDotSize;				//点大小
		DotStyle DrawDotStyle;			//点样式
		DataVarId SampleControlVarId;	//采集控制
		int ControlCond;				//控制条件 0:ON 1:OFF 
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

