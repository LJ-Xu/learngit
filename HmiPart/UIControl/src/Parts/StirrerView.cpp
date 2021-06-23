/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StirrerView.cpp
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : �������ؼ�������
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "StirrerView.h"
#include "StirrerModel.h"
#include "StirrerControl.h"
#include "ResourceService.h"

namespace UI
{
	StirrerView::StirrerView(int x, int y, int w, int h)
		: HMIBaseLabel(x, y, w, h) {

	}

	StirrerView::~StirrerView() {

	}
	// ���ƿؼ�
	void StirrerView::draw() {
		shared_ptr<StirrerModel> model = BaseView.GetModel<StirrerModel>();
		// ��ȡͼƬ��Ϣ
		//vector<Project::ImageResId> resIds = model->StirrerUnit.Keys;
		// ��ȡ��ǰ״̬��ԴͼƬ
		unique_ptr<Project::ImageResId> key;
		IResourceService::Ins()->SetRenderStatus(ResIndex, model->StirrerUnit.Keys.size());
		switch (ResIndex)
		{
		case 0:
			if (model->StirrerUnit.Keys.size() > 0) {
				key.reset(new Project::ImageResId(model->StirrerUnit.Keys[0]));
			}
			break;
		case 1:
			if (model->StirrerUnit.Keys.size() > 1) {
				key.reset(new Project::ImageResId(model->StirrerUnit.Keys[1]));
			}
			break;
		case 2:
			if (model->StirrerUnit.Keys.size() > 2) {
				key.reset(new Project::ImageResId(model->StirrerUnit.Keys[2]));
			}
			break;
		default:
			break;
		}
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
		Fl_Box::draw();
	}
}
