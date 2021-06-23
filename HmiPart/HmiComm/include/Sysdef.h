#pragma once

#ifdef WIN32
//typedef bool                   BOOL;
typedef unsigned char            BYTE;
typedef unsigned short           WORD;
typedef unsigned long            DWORD;
typedef unsigned long long       DDWORD;

//typedef char                   INT8;
typedef unsigned char            UINT8;
typedef short                    INT16;
typedef unsigned short           UINT16;
typedef int                      INT32;
typedef unsigned int             UINT32;
typedef long long                INT64;
typedef unsigned long long       UINT64;

typedef double                   DOUBLE;
typedef float                    FLOAT;
//typedef bool                   BOOL;
typedef void*                    INTPTR;


#else 

typedef bool                     BOOL;
typedef unsigned char            BYTE;
typedef unsigned short           WORD;
typedef unsigned long            DWORD;
typedef unsigned long long       DDWORD;

typedef char                     INT8;
typedef unsigned char            UINT8;
typedef short                    INT16;
typedef unsigned short           UINT16;
typedef int                      INT32;
typedef unsigned int             UINT32;
typedef long long                INT64;
typedef unsigned long long       UINT64;

typedef double                   DOUBLE;
typedef float                    FLOAT;
typedef bool                     BOOL;
typedef void*                    INTPTR;
typedef int LPARAM;
typedef int RPARAM;
typedef int WPARAM;
typedef int HWND;

#endif
#define INT32 int
#define BYTE  char
#define VOID  void
typedef struct 
{
	short Cls;//BIT BYTE WORD DWORD DDWORD STRING REGS
	short Type;//BCD UNSIGNED signed
} XJDataType;
typedef struct
{
	DDWORD Data;//数据内容
	XJDataType Type;//数据类型信息
}XJData;


