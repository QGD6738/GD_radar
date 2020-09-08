/*********************************************************************************************************************
** FILE NAME: UartCfg.c
**
** PROJECT:  DFSK_F508_RADAR
** CPU:	     EFM8BB21F16G
** COMPILER: SILICON
** -------------------------------------------------------------------------------------------------------------------
** VERSION:  V0.1
** DATE:     2020.08.06
*********************************************************************************************************************/
#include "SI_EFM8BB2_DEFS.h"
#include "Uart.h"
#include "UartCfg.h"
#include "Command.h"
#include "Cpu.h"
#include "Timer.h"

BCM_Mode_buff       xdata  MsgBCM_Mode;
BCM_Info_buff       xdata  MsgBCM_Info;
RPAS_ReqMode_buff   xdata  MsgRadar_ReqMode;
RPAS_Self_buff      xdata  MsgRadar_Self;
RPAS_Distance_buff  xdata  MsgRadar_Distance;

//stUart0_FrameInfoType Uart0_FrameInfoType;

const WORD       xdata  MsgBCM_Mode_Init[4] =	\
		{0x0000,0x0000,0x0000,0x0000};
const WORD       xdata  MsgBCM_Info_Init[4] =	\
		{0x0000,0x0004,0x0000,0x0000};
const WORD   xdata  MsgRadar_ReqMode_Init[4] =	\
        {0x00AA,0x000A,0x00AF,0x0063};
const WORD      xdata  MsgRadar_Self_Init[4] =	\
        {0x00BB,0x00FF,0x00BF,0x0000};
const WORD  xdata  MsgRadar_Distance_Init[8] =	\
//		{0x00CC,0x0000,0x0000,0x0000,0x0000,0x0000,0x00CF,0x019B};
        {0x00CC,0x00FF,0x00FF,0x00FF,0x00FF,0x00FF,0x00CF,0x00FF};

void   UART0_DataInit(void)
{
	BYTE i;
	for(i=0;i<4;i++)
	{
		MsgBCM_Mode._Byte[i] = MsgBCM_Mode_Init[i];
		MsgBCM_Info._Byte[i] = MsgBCM_Info_Init[i];
		MsgRadar_ReqMode._Word[i] = MsgRadar_ReqMode_Init[i];
		MsgRadar_Self._Byte[i] = MsgRadar_Self_Init[i];
	}

	for(i=0;i<8;i++)
	{
		MsgRadar_Distance._Byte[i] = MsgRadar_Distance_Init[i];
	}
}


BYTE   CalcByteEVEN(BYTE value)
{
   BYTE i,evensum,ret;
   evensum = 0;
   ret = 0;
   for(i=0;i<8;i++)
   {
	   if(BIT_SHIFLEFT(value,i) == 0x01)
	   {
		   evensum++;
	   }
   }
   ret = evensum%2;
   return ret;
}

BYTE   CalcFrameChecksum(E_Uart0_FrameType mode)
{
	BYTE i,ret;
	WORD checksum;
	checksum = 0x0000;
	ret = 0;
	switch (mode)
	{
		case UART0_FRM_DISTANCE:
		{
			for(i=0;i<7;i++)
			{
				checksum += (BYTE)(MsgRadar_Distance._Byte[i] & (WORD)0x00FF);
			}
			checksum = checksum % 256;
			break;
		}
		case UART0_FRM_REQMODE:
		{
			for(i=0;i<3;i++)
			{
				checksum += (BYTE)(MsgRadar_ReqMode._Word[i] & (WORD)0x00FF);
			}
			checksum = checksum % 256;
			break;
		}
		case UART0_FRM_VECHMOD:
		{
			for(i=0;i<3;i++)
			{
				checksum += (BYTE)(MsgBCM_Mode._Byte[i] & (WORD)0x00FF);
			}
			checksum = checksum % 256;
			break;
		}

		case UART0_FRM_SENDSELF:
		{
			for(i=0;i<3;i++)
			{
				checksum += (BYTE)(MsgRadar_Self._Byte[i] & (WORD)0x00FF);
			}
			checksum = checksum % 256;
			break;
		}

		case UART0_FRM_VECHINFO:
		{
			for(i=0;i<3;i++)
			{
				checksum += (BYTE)(MsgBCM_Info._Byte[i] & (WORD)0x00FF);
			}
			checksum = checksum % 256;
			break;
		}
	}
	ret = checksum;
	return ret;
}

void   ReceiveFramereCorresponding(WORD start)
{
	Uart0_ReceiveInfoType* pCtt;
    pCtt = &gUart0ReceivePduInfo;

	if(start == 0xAA)
	{
		MsgBCM_Mode._bit.Header         = (BYTE)(pCtt->ReceiveBuff[0]);
		MsgBCM_Mode._bit.Header_EVEN    = ((pCtt->ReceiveBuff[0] >> 8)&0x01);
		MsgBCM_Mode._bit.VechiMode      = (BYTE)(pCtt->ReceiveBuff[1]);
		MsgBCM_Mode._bit.VechiMode_EVEN = ((pCtt->ReceiveBuff[1] >> 8)&0x01);
		MsgBCM_Mode._bit.Tail           = (BYTE)(pCtt->ReceiveBuff[2]);
		MsgBCM_Mode._bit.Tail_EVEN      = ((pCtt->ReceiveBuff[2] >> 8)&0x01);
		MsgBCM_Mode._bit.Checksum       = (BYTE)(pCtt->ReceiveBuff[3]);
		MsgBCM_Mode._bit.Checksum_EVEN  = ((pCtt->ReceiveBuff[3] >> 8)&0x01);
	}
	else if(start == 0xCC)
	{
		MsgBCM_Info._bit.Header           = (BYTE)(pCtt->ReceiveBuff[0]);
		MsgBCM_Info._bit.Header_EVEN      = ((pCtt->ReceiveBuff[0] >> 8)&0x01);
		MsgBCM_Info._bit.VehicleSpeed     = ((pCtt->ReceiveBuff[1])&0x01);
		MsgBCM_Info._bit.ReversGearSts    = ((pCtt->ReceiveBuff[1] >> 1)&0x01);
		MsgBCM_Info._bit.ParkingGearSts   = ((pCtt->ReceiveBuff[1] >> 2)&0x01);
		MsgBCM_Info._bit.FrontSwitchSts   = ((pCtt->ReceiveBuff[1] >> 3)&0x01);
		MsgBCM_Info._bit.VechileInfo_EVEN = ((pCtt->ReceiveBuff[1] >> 8)&0x01);
		MsgBCM_Info._bit.Tail             = (BYTE)(pCtt->ReceiveBuff[2]);
		MsgBCM_Info._bit.Tail_EVEN        = ((pCtt->ReceiveBuff[2] >> 8)&0x01);
		MsgBCM_Info._bit.Checksum         = (BYTE)(pCtt->ReceiveBuff[3]);
		MsgBCM_Info._bit.Checksum_EVEN    = ((pCtt->ReceiveBuff[3] >> 8)&0x01);
	}
	else
	{
		InitUart0ReceiveVar();
	}

}

void InitUart0ReceiveVar(void)
{
	BYTE i;

	for(i=0;i<4;i++)
	{
		gUart0ReceivePduInfo.ReceiveBuff[i] = 0x0000;
		gUart0ReceivePduInfo.ReceiveByteBuff[i] = 0x0000;
	}
	gUart0ReceivePduInfo.pReceiveData = gUart0ReceivePduInfo.ReceiveBuff;
	gUart0ReceivePduInfo.ReceiveByteIdx = 0;
	gUart0ReceivePduInfo.ReceiveCheckSum = 0;
	gUart0ReceivePduInfo.ReceiveDatalen = VechileDataLen;
	gUart0ReceivePduInfo.ReceiveERRFlag = NULL_ERR;


}

void   InitUart0AllVar(void)
{
	UART0_DataInit();
	gslinContext.CommState = UART0_FRM_NULL;
	gslinContext.pCurFrameInfo = Uart0_NULL_PTR;
	gslinContext.pReceiveFrameInfo = Uart0_NULL_PTR;
	gslinContext.pCurFrameInfo->CurrentDataPtr = MsgBCM_Mode_Init;
	InitUart0ReceiveVar();


}


