#pragma once
namespace UI
{
	struct APIParam
	{
		int FuncId;
		int Param[8];//最多8个参数
		char* Ptr; //动态分配的数据存储内容
	};

	class UIApi
	{
	public:
		static void Draw(int id, APIParam* parma);
		static void DrawCircle(int x, int y,int r);
		static void DCMapClear(unsigned int DCMap);
		static void DCMapSetBackColor(unsigned int DCMap, unsigned int backColor);
		static void DCMapDrawLine(unsigned int DCMap, int x1, int y1, int x2,int y2,int linewidth,unsigned int color);
		static void DCMapDrawRect(unsigned int DCMap, int x, int y, int width, int height, int linewidth, unsigned int color, bool fillrect, unsigned int fillColor);
		static void DCMapDrawCircle(unsigned int DCMap,int x,int y,int radius,int linewidth,unsigned int color,bool fillrect,unsigned int fillColor);
		static void DCMapDrawEllipse(unsigned int DCMap, int x, int y, int xAxisLen, int yAxisLen, int linewidth, unsigned int color, bool fillrect, unsigned int fillColor);
		static void DCMapDrawCircleArc(unsigned int DCMap, int x, int y, int radius, int linewidth, unsigned int color,int startAngle,int endAngle);
		static void DCMapDrawEllipseArc(unsigned int DCMap, int x, int y, int xAxisLen, int yAxisLen, int linewidth, unsigned int color, int startAngle,int endAngle);
		static void OpenWindow(unsigned short winNo,unsigned short winX,unsigned short winY);
		static void CloseWindow(unsigned short winNo);
		static void ScreenJump(unsigned short screenNo);
	};
}


