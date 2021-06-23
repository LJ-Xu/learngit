#pragma once
#include <string>
namespace Project
{
	class HFont {
	public:
		std::string Name;
		int Size;
		int Style;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Name, Size, Style);
		}
	};
}