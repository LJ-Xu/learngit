#pragma once
namespace Project
{
	class ImageFormat
	{
	public:
		string	KeyVal;					//图片的哈希值
		bool	UseRotate = false;		//使用旋转
		int		RotateAngle = 0;		//旋转角度
		bool	UseFaheout = false;		//使用透明度
		int		FadeoutPecent;			//透明比例

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(KeyVal, UseRotate, RotateAngle, UseFaheout, FadeoutPecent);
		}
	};
}
