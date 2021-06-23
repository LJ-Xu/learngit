// stdafx.h: 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件
//

#pragma once

#include "targetver.h"

#ifdef WIN32

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <windowsx.h>
#include <Tchar.h>

#endif


// 在此处引用程序需要的其他标头
#include <memory>
#include <iostream>
#include <string>
#include <stdio.h>
#include <cstdio>
#include <vector>
#define NDEBUG
#include <assert.h>
//#ifndef WIN32
//#define WIN32
//#endif
using namespace std;
#include "Sysdef.h"
#include "Comm.h"
