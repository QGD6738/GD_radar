/*********************************************************************************************************************
** FILE NAME: Serial.c
**
** PROJECT:  DFSK_F508_RADAR
** CPU:	     EFM8BB21F16G
** COMPILER: SILICON
** -------------------------------------------------------------------------------------------------------------------
** VERSION:  V0.1
** DATE:     2020.08.06
*********************************************************************************************************************/

#include "cpu.h"
#include "DetectObj.h"
#include "Timer.h"
#include "Serial.h"
#include "command.h"
#include "Uart.h"

SERIAL_ISP  xdata Serial_ISP = Srl_GetHeader;
StSerial    xdata Serial;


void CommunicationMode(ECOM_MODE commode)
{
	if(COM_Internal == commode)
	{
		UART0_Moudle_Dis();
		EnableInternalCommunicaiton();
	}
	else if (COM_Uart == commode)
	{
		Stop_PCA0_Capture(0);		//DATA
		Uart0_Init();
		Uart0TX_NOTOK_Flag = FALSE;
	}
	else
	{
		UART0_Moudle_Dis();
		EnableInternalCommunicaiton();
		DisableInternalCommunicaiton();
	}
}




void EnableInternalCommunicaiton (void)
{
	SFRPAGE_TEMP = SFRPAGE;
	SFRPAGE = 0x20;
	P0SKIP |= (P0SKIP_B4__SKIPPED);
	P0SKIP &= (~P0SKIP_B5__SKIPPED);
	XBR1   |=  XBR1_PCA0ME__CEX0;   //PCA Module I/O Enable.
								    //0x0 DISABLED All PCA I/O unavailable at Port pins.
									//0x1 CEX0 CEX0 routed to Port pin.
									//0x2 CEX0_CEX1 CEX0, CEX1 routed to Port pins.
								    //0x3 CEX0_CEX1_CEX2 CEX0, CEX1, CEX2 routed to Port pins.

	SFRPAGE = 0x10;
	IE  |= IE_ES0__ENABLED;		//00; Set Interrupt Priority; 11 is highest; 00 is lowest;
	IP  &= (~0x10);
	IPH &= (~0x10);
	SFRPAGE = SFRPAGE_TEMP;

	Start_PCA0_Capture(0);		//DATA
}
void DisableInternalCommunicaiton (void)
{
	Stop_PCA0_Capture(0);			//DATA
	Stop_PCA0_Capture(1);			//LIN
}

void Srl_InitVariable(void)
{
	//BYTE xdata i;
	Serial.byCnt = 0;
	Serial.byData = 0;
	Serial.bDataHeader = 0;

	Serial.bValid = 0;
	Serial.CmdHead_Rx = 0;
	//for(i=0;i<16;i++)
	//{
	//	Serial.Time[i] = 0;
	//}
	//Serial.Index = 0;
	Serial_ISP = &Srl_GetHeader;
}

/***********************************************************************
** Send data in word size,parity bit is auto added.
***********************************************************************/
void Srl_SendData(WORD Head, BYTE byId, BYTE byIdLen, WORD byData, BYTE byDataLen)
{
    BYTE i;
    WORD Data;
    BYTE CheckSum = 0;

    Data = ((byData<<byIdLen)|byId); //DATA + ID

    if(Head == HEAD_800)
    {
    	Serial.bIO_800usFlag = TRUE;
    }
    else
    {
    	Serial.bIO_800usFlag = FALSE;
    }
    Serial.wSendBuffer[0] = 0xFFFF - Head + 13;
    for(i=0; i<(byIdLen+byDataLen); i++)
    {
        if(Data & (0x01<<i))
        {
        	Serial.wSendBuffer[2*i+1] = 0xFFFF - DATA_WIDTH_2 - 13;
        	Serial.wSendBuffer[2*i+2] = 0xFFFF - DATA_WIDTH_1 + 13;
        	CheckSum++;
        }
        else
        {
        	Serial.wSendBuffer[2*i+1] = 0xFFFF - DATA_WIDTH_1 - 13;
        	Serial.wSendBuffer[2*i+2] = 0xFFFF - DATA_WIDTH_2 + 13;
        }
    }
    CheckSum = CheckSum%2;
    if(CheckSum)
    {
    	Serial.wSendBuffer[(byIdLen+byDataLen)*2+1] = 0xFFFF - DATA_WIDTH_2 - 13;
    	Serial.wSendBuffer[(byIdLen+byDataLen)*2+2] = 0xFFFF - DATA_WIDTH_1 + 13;
    }
    else
    {
    	Serial.wSendBuffer[(byIdLen+byDataLen)*2+1] = 0xFFFF - DATA_WIDTH_1 - 13;
    	Serial.wSendBuffer[(byIdLen+byDataLen)*2+2] = 0xFFFF - DATA_WIDTH_2 + 13;
    }
    Serial.wSendBuffer[(byIdLen+byDataLen)*2+3] = 0xFFFF - DATA_WIDTH_1 - 13;
    Serial.wSendBuffer[(byIdLen+byDataLen)*2+4] = 0xFFFF - DATA_WIDTH_1 + 13;

	Serial.bSendIndex = 0;
	Serial.bSendMaxIndex = (byIdLen+byDataLen)*2+6;
	Start_PCA0_Comparator();

}

/***********************************************************************
** PCA0 interrupt serve function.
************************************************************************/
static xdata WORD CapTime0,CapTime1;
SI_INTERRUPT (PCA0_ISR, PCA0_IRQn)//PCA0 interrupt
{
	if(PCA0CN0_CCF0)//CCF0 is capture  DATA RX
	{
		bLevel = (BOOL)DATA_RX;
		CapTime0 = CapTime1;
		CapTime1 = PCA0CP0;
		if(CapTime1 > CapTime0)
		{
			wTime = CapTime1 - CapTime0;
		}
		else
		{
			wTime = 0xFFFF - CapTime0 + CapTime1;
		}
		//Serial.Time[Serial.Index++] = wTime;
		//if(Serial.Index == 16)Serial.Index = 0;
		(*Serial_ISP)(bLevel,wTime);
		PCA0CN0_CCF0 = 0;
	}
	if(PCA0CN0_CCF1)//CCF1 is capture  LIN RX
	{
//		bLevel = (BOOL)LIN_RX;
		CapTime0 = CapTime1;
		CapTime1 = PCA0CP1;
		if(CapTime1 > CapTime0)
		{
			wTime = CapTime1 - CapTime0;
		}
		else
		{
			wTime = 0xFFFF - CapTime0 + CapTime1;
		}
		//Serial.Time[Serial.Index++] = wTime;
		//if(Serial.Index == 16)Serial.Index = 0;
		(*Serial_ISP)(bLevel,wTime);
		PCA0CN0_CCF1 = 0;
	}
	if(PCA0CN0_CCF2)//CCF2 is capture
	{

		PCA0CN0_CCF2 = 0;
	}
	if(PCA0CN0_CF == 1) //Dont is PCA0 is overflow
	{
		if(Serial.bComparatorFlag == TRUE)
		{
//			if(Serial.bIO_800usFlag == TRUE)
//			{
//				DATA_TX = ~DATA_TX;
//			}
//			else
//			{
//				DATA_TX = ~DATA_TX;
//			}
			DATA_TX = ~DATA_TX;
			PCA0 = Serial.wSendBuffer[Serial.bSendIndex];
			Serial.bSendIndex++;
			if(Serial.bSendIndex == Serial.bSendMaxIndex)
			{
				Stop_PCA0_Comparator();
			}
		}
		PCA0CN0_CF = 0;
	}
}

void Srl_GetHeader(BOOL bLevel,WORD wTime)
{
    if(bLevel)
    {
    	INIT_RX_DATA;
    	if(wTime>HEAD_800_MIN && wTime<HEAD_800_MAX)
    	{
    		Serial.CmdHead_Rx = CMD_HEAD_800;
    		Serial_ISP = &Srl_GetData;
    	}
    	if(wTime>HEAD_1000_MIN && wTime<HEAD_1000_MAX)
    	{
    		Serial.CmdHead_Rx = CMD_HEAD_1000;
    		Serial_ISP = &Srl_GetData;
    	}
    	if(wTime>HEAD_1500_MIN && wTime<HEAD_1500_MAX)
    	{
    		Serial.CmdHead_Rx = CMD_HEAD_1500;
    		Serial_ISP = &Srl_GetData;
    	}
    	if(wTime>HEAD_2000_MIN && wTime<HEAD_2000_MAX)
    	{
    		Serial.CmdHead_Rx = CMD_HEAD_2000;
    		Serial_ISP = &Srl_GetData;
    	}
    }
}


void Srl_GetData(BOOL bLevel,WORD wTime)
{
    if(wTime<=DATA_WIDTH_1_MIN || wTime>=DATA_WIDTH_2_MAX)Serial_ISP = &Srl_GetHeader;

    if(bLevel)
    {
        if(wTime > DATA_WIDTH_1_MIN && wTime < DATA_WIDTH_1_MAX) // 100us
        {
            if(Serial.bDataHeader)
            {
                Serial.byData = Serial.byData+(1<<Serial.byCnt);
                Serial.byCnt++;
                Serial.bDataHeader = FALSE;
            }
            else
            {
            	//  By Sean
            	if(Serial.byCnt>0)
            	{
					if(RxDataParity() == TRUE)
						Serial.bValid = TRUE;
					Serial_ISP = &Srl_GetHeader;
            	}

                return;
            }
        }
        if(wTime > DATA_WIDTH_2_MIN && wTime < DATA_WIDTH_2_MAX) // 200us
        {
            if(Serial.bDataHeader)//err
            {
                Serial_ISP = &Srl_GetHeader;
            }
            else
            {
                Serial.byCnt++;
                Serial.bDataHeader = FALSE;
            }
        }
    }
    else
    {
        if(wTime > DATA_WIDTH_1_MIN && wTime < DATA_WIDTH_1_MAX) // 100us
        {
              Serial.bDataHeader = FALSE;
        }
        if(wTime > DATA_WIDTH_2_MIN && wTime < DATA_WIDTH_2_MAX) // 200us
        {
              Serial.bDataHeader = TRUE;
        }
    }
}

BYTE RxDataParity(void)
{
	BYTE i,RET = FALSE;
	BYTE CheckSum = 0;

	for(i=0 ;i<(Serial.byCnt-1);i++)
	{
		if(Serial.byData & (0x01<<i))
		{
			CheckSum ++;
		}
	}
	CheckSum = CheckSum%2;
	if(CheckSum == (Serial.byData>>(Serial.byCnt-1)))
	{
		RET = TRUE;
	}
	return RET;
}
