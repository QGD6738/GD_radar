
#ifndef _UARTCFG_H_
#define _UARTCFG_H_

#define EFM8PDL_UART1_AUTO_PAGE             STD_ON
#define UART_SYS_CLOCK                      49000000 /* 49MHz */

#define MsgRadar_Distance_Frame_Len		8     // 1Header + 5Data + 1CheckSUM + 1Tail

extern BYTE  xdata  MsgRadar_Distance[MsgRadar_Distance_Frame_Len];
void InitUart1AllVar(void);
#endif
