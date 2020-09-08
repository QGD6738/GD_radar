#include "SI_EFM8BB2_DEFS.h"
#include "Uart.h"
#include "UartCfg.h"
#include "Command.h"
#include "Cpu.h"
#include "Timer.h"

BOOL Uart0TX_NOTOK_Flag = TRUE;
BOOL Uart0TX_ALL_Flag = FALSE;
E_UartReceiveFlag Uart0RX_OK_Flag = Rec_ERR;
Uart0_ContextType gslinContext;
Uart0_ReceiveInfoType 	gUart0ReceivePduInfo;

void   Uart0_Init(void)
{
	SFRPAGE_TEMP = SFRPAGE;
	SFRPAGE = 0x20;
	P0SKIP &= (~P0SKIP_B7__SKIPPED);
	P1SKIP &= (~P1SKIP_B0__SKIPPED);
	XBR0   |=  XBR0_URT0E__ENABLED;

	SCON0 = SCON0_SMODE__9_BIT | SCON0_REN__RECEIVE_ENABLED;

	SFRPAGE = 0x10;
	IE  |= IE_ES0__ENABLED;		//00; Set Interrupt Priority; 11 is highest; 00 is lowest;
	IP  &= (~0x10);
	IPH &= (~0x10);
	SFRPAGE = SFRPAGE_TEMP;

	StartTheUart0Timer();
}

void   UART0_INTRX_Dis(void)
{
	DECL_PAGE;
	SET_PAGE(UART0_SFR_PAGE);

	SCON0_REN = 0;
	SCON0_RI = 0;

	RESTORE_PAGE;

}

void   UART0_INTRX_En(void)
{
	DECL_PAGE;
	SET_PAGE(UART0_SFR_PAGE);

	SCON0_REN = 1;
	SCON0_RI = 0;

	RESTORE_PAGE;

}

void   UART0_Moudle_Dis(void)
{
	SFRPAGE_TEMP = SFRPAGE;
	SFRPAGE = 0x20;
	P0SKIP |= P0SKIP_B7__SKIPPED;
	P1SKIP |= P1SKIP_B0__SKIPPED;
	XBR0   &=  (~XBR0_URT0E__ENABLED);

	SCON0 = SCON0_SMODE__9_BIT | SCON0_REN__RECEIVE_ENABLED;

	SFRPAGE = 0x10;
	IE  &= (~IE_ES0__ENABLED);		//00; Set Interrupt Priority; 11 is highest; 00 is lowest;
	IP  &= (~0x10);
	IPH &= (~0x10);
	SFRPAGE = SFRPAGE_TEMP;
}

void UART0_ModeDataL()
{
	SFRPAGE_TEMP = SFRPAGE;
	SFRPAGE = 0x20;
	P0SKIP &= (~P0SKIP_B7__SKIPPED);
	SFRPAGE = SFRPAGE_TEMP;
}


void UART0_ModeDataH()
{
	SFRPAGE_TEMP = SFRPAGE;
	SFRPAGE = 0x20;
	P0SKIP |= P0SKIP_B7__SKIPPED;
	SFRPAGE = SFRPAGE_TEMP;
}



void   UART0_WriteByte(WORD dataTx)
{
	DECL_PAGE;
	SET_PAGE(UART0_SFR_PAGE);

//	while(Uart0TX_NOTOK_Flag)
//	{
//		CLR_WDT;
//	}
//	Uart0TX_NOTOK_Flag = TRUE;
	SCON0_TB8 = (dataTx>>8);
	SBUF0 = (BYTE)(dataTx & 0x00FF);
	while(Uart0TX_NOTOK_Flag)
	{
		CLR_WDT;
	}
	Uart0TX_NOTOK_Flag = TRUE;
	RESTORE_PAGE;
}

void   UART0_RXData(WORD rxdata)
{
	BYTE i;
	Uart0_ReceiveInfoType* pCtt;
	pCtt = &gUart0ReceivePduInfo;

//	if(rxdata == 0xAA ||
//		rxdata == 0xCC	)
//	{
//		pCtt->pReceiveData = (pCtt->ReceiveBuff);
//		pCtt->ReceiveCheckSum = 0;
//		pCtt->ReceiveDatalen = 4;
//		pCtt->ReceiveByteIdx = 0;
//		*(pCtt->pReceiveData) = rxdata;
//	}
//	if(pCtt->ReceiveBuff[0] == 0xAA ||
//	   pCtt->ReceiveBuff[0] == 0xCC	)
//	{
////		*(pCtt->pReceiveData) = rxdata;
////		(pCtt->pReceiveData)++;
//		pCtt->ReceiveBuff[pCtt->ReceiveByteIdx] = rxdata;
//		pCtt->ReceiveByteIdx++;
//
//		if(pCtt->ReceiveByteIdx == pCtt->ReceiveDatalen)
//		{
//			pCtt->pReceiveData = (pCtt->ReceiveBuff);
//			if(pCtt->ReceiveBuff[2] == FRAME_TAIL(pCtt->ReceiveBuff[0]))   //Frame Head: XX  Frame Tail: XF
//			{
//				for(i=0;i<(pCtt->ReceiveDatalen);i++)	//Byte even
//				{
//					if(((pCtt->ReceiveBuff[i]>>8)&0x01) == CalcByteEVEN(pCtt->ReceiveBuff[i]))
//					{
//						pCtt->ReceiveERRFlag = NO_ERR;
//					}
//					else
//					{
//						pCtt->ReceiveERRFlag = EVEN_ERR;
//						break;
//					}
//				}
//			}
//			else   //Frame tail is not match frame head
//			{
//				pCtt->ReceiveERRFlag = FRAME_ERR;
//			}
//
//			if(pCtt->ReceiveERRFlag == NO_ERR)		//Frame checksum
//			{
//				for(i=0;i<(pCtt->ReceiveDatalen-1);i++)
//				{
//					(pCtt->ReceiveCheckSum) += (*((pCtt->pReceiveData) + i) & 0x00FF);
//				}
//
//				(pCtt->ReceiveCheckSum) = (pCtt->ReceiveCheckSum)%256;
//
//				if((pCtt->ReceiveCheckSum) == pCtt->ReceiveBuff[3])
//				{
//					pCtt->ReceiveERRFlag = NO_ERR;
//					ReceiveFramereCorresponding(pCtt->ReceiveBuff[0]);
//					Uart0RX_OK_Flag = TRUE;
//				}
//				else
//				{
//					pCtt->ReceiveERRFlag = CHECKSUN_ERR;
//					Uart0RX_OK_Flag = FALSE;
//				}
//			}
//		}
//	}
//
//	if(pCtt->ReceiveERRFlag > NO_ERR)
//	{
//		InitUart0ReceiveVar();
//	}

	if(rxdata == 0xAA ||
		rxdata == 0xCC	)
	{
		pCtt->pReceiveData = (pCtt->ReceiveBuff);
		pCtt->ReceiveCheckSum = 0;
		pCtt->ReceiveDatalen = VechileDataLen;
		pCtt->ReceiveByteIdx = 0;
		*(pCtt->pReceiveData) = rxdata;
	}
	if(pCtt->ReceiveBuff[0] == 0xAA ||
	   pCtt->ReceiveBuff[0] == 0xCC	)
	{
		pCtt->ReceiveBuff[pCtt->ReceiveByteIdx] = rxdata;
		pCtt->ReceiveByteIdx++;

		if(pCtt->ReceiveByteIdx == pCtt->ReceiveDatalen)
		{
			pCtt->pReceiveData = (pCtt->ReceiveBuff);
			if(pCtt->ReceiveBuff[2] == FRAME_TAIL(pCtt->ReceiveBuff[0]))   //Frame Head: XX  Frame Tail: XF
			{
				for(i=0;i<(pCtt->ReceiveDatalen);i++)	//Byte even
				{
					if(((pCtt->ReceiveBuff[i]>>8)&0x01) == CalcByteEVEN(pCtt->ReceiveBuff[i]))
					{
						pCtt->ReceiveERRFlag = NO_ERR;
					}
					else
					{
						pCtt->ReceiveERRFlag = EVEN_ERR;
						break;
					}
				}
			}
			else   //Frame tail is not match frame head
			{
				pCtt->ReceiveERRFlag = FRAME_ERR;
			}

			if(pCtt->ReceiveERRFlag == NO_ERR)		//Frame checksum
			{
				pCtt->ReceiveERRFlag = NO_ERR;
				ReceiveFramereCorresponding(pCtt->ReceiveBuff[0]);
				if(pCtt->ReceiveBuff[0] == 0xAA)
				{
					Uart0RX_OK_Flag = Rec_VechiMode;
				}
				if(pCtt->ReceiveBuff[0] == 0xCC)
				{
					Uart0RX_OK_Flag = Rec_VechiInfo;
				}
				else
				{
					InitUart0ReceiveVar();
				}
			}
			else
			{
				Uart0RX_OK_Flag = FALSE;
				InitUart0ReceiveVar();
			}
		}

		if(pCtt->ReceiveERRFlag > NO_ERR)
		{
			InitUart0ReceiveVar();
		}
	}

	if(pCtt->ReceiveByteIdx > VechileDataLen ||
	   pCtt->ReceiveDatalen > VechileDataLen)
	{
		InitUart0ReceiveVar();
	}


}


SI_INTERRUPT(UART0_ISR, UART0_IRQn)
{
	BYTE scon0_buff,sbuf0_buff;
	WORD data_9bit;//rb8_buff;
	data_9bit = 0x0000;
	scon0_buff = SCON0;
//	if(SCON0_RI__SET == (scon0_buff & SCON0_RI__BMASK))
	if(0x01 == (scon0_buff & 0x01))
	{
		SCON0_RI = 0;
		sbuf0_buff = SBUF0;
		data_9bit = (((WORD)(scon0_buff&0x04))<<6) | sbuf0_buff;
		UART0_RXData(data_9bit);
	}

	if(SCON0_TI__SET == (scon0_buff & SCON0_TI__BMASK))
	{
		SCON0_TI = 0;
		Uart0TX_NOTOK_Flag = FALSE;
	}
}


