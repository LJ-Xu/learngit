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

		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
			return;
		}

		StickChartConfig.InitBaseData(json);
		////对象（地址数据）

		//数据
		if (json.HasMember("BarVar") && json["BarVar"].IsObject())
			StickChartConfig.BarVar.Parse(json["BarVar"]);
		////范围（范围）
		///最大值
		//最大值(常数)
		if (json.HasMember("MaxValue"))
			StickChartConfig.MaxValue = json["MaxValue"].GetInt();
		//最大值(地址)
		if (json.HasMember("MaxData") && json["MaxData"].IsObject())
			StickChartConfig.MaxData.Parse(json["MaxData"]);
		//最大值是否由寄存器指定 
		if (json.HasMember("UseMaxAddr"))
			StickChartConfig.UseMaxAddr = json["UseMaxAddr"].GetBool();

		///最小值
		//最小值（常数）
		if (json.HasMember("MinValue"))
			StickChartConfig.MinValue = json["MinValue"].GetInt();
		//最小值(地址)
		if (json.HasMember("MinData") && json["MinData"].IsObject())
			StickChartConfig.MinData.Parse(json["MinData"]);
		//最小值是否由寄存器指定
		if (json.HasMember("UseMinAddr"))
			StickChartConfig.UseMinAddr = json["UseMinAddr"].GetBool();

		///目标区间
		//目标值（常数）
		if (json.HasMember("DstValue"))
			StickChartConfig.DstValue = json["DstValue"].GetInt();
		//目标值(地址)
		if (json.HasMember("DstValueAddr") && json["DstValueAddr"].IsObject())
			StickChartConfig.DstValueAddr.Parse(json["DstValueAddr"]);
		//使用寄存器指定目标值
		if (json.HasMember("UseDstField"))
			StickChartConfig.UseDstField = json["UseDstField"].GetBool();
		//误差范围（常数）
		if (json.HasMember("DstRange"))
			StickChartConfig.DstRange = json["DstRange"].GetInt();
		//误差范围(地址)
		if (json.HasMember("DstRangeAddr") && json["DstRangeAddr"].IsObject())
			StickChartConfig.DstRangeAddr.Parse(json["DstRangeAddr"]);
		//使用寄存器指定目标值
		if (json.HasMember("UseDstRangeByAddr"))
			StickChartConfig.UseDstRangeByAddr = json["UseDstRangeByAddr"].GetBool();
		//误差区间颜色
		if (json.HasMember("DstColor"))
			StickChartConfig.DstColor = json["DstColor"].GetInt();

		///范围报警
		//启用范围报警
		if (json.HasMember("UseRangeWarn"))
			StickChartConfig.UseRangeWarn = json["UseRangeWarn"].GetBool();

		//上警报值（常数）
		if (json.HasMember("UpperWarnValue"))
			StickChartConfig.UpperWarnValue = json["UpperWarnValue"].GetInt();
		//上警报值(地址)
		if (json.HasMember("UpperWarnAddr") && json["UpperWarnAddr"].IsObject())
			StickChartConfig.UpperWarnAddr.Parse(json["UpperWarnAddr"]);
		//寄存器指定上警报值
		if (json.HasMember("UseUpperWarnAddr"))
			StickChartConfig.UseUpperWarnAddr = json["UseUpperWarnAddr"].GetBool();
		//前景色
		if (json.HasMember("UpperWarnFillinColor"))
			StickChartConfig.UpperWarnFillinColor = json["UpperWarnFillinColor"].GetInt();

		
		//下警报值（常数）
		if (json.HasMember("LowerWarnValue"))
			StickChartConfig.LowerWarnValue = json["LowerWarnValue"].GetInt();
		//下警报值(地址)
		if (json.HasMember("LowerWarnAddr") && json["LowerWarnAddr"].IsObject())
			StickChartConfig.LowerWarnAddr.Parse(json["LowerWarnAddr"]);
		//寄存器指定下警报值
		if (json.HasMember("UseLowerWarnAddr"))
			StickChartConfig.UseLowerWarnAddr = json["UseLowerWarnAddr"].GetBool();
		//前景色
		if (json.HasMember("LowerWarnFillinColor"))
			StickChartConfig.LowerWarnFillinColor = json["LowerWarnFillinColor"].GetInt();

		////外观
		//当前选择的形状：0直条、1扇形
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


		///直条
		//方向：0上；1下；2左；3右
		if (json.HasMember("BarDirection"))
			StickChartConfig.BarDirection = json["BarDirection"].GetInt();

		///扇形
		//内外环比例
		if (json.HasMember("ArcInnerCirclePercent"))
			StickChartConfig.ArcInnerCirclePercent = json["ArcInnerCirclePercent"].GetInt();
		//起始角度
		if (json.HasMember("ArcStartAngle"))
			StickChartConfig.ArcStartAngle = json["ArcStartAngle"].GetInt();
		//终点角度
		if (json.HasMember("ArcSweepAngle"))
			StickChartConfig.ArcSweepAngle = json["ArcSweepAngle"].GetInt();
		//方向：0顺时针；1逆时针
		if (json.HasMember("ArcDirection"))
			StickChartConfig.ArcDirection = json["ArcDirection"].GetInt();

		///边框
		//显示边框
		if (json.HasMember("ShowFrame"))
			StickChartConfig.ShowFrame = json["ShowFrame"].GetBool();
		//前景色
		if (json.HasMember("ColorShowFrame"))
			StickChartConfig.ColorShowFrame = json["ColorShowFrame"].GetInt();

		///背景
		//显示背景色
		if (json.HasMember("ShowBackground"))
			StickChartConfig.ShowBackground = json["ShowBackground"].GetBool();
		//背景色
		if (json.HasMember("ColorBack"))
			StickChartConfig.ColorBack = json["ColorBack"].GetInt();

		///填充
		//填充色
		if (json.HasMember("FIllinColor"))
			StickChartConfig.FIllinColor = json["FIllinColor"].GetInt();

		///图案填充
		////图案填充
		if (json.HasMember("UseFillinStyle"))
			StickChartConfig.UseFillinStyle = json["UseFillinStyle"].GetBool();
		//样式
		if (json.HasMember("FillinStyle"))
			StickChartConfig.FillinStyle = json["FillinStyle"].GetInt();
		//前景色
		if (json.HasMember("ColorFillinStyle"))
			StickChartConfig.ColorFillinStyle = json["ColorFillinStyle"].GetInt();

		///渐变
		//渐变
		if (json.HasMember("UseFillinShade"))
			StickChartConfig.UseFillinShade = json["UseFillinShade"].GetBool();
		//样式
		if (json.HasMember("ShadeStyle"))
			StickChartConfig.ShadeStyle = json["ShadeStyle"].GetInt();
		//前景色
		if (json.HasMember("ColorShadeStyle"))
			StickChartConfig.ColorShadeStyle = json["ColorShadeStyle"].GetInt();
		//透明度
		if (json.HasMember("FadeoutPercent"))
			StickChartConfig.FadeoutPercent = json["FadeoutPercent"].GetInt();


		////刻度与标记
		///刻度
		//显示刻度
		if (json.HasMember("ShowGraduate"))
			StickChartConfig.ShowGraduate = json["ShowGraduate"].GetBool();
		//刻度与棒图的相对位置
		if (json.HasMember("RelatedGraduateSeat"))
			StickChartConfig.RelatedGraduateSeat = json["RelatedGraduateSeat"].GetInt();
		//主刻度数
		if (json.HasMember("MainGraduateCount"))
			StickChartConfig.MainGraduateCount = json["MainGraduateCount"].GetInt();
		//主刻度长度
		if (json.HasMember("MainGraduateLen"))
			StickChartConfig.MainGraduateLen = json["MainGraduateLen"].GetInt();
		//副刻度数
		if (json.HasMember("SecondaryGraduateCount"))
			StickChartConfig.SecondaryGraduateCount = json["SecondaryGraduateCount"].GetInt();
		//副刻度长度
		if (json.HasMember("SecondaryGraduateLen"))
			StickChartConfig.SecondaryGraduateLen = json["SecondaryGraduateLen"].GetInt();

		///刻度线
		//刻度线尺寸
		if (json.HasMember("LineColor"))
			StickChartConfig.LineColor = json["LineColor"].GetInt();
		//刻度线样式
		if (json.HasMember("LineStyle"))
			StickChartConfig.LineStyle = json["LineStyle"].GetInt();
		//刻度线宽度
		if (json.HasMember("LineWidget"))
			StickChartConfig.LineWidget = json["LineWidget"].GetInt();


		///数字标记
		//显示数字标记
		if (json.HasMember("ShowGraduateSign"))
			StickChartConfig.ShowGraduateSign = json["ShowGraduateSign"].GetBool();
		//整数位
		if (json.HasMember("IntegralDigit"))
			StickChartConfig.IntegralDigit = json["IntegralDigit"].GetInt();
		//小数位
		if (json.HasMember("DecimalDigit"))
			StickChartConfig.DecimalDigit = json["DecimalDigit"].GetInt();
		//刻度字体
		if (json.HasMember("TypeFaceGraduate") && json["TypeFaceGraduate"].IsObject())
			StickChartConfig.TypeFaceGraduate.Parse(json["TypeFaceGraduate"]);

		///百分比
		if (json.HasMember("ShowPercent"))
			StickChartConfig.ShowPercent = json["ShowPercent"].GetBool();
		//刻度字体
		if (json.HasMember("TypeFacePecent") && json["TypeFacePecent"].IsObject())
			StickChartConfig.TypeFacePecent.Parse(json["TypeFacePecent"]);
		//显示轴线
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


		////范围（范围）
		///最大值
		//最大值(常数)
		StickChartConfig.MaxValue = 100;
		//最大值(地址)
		//m_StickChartConfig.MaxData.Parse(json["MaxData"]);
		//最大值是否由寄存器指定 
		StickChartConfig.UseMaxAddr = false;

		///最小值
		//最小值（常数）
		StickChartConfig.MinValue = 0;
		//最小值(地址)
		//m_StickChartConfig.MinData.Parse(json["MinData"]);
		//最小值是否由寄存器指定
		StickChartConfig.UseMinAddr = false;


		//m_StickChartConfig.BarVar.Parse(json["BarVar"]);
		///目标区间
		//目标值（常数）
		StickChartConfig.DstValue = 50;
		//目标值(地址)
		//m_StickChartConfig.DstValueAddr.Parse(json["DstValueAddr"]);
		//使用寄存器指定目标值
		StickChartConfig.UseDstField = false;
		//误差范围（常数）
		StickChartConfig.DstRange = 0;
		//误差范围(地址)
		//m_StickChartConfig.DstRangeAddr.Parse(json["DstRangeAddr"]);
		//使用寄存器指定目标值
		StickChartConfig.UseDstRangeByAddr = false;
		//误差区间颜色
		StickChartConfig.DstColor = 16711680;
		///范围报警
		//启用范围报警
		StickChartConfig.UseRangeWarn = false;

		//上警报值（常数）
		StickChartConfig.UpperWarnValue = 90;
		//上警报值(地址)
		//m_StickChartConfig.UpperWarnAddr.Parse(json["UpperWarnAddr"]);
		//寄存器指定上警报值
		StickChartConfig.UseUpperWarnAddr = false;
		StickChartConfig.UpperWarnFillinColor = 255;
		//下警报值（常数）
		StickChartConfig.LowerWarnValue = 10;
		//下警报值(地址)
		//m_StickChartConfig.LowerWarnAddr.Parse(json["LowerWarnAddr"]);
		//寄存器指定下警报值
		StickChartConfig.UseLowerWarnAddr = false;

		StickChartConfig.LowerWarnFillinColor = 65535;
		////外观
		//当前选择的形状：0直条、1扇形
		//m_StickChartConfig.Shape = 0;

		///直条
		//方向：0上；1下；2左；3右
		//m_StickChartConfig.BarDirection = 0;

		///扇形
		//内外环比例
		StickChartConfig.ArcInnerCirclePercent = 15;
		//起始角度
		StickChartConfig.ArcStartAngle = 0;
		//经过角度
		StickChartConfig.ArcSweepAngle = 360;
		//方向：0顺时针；1逆时针
		StickChartConfig.ArcDirection = 0;

		///边框
		//显示边框
		StickChartConfig.ShowFrame = true;
		//前景色
		StickChartConfig.ColorShowFrame = 0;

		///背景
		//显示背景色
		StickChartConfig.ShowBackground = true;
		//背景色
		StickChartConfig.ColorBack = 16777215;

		///填充
		//填充色
		StickChartConfig.FIllinColor = 15128749;

		///图案填充
		////图案填充
		StickChartConfig.UseFillinStyle = false;
		//样式
		StickChartConfig.FillinStyle = 1;
		//前景色
		StickChartConfig.ColorFillinStyle = 0;

		///渐变
		//渐变
		StickChartConfig.UseFillinShade = false;
		//样式
		StickChartConfig.ShadeStyle = 0;
		//前景色
		StickChartConfig.ColorShadeStyle = 16775408;
		//透明度
		StickChartConfig.FadeoutPercent = 50;


		////刻度与标记
		///刻度
		//显示刻度
		StickChartConfig.ShowGraduate = true;
		//刻度与棒图的相对位置
		StickChartConfig.RelatedGraduateSeat = 0;////
		//主刻度数
		StickChartConfig.MainGraduateCount = 5;
		//主刻度长度
		StickChartConfig.MainGraduateLen = 6;
		//副刻度数
		StickChartConfig.SecondaryGraduateCount = 3;
		//副刻度长度
		StickChartConfig.SecondaryGraduateLen = 3;

		///刻度线
		//刻度线颜色
		StickChartConfig.LineColor = 0;
		//刻度线样式
		StickChartConfig.LineStyle = 0;
		//刻度线宽度
		StickChartConfig.LineWidget = 1;


		///数字标记
		//显示数字标记
		StickChartConfig.ShowGraduateSign = true;
		//整数位
		StickChartConfig.IntegralDigit = 3;
		//小数位
		StickChartConfig.DecimalDigit = 2;
		//刻度字体
		StickChartConfig.TypeFaceGraduate.Colors = 0;
		StickChartConfig.TypeFaceGraduate.Font.Name = "Arial";
		StickChartConfig.TypeFaceGraduate.Font.Size = 12;
		StickChartConfig.TypeFaceGraduate.Alpha = 255;
		StickChartConfig.TypeFaceGraduate.Align = 0;

		///百分比
		StickChartConfig.ShowPercent = true;
		//刻度字体
		StickChartConfig.TypeFaceGraduate.Colors = 0;
		StickChartConfig.TypeFaceGraduate.Font.Name = "Arial";
		StickChartConfig.TypeFaceGraduate.Font.Size = 12;
		StickChartConfig.TypeFaceGraduate.Alpha = 255;
		StickChartConfig.TypeFaceGraduate.Align = 0;
		//显示轴线
		StickChartConfig.ShowAxis = true;

	}

}