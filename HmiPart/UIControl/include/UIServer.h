#pragma once
#include <thread>
#include "UIApi.h"
//#include "rest_rpc/rpc_server.h"
namespace rest_rpc
{
	namespace rpc_service
	{
		class rpc_server;
		class connection;
		using rpc_conn = std::weak_ptr<connection>;
	}
}


namespace UI
{
	
	class UIServer
	{
	public :
		static int Init();
		static void Start();
		static void Stop();
	private:
		static void DCMapClear(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, unsigned int backColor);
		static void DCMapSetBackColor(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, unsigned int backColor);
		static void DCMapDrawLine(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, int x1, int y1, int x2, int y2, int linewidth, unsigned int color);
		static void DCMapDrawRect(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, int x, int y, int width, int height, int linewidth, unsigned int color, bool fillrect, unsigned int fillColor);
		static void DCMapDrawCircle(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, int x, int y, int radius, int linewidth, unsigned int color, bool fillrect, unsigned int fillColor);
		static void DCMapDrawEllipse(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, int x, int y, int xAxisLen, int yAxisLen, int linewidth, unsigned int color, bool fillrect, unsigned int fillColor);
		static void DCMapDrawCircleArc(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, int x, int y, int radius, int linewidth, unsigned int color, int startAngle, int endAngle);
		static void DCMapDrawEllipseArc(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, int x, int y, int xAxisLen, int yAxisLen, int linewidth, unsigned int color, int startAngle, int endAngle);
		static void OpenWindow(rest_rpc::rpc_service::rpc_conn con, unsigned short winNo, unsigned short winX, unsigned short winY);
		static void CloseWindow(rest_rpc::rpc_service::rpc_conn con, unsigned short winNo);
		static void ScreenJump(rest_rpc::rpc_service::rpc_conn con, unsigned short screenNo);
		
	private:
		static bool flag_;
		static std::thread* td_;
		//static std::unique_ptr<buttonrpc> srv_;
		static rest_rpc::rpc_service::rpc_server* srv_;
		//UIApi ptr;

	};

}