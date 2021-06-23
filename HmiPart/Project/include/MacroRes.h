#pragma once
namespace Project
{
	class MacroRes
	{
	public:
		int Id;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Id);
		}
	};
}