#ifndef __BSP_PRINTER_H_
#define __BSP_PRINTER_H_

#include <linux/limits.h>



enum Printer_Type{
    PType_UART,
    PType_USB,
    PType_NET,
    PType_REMOTE,
};
#define DEV_MAX 12
struct uart_param{
    char dev[DEV_MAX];
    int speed;
    int databits;
    int stopbits;
    int parity;
};

typedef	struct printer_info
{
	short	Width;                //图形宽度
	short	Height;               //图形高度 
	short   Graph_Num;            //图形个数(高度/Pixeles)
	char	Print_Dir;            //打印顺序  0：从左到右 1：从右到左
	char	IsCut;                //切刀功能 0：不切 1：切

	char	*PrinterData;	      //打印的数据
	struct	printer_info *Next;   //下一个打印图形
    struct uart_param uart;
}	PRINTER_INFO;


typedef struct Set_Para
{
    char Printer_Type;
    char bmp[PATH_MAX];
	char	Print_Dir;            //打印顺序  0：从左到右   1：从右到左
	char	IsCut;                //切刀功能 0：不切       1：切
    struct uart_param uart;
}Print_Para;


PRINTER_INFO *PreparePrintData(Print_Para Para_Set);
void PrinterProcess(int uart_fd, PRINTER_INFO *pPrinterCur);
#endif
