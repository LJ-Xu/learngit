/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : WaterPumpView.cpp
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : ˮ�ÿؼ�������
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "WaterPumpView.h"
#include "WaterPumpModel.h"
#include "WaterPumpControl.h"
#include "ResourceService.h"

namespace UI
{
	WaterPumpView::WaterPumpView(int x, int y, int w, int h)
		: HMIBaseView(x, y, w, h) {

	}

	WaterPumpView::~WaterPumpView() {

	}

	// ���ƿؼ�
	void WaterPumpView::draw() {
		shared_ptr<WaterPumpModel> model = BaseView.GetModel<WaterPumpModel>();
		shared_ptr<WaterPumpControl> ctl = BaseView.GetControl<WaterPumpControl>();
		FinX = model->WaterPumpUnit.X + model->WaterPumpUnit.OffX;
		FinY = model->WaterPumpUnit.Y + model->WaterPumpUnit.OffY;
		// ��ȡͼƬ��Ϣ
		//vector<Project::ImageResId> model->WaterPumpUnit.Keys = model->WaterPumpUnit.Keys;
		// ��ȡ��ǰ״̬��ԴͼƬ
		unique_ptr<Project::ImageResId> key;
		IResourceService::Ins()->SetRenderStatus(model->WaterPumpUnit.CurrentStatus, model->WaterPumpUnit.Keys.size());
		
		switch (model->WaterPumpUnit.CurrentStatus)
		{
		case 0:									// Close
			if (model->WaterPumpUnit.Keys.size() > 0) {
				key.reset(new Project::ImageResId(model->WaterPumpUnit.Keys[0]));
			}
			break;
		case 1:									// Open
			if (model->WaterPumpUnit.Keys.size() > 1) {
				key.reset(new Project::ImageResId(model->WaterPumpUnit.Keys[1]));
			}
			break;
		default:
			break;
		}
		
		//key.reset(new Project::ImageResId(model->WaterPumpUnit.Keys[model->WaterPumpUnit.CurrentStatus]));
		if (!key) {
			return;
		}
		// ��ȡ��ǰͼƬ
		Fl_Image * img = IResourceService::Ins()->GetImage(key->KeyVal);
		if (img) {
			// ���ÿؼ�ͼƬ
			image(img);
			// ����ͼƬΪ����
			align(FL_ALIGN_IMAGE_BACKDROP);
			// ���û�����ʽ
			box(FL_NO_BOX);
		}
		// ���Ʊ���ͼƬ
		draw_box();
		//����״̬
		if (model->WaterPumpUnit.CurrentStatus)
		{
			//��������
			//�������
			int curmk = model->WaterPumpUnit.Dir ? (MarkCount - CurrentSpeendMark) : CurrentSpeendMark;
			int mkx = FinX + model->WaterPumpUnit.InnerRectX + model->WaterPumpUnit.InnerRectW / MarkCount * curmk;
			int mky = FinY + model->WaterPumpUnit.InnerRectY;
			fl_color(FL_WHITE);
			fl_rectf(mkx,
				mky,
				ctl->FluidSpeedMarkWidth,
				model->WaterPumpUnit.InnerRectH);
		}
	}
}
