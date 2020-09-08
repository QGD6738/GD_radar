#include "SI_EFM8BB2_DEFS.h"
#include "Uart.h"
#include "UartCfg.h"
#include "Command.h"
#include "Cpu.h"

#define UART_INT_NEST_COUNT    253
static uint8 UartInterruptCount = 0;

/* Clock prescaler values for baud rate initialization */
#define NUM_PRESC              8

void UART1_Init(void)
{
	SFRPAGE_TEMP = SFRPAGE;
	P0SKIP &= (~P0SKIP_B4__SKIPPED);
	P0SKIP &= (~P0SKIP_B5__SKIPPED);
	XBR2   |=  XBR2_URT1E__ENABLED;

	SFRPAGE = 0x20;
	SBCON1 = SBCON1_BREN__ENABLED | SBCON1_BPS__DIV_BY_48;
	UART1FCN0 = UART1FCN0_RXTH__ZERO | UART1FCN0_TXTH__ZERO
			| UART1FCN0_RFRQE__ENABLED | UART1FCN0_TFRQE__DISABLED;
	SBRLH1 = (0xFE << SBRLH1_BRH__SHIFT);
	SBRLL1 = (0x56 << SBRLL1_BRL__SHIFT);
	UART1LIN |= UART1LIN_AUTOBDE__DISABLED | UART1LIN_LINMDE__DISABLED;
	SCON1 |= SCON1_REN__RECEIVE_ENABLED;
	UART1FCN1 &= ~(UART1FCN1_RIE__BMASK | UART1FCN1_TIE__BMASK);
	UART1FCN1 |= UART1FCN1_RXTO__TIMEOUT_16;
	SFRPAGE = 0x10;
    EIE2  |= EIE2_ES1__ENABLED;
    EIP2  &= (~0x04);  //00; Set Interrupt Priority; 11 is highest; 00 is lowest;
    EIP2H &= (~0x04);

    SFRPAGE = 0x20;
    SCON1 = UART_RX_EN;

	/*Set data bit and stop bit length */
	SMOD1 = UART_DL_8_BITS | UART_SBL_SHORT;

	/* Init TX FIFO */
	UART1FCN0 &= ~(UART_TFRQE | UART_TFLSH | UART_TXTH);
	UART1FCN0 |= (UART_TXTH_ZERO | UART_TFRQE_DISABLED);

	UART1FCN1 &= ~(UART_TFRQ | UART_TXHOLD | UART_TXNF | UART_TIE);
	UART1FCN1 |= (UART_TFRQ_SET | UART_TXHOLD_CONTINUE | UART_TIE_DISABLED);

	/* Init RX FIFO */
	UART1FCN0 &= ~(UART_RFRQE | UART_RFLSH | UART_RXTH | UART_RFRQE);
	UART1FCN0 |= (UART_RXTH_ZERO | UART_RFRQE_ENABLED);

	UART1FCN1 &= ~(UART_RFRQ_IF | UART_RXTO | UART_RIE);
	UART1FCN1 |= (UART_RFRQ_SET	| UART_RXTO_TIMEOUT_16 | UART_RIE_DISABLED);

	SFRPAGE = SFRPAGE_TEMP;

}


void UART1_Moudle_Dis(void)
{   
	DECL_PAGE;
	SET_PAGE(UART1_SFR_PAGE);
	SCON1 = (UART_OVR_NOT_SET
			| UART_PERR_NOT_SET
			| UART_REN_RECEIVE_DISABLED
			| UART_TBX_LOW
			| UART_RBX_LOW
			| UART_TI_NOT_SET
			| UART_RI_NOT_SET);
	SMOD1 = (UART_MCE_MULTI_DISABLED
			| UART_SPT_ODD_PARITY
			| UART_PE_PARIY_DISABLED
			| UART_DL_8_BITS
			| UART_XBE_DISABLED
			| UART_SBL_SHORT);
	SBCON1 = (SBCON1_BREN__DISABLED | SBCON1_BPS__DIV_BY_48);
	UART1LIN = (UART_AUTOBDE_DISABLED | UART_SYNCDIE_DISABLED);
	RESTORE_PAGE;
}

void UART_INTRX_Dis(void) 
{
	DECL_PAGE;

    if(0 == UartInterruptCount)
    {
		SET_PAGE(UART1_SFR_PAGE);
		UART1FCN1_RIE = 0;
		RESTORE_PAGE;
    }

    UartInterruptCount++;
    if(UartInterruptCount > UART_INT_NEST_COUNT)
    {
        UartInterruptCount = UART_INT_NEST_COUNT;
    }

}

void UART_INTRX_En(void) 
{
	DECL_PAGE;

    if(UartInterruptCount > 0)
    {
        UartInterruptCount--;
    }

    if(0 == UartInterruptCount)
    {
		SET_PAGE(UART1_SFR_PAGE);
		UART1FCN1_RIE = 1;
		RESTORE_PAGE;
    }
}

BOOL UART1_TXOK_Flag = FALSE;
void UART1_WriteByte(uint8 dataTx)
{ 
	DECL_PAGE;
	SET_PAGE(UART1_SFR_PAGE);
	SBUF1 = dataTx;

	UART1_TXOK_Flag = FALSE;
	while(!UART1_TXOK_Flag)
	{
		if(((SCON1&SCON1_TI__BMASK) >> SCON1_TI__SHIFT) > 0)
		{
			SCON1_TI = 0;
			UART1_TXOK_Flag = TRUE;
		}
		CLR_WDT;
	}
	RESTORE_PAGE;
}

void UART_BreakIntEn(void)
{
	DECL_PAGE;
	SET_PAGE(UART1_SFR_PAGE);
	UART1LIN |= UART_BREAKDNIE_ENABLED;
	RESTORE_PAGE;
}


void UART_BreakIntDis(void)                       
{      
	DECL_PAGE;
	SET_PAGE(UART1_SFR_PAGE);
	UART1LIN &= ~UART_BREAKDNIE_ENABLED;
	RESTORE_PAGE;
}

void UART_RxDataIntEn(void)
{

}


void UART_RxDataIntDis(void)                       
{      

}

void UART_BreakDetectEn(void)
{ 

}

void UART_BreakTrigger(void)
{ 

}

void UART_BreakDetectDis(void)
{
 
} 

void		UART1_RxErrorHook(void)
{


}

void		UART1_RxDataHook(uint8 databyte)
{
	if(databyte == 0xF0)
	{

	}

}



//static uint8 rxRemaining = 0;
//static uint8 *rxBuffer;

SI_INTERRUPT(UART1_ISR, UART1_IRQn)
{
#if 1
	uint8 discard;
	uint8 errors;

	/* If auto-baud sync word detected to set baudrate, clear flag and disable auto-baud detection */
	if(0 !=  (UART1LIN & UART_AUTOBAUD_IF))
	{
		UART1LIN &= ~(UART_AUTOBAUD_IF | UART_AUTOBDE_ENABLED /*| UART_SYNCDIE_ENABLED*/);
	}

		/* If parity or overrun error, clear flags, and call user */
		errors = SCON1 & (UART_RXOVR_EF | UART_PARITY_EF);
		if(errors)
		{
			SCON1 &= ~errors;
			UART1_RxErrorHook();
		}

		/* Store byte if there is no parity error */
		if (errors & UART_PARITY_EF)
		{
			discard = SBUF1;
		}
		else
		{
			discard = SBUF1;
			UART1_RxDataHook(discard);
		}
#endif
}

