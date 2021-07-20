#pragma once
#include "version.h"
#include "__ExtInfo.h"
#ifndef EXE_GIT_AUTHOR 
#define EXE_GIT_AUTHOR "no"
#define EXE_GIT_BRANCH "no"
#define EXE_GIT_HASH   "no"
#define EXE_GIT_SUBBRANCH "no"
#define EXE_GIT_SUBHASH   "no"
#define EXE_VER_DATE "xxxxxx"
#define EXE_VER_TIME "xxxxxx"
#endif
#define TAG_VER_STR  "$TV:1$"  
#define EXE_NAME_STR "$EN:HmiMain$"  
#define EXE_VER_STR "$V:" VER_STR "$"
#define EXE_GIT_STR "$G:" EXE_GIT_AUTHOR "|" EXE_GIT_BRANCH "|" EXE_GIT_HASH "|" EXE_GIT_SUBBRANCH "|"  EXE_GIT_SUBHASH "$" 
#define EXE_TIME_STR "$T:" EXE_VER_DATE  EXE_VER_TIME "$"
#define EXE_STR "#X_I_N_J_E_H_M_I" TAG_VER_STR EXE_NAME_STR EXE_VER_STR EXE_GIT_STR EXE_TIME_STR "X_I_N_J_E_H_M_I#"
#ifdef _MSC_VER
//#pragma( user, "XINJE_EVENT_822342941" ) 
//#pragma comment(exestr,"XINJE_EVENT_822342941")
__inline  char *_ExeStr(const char *str) { return (char*)str; }
#pragma data_seg(".exestr") 
static char *__exestr = _ExeStr(EXE_STR);
#else
static __attribute__((used)) const  char* exestr = EXE_STR;
#endif