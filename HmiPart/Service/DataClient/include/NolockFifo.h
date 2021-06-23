#pragma once
namespace HMI
{
	class  NolockFifo
	{
	public:
		~NolockFifo();
		void Init(int size);
		unsigned int Put(char *buffer, unsigned int len, bool affect = true);
		unsigned int Get(char *buffer, unsigned int len, bool affect=true);
		unsigned int Count();
		void Clear(unsigned int len=-1);
	private:
		unsigned int	in_=0;
		unsigned int	out_=0;
		//unsigned int	mask_;
		unsigned int	size_;
		void		   *data_;
	};

}
