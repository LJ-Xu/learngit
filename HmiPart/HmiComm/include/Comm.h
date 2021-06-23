#pragma once
#define B(color) (((color)>>16)&0xFF) 
#define G(color) (((color)>>8)&0xFF) 
#define R(color) ((color)&0xFF) 
typedef enum
{
	HMIResult_SUCCESS,
	HMIResult_ERR,


} HMIResult;
#define MIN(a,b) (((a)>(b))?(b):(a))
#define MAX(a,b) (((a)>(b))?(a):(b))
