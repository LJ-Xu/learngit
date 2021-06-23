#pragma once
namespace Project
{
	class ImageFormat
	{
	public:
		string	KeyVal;					//ͼƬ�Ĺ�ϣֵ
		bool	UseRotate = false;		//ʹ����ת
		int		RotateAngle = 0;		//��ת�Ƕ�
		bool	UseFaheout = false;		//ʹ��͸����
		int		FadeoutPecent;			//͸������

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(KeyVal, UseRotate, RotateAngle, UseFaheout, FadeoutPecent);
		}
	};
}
