#include <string>
#include <iostream>
#include <rest_rpc.hpp>

#include "UIServer.h"
//#include "UIManager.h"
using namespace rest_rpc;
using namespace rpc_service;
namespace UI
{
	/*void DCMapClear(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, unsigned int backColor)
	{
		UIApi::DCMapClear(DCMap, backColor);
	}
	void DCMapSetBackColor(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, unsigned int backColor)
	{
		UIApi::DCMapSetBackColor(DCMap, backColor);
	}
	void DCMapDrawLine(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, int x1, int y1, int x2, int y2, int linewidth, unsigned int color)
	{
		UIApi::DCMapDrawLine(DCMap, x1, y1, x2, y2, linewidth, color);
	}
	void DCMapDrawRect(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, int x, int y, int width, int height, int linewidth, unsigned int color, bool fillrect, unsigned int fillColor)
	{
		UIApi::DCMapDrawRect(DCMap, x, y, width, height, linewidth, color, fillrect, fillColor);
	}
	void DCMapDrawCircle(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, int x, int y, int radius, int linewidth, unsigned int color, bool fillrect, unsigned int fillColor)
	{
		UIApi::DCMapDrawCircle(DCMap, x, y, radius, linewidth, color, fillrect, fillColor);
	}
	void DCMapDrawEllipse(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, int x, int y, int xAxisLen, int yAxisLen, int linewidth, unsigned int color, bool fillrect, unsigned int fillColor)
	{
		UIApi::DCMapDrawEllipse(DCMap, x, y, xAxisLen, yAxisLen, linewidth, color, fillrect, fillColor);
	}
	void DCMapDrawCircleArc(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, int x, int y, int radius, int linewidth, unsigned int color, int startAngle, int endAngle)
	{
		UIApi::DCMapDrawCircleArc(DCMap, x, y, radius, linewidth, color, startAngle, endAngle);
	}
	void DCMapDrawEllipseArc(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, int x, int y, int xAxisLen, int yAxisLen, int linewidth, unsigned int color, int startAngle, int endAngle)
	{
		UIApi::DCMapDrawEllipseArc(DCMap, x, y, xAxisLen, yAxisLen, linewidth, color, startAngle, endAngle);
	}
	void OpenWindow(rest_rpc::rpc_service::rpc_conn con, unsigned short winNo, unsigned short winX, unsigned short winY)
	{
		UIApi::OpenWindow(winNo, winX, winY);
	}
	void CloseWindow(rest_rpc::rpc_service::rpc_conn con, unsigned short winNo)
	{
		UIApi::CloseWindow(winNo);
	}
	void ScreenJump(rest_rpc::rpc_service::rpc_conn con, unsigned short screenNo)
	{
		UIApi::ScreenJump(screenNo);
	}
*/

	std::thread* UIServer::td_;
	rpc_server* UIServer::srv_; 
	bool UIServer::flag_;
	int UIServer::Init()
	{
		srv_ = new rpc_server(9000, std::thread::hardware_concurrency());
		/*srv_->register_handler("DCMapClear", &DCMapClear);
		srv_->register_handler("DCMapSetBackColor", &DCMapSetBackColor);
		srv_->register_handler("DCMapDrawLine", &DCMapDrawLine);
		srv_->register_handler("DCMapDrawRect", &DCMapDrawRect);
		srv_->register_handler("DCMapDrawCircle", &DCMapDrawCircle);
		srv_->register_handler("DCMapDrawEllipse", &DCMapDrawEllipse);
		srv_->register_handler("DCMapDrawCircleArc", &DCMapDrawCircleArc);
		srv_->register_handler("DCMapDrawEllipseArc", &DCMapDrawEllipseArc);
		srv_->register_handler("OpenWindow", &OpenWindow);
		srv_->register_handler("CloseWindow", &CloseWindow);
		srv_->register_handler("ScreenJump", &ScreenJump);*/
		srv_->register_handler("DCMapClear", &UIServer::DCMapClear);
		srv_->register_handler("DCMapSetBackColor", &UIServer::DCMapSetBackColor);
		srv_->register_handler("DCMapDrawLine", &UIServer::DCMapDrawLine);
		srv_->register_handler("DCMapDrawRect", &UIServer::DCMapDrawRect);
		srv_->register_handler("DCMapDrawCircle", &UIServer::DCMapDrawCircle);
		srv_->register_handler("DCMapDrawEllipse", &UIServer::DCMapDrawEllipse);
		srv_->register_handler("DCMapDrawCircleArc", &UIServer::DCMapDrawCircleArc);
		srv_->register_handler("DCMapDrawEllipseArc", &UIServer::DCMapDrawEllipseArc);
		srv_->register_handler("OpenWindow", &UIServer::OpenWindow);
		srv_->register_handler("CloseWindow", &UIServer::CloseWindow);
		srv_->register_handler("ScreenJump", &UIServer::ScreenJump);
		return 0;
	}
	void UIServer::Start()
	{
		td_ = new  std::thread([]()
		{
			if (srv_)
				srv_->run();
		});
		//UIManager::Ins().AddExit(UIServer::Stop);
	}
	void UIServer::Stop()
	{
	 
		if (td_&&td_->joinable())
		{
			if (srv_)
				srv_->stop();
			td_->join();
		}

		/*if (srv_)
			srv_->stop();*/
	}
	void UIServer::DCMapClear(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, unsigned int backColor)
	{
		UIApi::DCMapClear(DCMap, backColor);
	}
	void UIServer::DCMapSetBackColor(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, unsigned int backColor)
	{
		UIApi::DCMapSetBackColor(DCMap,backColor);
	}
	void UIServer::DCMapDrawLine(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, int x1, int y1, int x2, int y2, int linewidth, unsigned int color)
	{
		UIApi::DCMapDrawLine(DCMap,x1, y1, x2, y2, linewidth, color);
	}
	void UIServer::DCMapDrawRect(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, int x, int y, int width, int height, int linewidth, unsigned int color, bool fillrect, unsigned int fillColor)
	{
		UIApi::DCMapDrawRect(DCMap, x, y, width, height, linewidth, color, fillrect, fillColor);
	}
	void UIServer::DCMapDrawCircle(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, int x, int y, int radius, int linewidth, unsigned int color, bool fillrect, unsigned int fillColor)
	{
		UIApi::DCMapDrawCircle(DCMap, x, y, radius, linewidth, color, fillrect, fillColor);
	}
	void UIServer::DCMapDrawEllipse(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, int x, int y, int xAxisLen, int yAxisLen, int linewidth, unsigned int color, bool fillrect, unsigned int fillColor)
	{
		UIApi::DCMapDrawEllipse(DCMap, x, y, xAxisLen, yAxisLen, linewidth, color, fillrect, fillColor);
	}
	void UIServer::DCMapDrawCircleArc(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, int x, int y, int radius, int linewidth, unsigned int color, int startAngle, int endAngle)
	{
		UIApi::DCMapDrawCircleArc(DCMap, x, y, radius, linewidth, color, startAngle, endAngle);
	}
	void UIServer::DCMapDrawEllipseArc(rest_rpc::rpc_service::rpc_conn con, unsigned int DCMap, int x, int y, int xAxisLen, int yAxisLen, int linewidth, unsigned int color, int startAngle, int endAngle)
	{
		UIApi::DCMapDrawEllipseArc(DCMap, x, y, xAxisLen, yAxisLen, linewidth, color, startAngle, endAngle);
	}
	void UIServer::OpenWindow(rest_rpc::rpc_service::rpc_conn con, unsigned short winNo, unsigned short winX, unsigned short winY)
	{
		UIApi::OpenWindow(winNo, winX, winY);
	}
	void UIServer::CloseWindow(rest_rpc::rpc_service::rpc_conn con, unsigned short winNo)
	{
		UIApi::CloseWindow(winNo);
	}
	void UIServer::ScreenJump(rest_rpc::rpc_service::rpc_conn con, unsigned short screenNo)
	{
		UIApi::ScreenJump(screenNo);
	}
}
