#include "SI_EFM8BB2_DEFS.h"
#include "Uart.h"
#include "UartCfg.h"
#include "Command.h"
#include "Cpu.h"
#include "Timer.h"

BYTE  xdata  MsgRadar_Distance[MsgRadar_Distance_Frame_Len];

const BYTE  xdata  MsgRadar_Distance_Init[MsgRadar_Distance_Frame_Len] = {0xF0,0x03,0x04,0x09,0x28,0x28,0xE0,0xFA};
//TEST
//const BYTE  xdata  MsgRadar_Distance_Init[MsgRadar_Distance_Frame_Len] =	\
//        {0xF0,0x28,0x28,0x28,0x28,0x28,0xE0,0xFA};

void InitUart1AllVar(void)
{
	BYTE i;
	for(i=0;i<MsgRadar_Distance_Frame_Len;i++)
	{
		MsgRadar_Distance[i] = MsgRadar_Distance_Init[i];
	}

}
