#pragma once
#include "stdafx.h"
#include "BaseVar.h"
#include "ProjectPortVarsInfo.h"
#include "HMIProject.h"
namespace UI
{

#define BITS_PER_BYTE (8)
#define BYTES_PER_WORD (2)
#define BITS_PER_WORD ( BITS_PER_BYTE * BYTES_PER_WORD )
#define BYTES_PER_DWORD (4)
#define BITS_PER_DWORD ( BITS_PER_BYTE * BYTES_PER_DWORD )
#define BYTES_PER_DDWORD (8)
#define BITS_PER_DDWORD ( BITS_PER_BYTE * BYTES_PER_DDWORD )
	class DataSwap
	{
	public:
		static void Swap(Project::BaseVar *var, BYTE *data, int len);
	};
}
