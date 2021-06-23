/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GFuncControl.h
 * Author   : TangYao
 * Date     : 2021/02/07
 * Descript : 函数画布控件控制类
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/

#include "stdafx.h"
#include "Message.h"
#include "HMIWindow.h"
#include "GFuncView.h"
#include "GFuncControl.h"
#include "PermUtility.h"
#include "UIData.h"
#include "Logger.h"

namespace UI
{
	GFuncControl::GFuncControl(HMIPage * pg) : BaseControl(pg) {
		// 创建Model对象
		model_ = shared_ptr<GFuncModel>(new GFuncModel());
		InitMVCModel(model_);
	}

	GFuncControl::~GFuncControl() {

	}

	void GFuncControl::CreateView() {
		// 初始化View
		GFuncView * view = new GFuncView(
			model_->GFuncUnit.X + model_->GFuncUnit.OffX,
			model_->GFuncUnit.Y + model_->GFuncUnit.OffY,
			model_->GFuncUnit.Width,
			model_->GFuncUnit.Height);
		InitMVCView(view);
	}

	int GFuncControl::PeekHMIMessage(Message::Msg * msg) {
		shared_ptr<GFuncModel> model = static_pointer_cast<GFuncModel>(model_);
		GFuncView * view = (GFuncView *)pView_;
		switch (msg->Code)
		{
		case WM_EVENT_DATEUPDATE:
		{
			DCMapDrawFunc * func = (DCMapDrawFunc *)(msg->LParam);
			model->GFuncUnit.FuncId = func->FuncId;
			// 初始化绘制参数
			model->GFuncUnit.Init();
			switch (model->GFuncUnit.FuncId)
			{
			case Project::DrawFunc::DCMapSetBackColor:
				if (func->Length == 8) {
					int Params[2] = { 0 };
					// 解析绘制参数
					for (int i = 0; i < func->Length / 4; ++i) {
						char tmp[4] = { 0 };
						stringstream ss;
						strncpy(tmp, func->Params + i * 4, sizeof(tmp));
						ss << tmp;
						ss >> Params[i];
						if ((ss.rdstate() & stringstream::failbit) != 0) {
							LOG_ERROR("Param[%d]: %s Error\n", i, tmp);
							continue;
						}
					}
					// 获取绘制参数
					model->GFuncUnit.MapId = Params[0];
					// 画布背景色
					model->GFuncUnit.BackColor.R = R(Params[1]);
					model->GFuncUnit.BackColor.G = G(Params[1]);
					model->GFuncUnit.BackColor.B = B(Params[1]);
				}
				break;
			case Project::DrawFunc::DCMapDrawLine:
				if (func->Length == 28) {
					int Params[7] = { 0 };
					// 解析绘制参数
					for (int i = 0; i < func->Length / 4; ++i) {
						char tmp[4] = { 0 };
						stringstream ss;
						strncpy(tmp, func->Params + i * 4, sizeof(tmp));
						ss << tmp;
						ss >> Params[i];
						if ((ss.rdstate() & stringstream::failbit) != 0) {
							LOG_ERROR("Param[%d]: %s Error\n", i, tmp);
							continue;
						}
					}
					// 获取绘制参数
					model->GFuncUnit.MapId = Params[0];
					// 线条坐标、长度
					model->GFuncUnit.GX = Params[1];
					model->GFuncUnit.GY = Params[2];
					model->GFuncUnit.GW = Params[3];
					model->GFuncUnit.GH = Params[4];
					// 线条粗细
					model->GFuncUnit.LineWidth = Params[5];
					// 线条颜色
					model->GFuncUnit.LineColor.R = R(Params[6]);
					model->GFuncUnit.LineColor.G = G(Params[6]);
					model->GFuncUnit.LineColor.B = B(Params[6]);
				}
				break;
			case Project::DrawFunc::DCMapDrawRect:
				if (func->Length >= 29) {
					int Params[7] = { 0 };
					// 解析绘制参数
					for (int i = 0; i < 7; ++i) {
						char tmp[4] = { 0 };
						stringstream ss;
						strncpy(tmp, func->Params + i * 4, sizeof(tmp));
						ss << tmp;
						ss >> Params[i];
						if ((ss.rdstate() & stringstream::failbit) != 0) {
							LOG_ERROR("Param[%d]: %s Error\n", i, tmp);
							continue;
						}
					}
					// 获取绘制参数
					model->GFuncUnit.MapId = Params[0];
					// 矩形坐标、尺寸
					model->GFuncUnit.GX = Params[1];
					model->GFuncUnit.GY = Params[2];
					model->GFuncUnit.GW = Params[3];
					model->GFuncUnit.GH = Params[4];
					// 矩形边框线粗
					model->GFuncUnit.LineWidth = Params[5];
					// 矩形边框颜色
					model->GFuncUnit.LineColor.R = R(Params[6]);
					model->GFuncUnit.LineColor.G = G(Params[6]);
					model->GFuncUnit.LineColor.B = B(Params[6]);
					// 矩形是否填充
					model->GFuncUnit.FillRect = static_cast<bool>(func->Params[28]);
					if (model->GFuncUnit.FillRect) {
						if (func->Length == 33) {
							char color[4] = { 0 };
							int fillColor = 0;
							stringstream ss;
							strncpy(color, func->Params + 29, sizeof(color));
							ss << color;
							ss >> fillColor;
							if ((ss.rdstate() & stringstream::failbit) != 0) {
								LOG_ERROR("Param[8]: %s Error\n", color);
								return -1;
							}
							// 获取填充颜色
							model->GFuncUnit.FillColor.R = R(fillColor);
							model->GFuncUnit.FillColor.G = G(fillColor);
							model->GFuncUnit.FillColor.B = B(fillColor);
						}
					}
				}
				break;
			case Project::DrawFunc::DCMapDrawCircle:
				if (func->Length >= 25) {
					int Params[6] = { 0 };
					// 解析绘制参数
					for (int i = 0; i < 6; ++i) {
						char tmp[4] = { 0 };
						stringstream ss;
						strncpy(tmp, func->Params + i * 4, sizeof(tmp));
						ss << tmp;
						ss >> Params[i];
						if ((ss.rdstate() & stringstream::failbit) != 0) {
							LOG_ERROR("Param[%d]: %s Error\n", i, tmp);
							continue;
						}
					}
					// 获取绘制参数
					model->GFuncUnit.MapId = Params[0];
					// 圆形坐标、尺寸
					model->GFuncUnit.GX = Params[1];
					model->GFuncUnit.GY = Params[2];
					model->GFuncUnit.GW = Params[3];
					model->GFuncUnit.GH = Params[3];
					// 圆形边框线粗
					model->GFuncUnit.LineWidth = Params[4];
					// 圆形边框颜色
					model->GFuncUnit.LineColor.R = R(Params[5]);
					model->GFuncUnit.LineColor.G = G(Params[5]);
					model->GFuncUnit.LineColor.B = B(Params[5]);
					// 圆形是否填充
					model->GFuncUnit.FillRect = static_cast<bool>(func->Params[24]);
					if (model->GFuncUnit.FillRect) {
						if (func->Length == 29) {
							char color[4] = { 0 };
							int fillColor = 0;
							stringstream ss;
							strncpy(color, func->Params + 25, sizeof(color));
							ss << color;
							ss >> fillColor;
							if ((ss.rdstate() & stringstream::failbit) != 0) {
								LOG_ERROR("Params[7]: %d Error\n", color);
								return -1;
							}
							// 获取填充颜色
							model->GFuncUnit.FillColor.R = R(fillColor);
							model->GFuncUnit.FillColor.G = G(fillColor);
							model->GFuncUnit.FillColor.B = B(fillColor);
						}
					}
				}
				break;
			case Project::DrawFunc::DCMapDrawEllipse:
				if (func->Length >= 29) {
					int Params[7] = { 0 };
					// 解析绘制参数
					for (int i = 0; i < 7; ++i) {
						char tmp[4] = { 0 };
						stringstream ss;
						strncpy(tmp, func->Params + i * 4, sizeof(tmp));
						ss << tmp;
						ss >> Params[i];
						if ((ss.rdstate() & stringstream::failbit) != 0) {
							LOG_ERROR("Param[%d]: %s Error\n", i, tmp);
							continue;
						}
					}
					// 获取绘制参数
					model->GFuncUnit.MapId = Params[0];
					// 椭圆坐标、尺寸
					model->GFuncUnit.GX = Params[1];
					model->GFuncUnit.GY = Params[2];
					model->GFuncUnit.GW = Params[3];
					model->GFuncUnit.GH = Params[4];
					// 椭圆边框线粗
					model->GFuncUnit.LineWidth = Params[5];
					// 椭圆边框颜色
					model->GFuncUnit.LineColor.R = R(Params[6]);
					model->GFuncUnit.LineColor.G = G(Params[6]);
					model->GFuncUnit.LineColor.B = B(Params[6]);
					// 椭圆是否填充
					model->GFuncUnit.FillRect = static_cast<bool>(func->Params[28]);
					if (model->GFuncUnit.FillRect) {
						if (func->Length == 33) {
							char color[4] = { 0 };
							int fillColor = 0;
							stringstream ss;
							strncpy(color, func->Params + 29, sizeof(color));
							ss << color;
							ss >> fillColor;
							if ((ss.rdstate() & stringstream::failbit) != 0) {
								LOG_ERROR("Params[8]: %s Error\n", color);
								return -1;
							}
							// 获取填充颜色
							model->GFuncUnit.FillColor.R = R(fillColor);
							model->GFuncUnit.FillColor.G = G(fillColor);
							model->GFuncUnit.FillColor.B = B(fillColor);
						}
					}
				}
				break;
			case Project::DrawFunc::DCMapDrawCircleArc:
				if (func->Length == 32) {
					int Params[8] = { 0 };
					// 解析绘制参数
					for (int i = 0; i < func->Length / 4; ++i) {
						char tmp[4] = { 0 };
						stringstream ss;
						strncpy(tmp, func->Params + i * 4, sizeof(tmp));
						ss << tmp;
						ss >> Params[i];
						if ((ss.rdstate() & stringstream::failbit) != 0) {
							LOG_ERROR("Params[%d]: %s Error\n", i, tmp);
							continue;
						}
					}
					// 获取绘制参数
					model->GFuncUnit.MapId = Params[0];
					// 圆弧坐标、尺寸
					model->GFuncUnit.GX = Params[1];
					model->GFuncUnit.GY = Params[2];
					model->GFuncUnit.GW = Params[3];
					model->GFuncUnit.GH = Params[3];
					// 圆弧线粗
					model->GFuncUnit.LineWidth = Params[4];
					// 圆弧颜色
					model->GFuncUnit.LineColor.R = R(Params[5]);
					model->GFuncUnit.LineColor.G = G(Params[5]);
					model->GFuncUnit.LineColor.B = B(Params[5]);
					// 圆弧起始角
					model->GFuncUnit.StartAngle = Params[6];
					// 圆弧终止角
					model->GFuncUnit.EndAngle = Params[7];
				}
				break;
			case Project::DrawFunc::DCMapDrawEllipseArc:
				if (func->Length == 36) {
					int Params[9] = { 0 };
					// 解析绘制参数
					for (int i = 0; i < func->Length / 4; ++i) {
						char tmp[4] = { 0 };
						stringstream ss;
						strncpy(tmp, func->Params + i * 4, sizeof(tmp));
						ss << tmp;
						ss >> Params[i];
						if ((ss.rdstate() & stringstream::failbit) != 0) {
							LOG_ERROR("Params[%d]: %s Error\n", i, tmp);
							continue;
						}
					}
					// 获取绘制参数
					model->GFuncUnit.MapId = Params[0];
					// 椭圆弧坐标、尺寸
					model->GFuncUnit.GX = Params[1];
					model->GFuncUnit.GY = Params[2];
					model->GFuncUnit.GW = Params[3];
					model->GFuncUnit.GH = Params[4];
					// 椭圆弧线粗
					model->GFuncUnit.LineWidth = Params[5];
					// 椭圆弧颜色
					model->GFuncUnit.LineColor.R = R(Params[6]);
					model->GFuncUnit.LineColor.G = G(Params[6]);
					model->GFuncUnit.LineColor.B = B(Params[6]);
					// 椭圆弧起始角
					model->GFuncUnit.StartAngle = Params[7];
					// 椭圆弧终止角
					model->GFuncUnit.EndAngle = Params[8];
				}
				break;
			default:
				break;
			}
			// 重绘画布
			view->redraw();
			// 控件显示
			if (msg->LParam == model->GFuncUnit.Perm.ShowVID.Vid) {
				PermUtility::HandleShowPerm(model->GFuncUnit.Perm, view);
			}
			// 控件使能
			if (msg->LParam == model->GFuncUnit.Perm.EnableVID.Vid) {
				PermUtility::HandleEnablePerm(model->GFuncUnit.Perm, view);
			}
			view->position(view->x(), view->y());
			view->redraw();
			break;
		}
		default:
			break;
		}
		return 1;
	}
}
