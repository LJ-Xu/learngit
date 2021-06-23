#pragma once

typedef int(*_Sys_HMIMacroApi)(const char* apiid, void *param);

//#define  MacroMain()  _MacroMain(_Sys_HMIMacroApi _api)
typedef char bool;
typedef unsigned int DWORD;
typedef unsigned short WORD;
/*********************************************************************/
#define GetBit(varId)  _GetBit(varId,_g_api,&_g_erro)
extern bool _GetBit(int varId, _Sys_HMIMacroApi api, int *err);

#define DrawCircle(x,y,r)  _DrawCircle(x,y,r,_g_api,&_g_erro)
extern void _DrawCircle(int x,int y,int r, _Sys_HMIMacroApi api, int *err);

#define Beep()  _Beep(_g_api, &_g_erro)
extern void _Beep(_Sys_HMIMacroApi api, int *err);

#define Delay(ms)  _Delay(ms, _g_api, &_g_erro)
extern void _Delay(unsigned int ms, _Sys_HMIMacroApi api, int *err);

#define MakeDCID(winID, DCOrder)  _MakeDCID(winID, DCOrder, _g_api, &_g_erro)
extern unsigned int _MakeDCID(WORD winID, WORD DCOrder, _Sys_HMIMacroApi api, int *err);

#define DCMapClear(DCMapID, backColor)  _DCMapClear(DCMapID, backColor, _g_api, &_g_erro)
extern bool _DCMapClear(unsigned int DCMapID, unsigned int backColor, _Sys_HMIMacroApi api, int *err);

#define DCMapSetBackColor(DCMapID, backColor)  _DCMapSetBackColor(DCMapID, backColor, _g_api, &_g_erro)
extern bool _DCMapSetBackColor(unsigned int DCMapID, unsigned int backColor, _Sys_HMIMacroApi api, int *err);

#define DCMapDrawLine(DCMapID, x, y, width, height, linewidth, color)  _DCMapDrawLine(DCMapID, x, y, width, height, linewidth, color, _g_api, &_g_erro)
extern bool _DCMapDrawLine(unsigned int DCMapID, int x, int y, int width, int height, int linewidth, unsigned int color, _Sys_HMIMacroApi api, int *err);

#define DCMapDrawCircle(DCMapID, x, y, radius, linewidth, color, fillRect, fillColor)  _DCMapDrawCircle(DCMapID, x, y, radius, linewidth, color, fillRect, fillColor, _g_api, &_g_erro)
extern bool _DCMapDrawCircle(unsigned int DCMapID, int x, int y, int radius, int linewidth, unsigned int color, bool fillRect, unsigned int fillColor, _Sys_HMIMacroApi api, int *err);

#define DCMapDrawEllipse(DCMapID, x, y, xAxisLen, yAxisLen, linewidth, color, fillRect, fillColor) _DCMapDrawEllipse(DCMapID, x, y, xAxisLen, yAxisLen, linewidth, color, fillRect, fillColor, _g_api, &_g_erro)
extern bool _DCMapDrawEllipse(unsigned int DCMapID, int x, int y, int xAxisLen, int yAxisLen, int linewidth, unsigned int color, bool fillRect, unsigned int fillColor, _Sys_HMIMacroApi api, int *err);

#define DCMapDrawCircleArc(DCMapID, x, y, radius, linewidth, color, startAngle, endAngle)  _DCMapDrawCircleArc(DCMapID, x, y, radius, linewidth, color, startAngle, endAngle, _g_api, &_g_erro)
extern bool _DCMapDrawCircleArc(DWORD DCMapID, int x, int y, int radius, int linewidth, DWORD color, DWORD startAngle, DWORD endAngle, _Sys_HMIMacroApi api, int *err);

#define DCMapDrawEllipseArc(DCMapID, x, y, xAxisLen, yAxisLen, linewidth, color, startAngle, endAngle) _DCMapDrawEllipseArc(DCMapID, x, y, xAxisLen, yAxisLen, linewidth, color, startAngle, endAngle, _g_api, &_g_erro)
extern bool _DCMapDrawEllipseArc(DWORD DCMapID, int x, int y, int xAxisLen, int yAxisLen, int linewidth, DWORD color, DWORD startAngle, DWORD endAngle, _Sys_HMIMacroApi api, int *err);

#define OpenWindow(winNo, winX, winY)  _OpenWindow(winNo, winX, winY, _g_api, &_g_erro)
extern void _OpenWindow(WORD winNo, WORD winX, WORD winY, _Sys_HMIMacroApi api, int *err);

#define ScreenJump(screenNo)  _ScreenJump(screenNo, _g_api, &_g_erro)
extern WORD _ScreenJump(WORD screenNo, _Sys_HMIMacroApi api, int *err);

#define CloseWindow(winNo)  _CloseWindow(winNo, _g_api, &_g_erro)
extern void _CloseWindow(WORD winNo, _Sys_HMIMacroApi api, int *err);

#define Reads(devID, staNo, regType, dataType, addr, addr1, count, buf) _Reads(devID, staNo, regType, dataType, addr, addr1, count, buf, _g_api, &_g_erro)
extern bool  _Reads(int devID, int staNo, int regType, int dataType, int addr, int addr1, int count, void* buf, _Sys_HMIMacroApi api, int *err);

#define Writes(devID, staNo, regType, dataType, addr, addr1, count, buf) _Writes(devID, staNo, regType, dataType, addr, addr1, count, buf, _g_api, &_g_erro)
extern bool  _Writes(int devID, int staNo, int regType, int dataType, int addr, int addr1, int count, void* buf, _Sys_HMIMacroApi api, int *err);

#define GetPSB(addr)  _GetPSB(addr, _g_api, &_g_erro)
extern bool _GetPSB(DWORD addr, _Sys_HMIMacroApi api, int *err);

#define SetPSB(addr, val)  _SetPSB(addr, val, _g_api, &_g_erro)
extern void _SetPSB(DWORD addr, bool val, _Sys_HMIMacroApi api, int *err);

#define GetSPSB(addr)  _GetSPSB(addr, _g_api, &_g_erro)
extern bool _GetSPSB(DWORD addr, _Sys_HMIMacroApi api, int *err);

#define SetSPSB(addr, val)  _SetSPSB(addr, val, _g_api, &_g_erro)
extern void _SetSPSB(DWORD addr, bool val, _Sys_HMIMacroApi api, int *err);

#define PSW _PSW(_g_api, &_g_erro)
extern unsigned short* _PSW(_Sys_HMIMacroApi api, int *err);

#define SPSW _SPSW(_g_api, &_g_erro)
extern unsigned short* _SPSW(_Sys_HMIMacroApi api, int *err);

#define ReadPFW(addr, len, buf)  _ReadPFW(addr, len, buf, _g_api, &_g_erro)
extern void _ReadPFW(int addr, int len, void* buf, _Sys_HMIMacroApi api, int *err);

#define WritePFW(addr, len, buf)  _WritePFW(addr, len, buf, _g_api, &_g_erro)
extern void _WritePFW(int addr, int len, void* buf, _Sys_HMIMacroApi api, int *err);

#define ReadSPFW(addr, len, buf)  _ReadSPFW(addr, len, buf, _g_api, &_g_erro)
extern void _ReadSPFW(int addr, int len, void* buf, _Sys_HMIMacroApi api, int *err);

#define WriteSPFW(addr, len, buf)  _WriteSPFW(addr, len, buf, _g_api, &_g_erro)
extern void _WriteSPFW(int addr, int len, void* buf, _Sys_HMIMacroApi api, int *err);
