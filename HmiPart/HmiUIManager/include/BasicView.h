#pragma once
//class BaseControl;
//class BaseModel;
#include "BaseControl.h"
#include "BaseModel.h"
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
namespace UI
{
	class BasicView
	{
	public:
		template <class T>
		std::shared_ptr<T> GetControl()
		{
			return std::static_pointer_cast<T>(ctrl_.lock());
		}

		template <class T>
		std::shared_ptr<T> GetModel()
		{
			return std::static_pointer_cast<T>(model_);
		}

		void SetCtrlModel(std::shared_ptr<BaseControl> ctrl, std::shared_ptr<BaseModel> model)
		{
			ctrl_ = ctrl;
			model_ = model;
		}

	private:
		std::weak_ptr<BaseControl> ctrl_;
		std::shared_ptr<BaseModel> model_;
	};
}

 
