#pragma once
class BasePerm
{
public:
	int RequireRegion;//需要的权限范围
	int AfterOpMode;//操作完成后的模式，可以多选 1:操作完成后取消权限限制  2:无权限时隐藏  4:无权限时提示
	
	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(RequireRegion, AfterOpMode);
	}
};

