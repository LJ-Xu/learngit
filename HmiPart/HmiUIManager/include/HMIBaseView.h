#pragma once
#include <memory>
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
namespace UI
{
	class BaseModel;
	class BaseControl;
	class HMIBaseView:public Fl_Widget
	{
	public:
		HMIBaseView(int X, int Y, int W, int H, const char* l) :Fl_Widget(X,Y,W,H,l){}
		HMIBaseView(int X, int Y, int W, int H) :Fl_Widget(X, Y, W, H) {}
		~HMIBaseView() {}

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
