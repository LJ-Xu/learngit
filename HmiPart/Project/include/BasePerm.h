#pragma once
class BasePerm
{
public:
	int RequireRegion;//��Ҫ��Ȩ�޷�Χ
	int AfterOpMode;//������ɺ��ģʽ�����Զ�ѡ 1:������ɺ�ȡ��Ȩ������  2:��Ȩ��ʱ����  4:��Ȩ��ʱ��ʾ
	
	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(RequireRegion, AfterOpMode);
	}
};

