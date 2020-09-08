/*********************************************************************************************************************
** FILE NAME: cpu.c
**
** PROJECT:  DFSK_F516_RADAR
** CPU:	     EFM8BB21F16G
** COMPILER: SILICON
** -------------------------------------------------------------------------------------------------------------------
** VERSION:  V0.1
** DATE:     2019.01.28
*********************************************************************************************************************/
#include <SI_EFM8BB2_Register_Enums.h>                  // SFR declarations
#include "cpu.h"
#include "ADC.h"
#include "Timer.h"

BYTE SFRPAGE_TEMP = 0;

void Cpu_Clk_Init (void)
{
	SFRPAGE = 0x10;
	PFE0CN = PFE0CN_PFEN__ENABLED | PFE0CN_FLRT__SYSCLK_BELOW_50_MHZ;

	SFRPAGE = 0x00;
	CLKSEL = CLKSEL_CLKSL__HFOSC0 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
	CLKSEL = CLKSEL_CLKSL__HFOSC0 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
	while ((CLKSEL & CLKSEL_DIVRDY__BMASK) == CLKSEL_DIVRDY__NOT_READY);
	CLKSEL = CLKSEL_CLKSL__HFOSC1 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
	CLKSEL = CLKSEL_CLKSL__HFOSC1 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
	while ((CLKSEL & CLKSEL_DIVRDY__BMASK) == CLKSEL_DIVRDY__NOT_READY);

}


void Cpu_WDG_Init(BOOL Switch)//The total interrupt needs to be closed when invoked
{
	SFRPAGE = 0x00;
	if(Switch == FUN_ON)
	{
		WDTCN = 0x02;//T LFOSC × 4^(WDTCN[2:0]+3) //T LFOSC = 80khz = 12.5us  so: WDTCN[2:0]=4，over is 204.8ms
		WDTCN = 0xFF;//lock the wdt
	}
	if(Switch == FUN_OFF)
	{
		WDTCN = 0xDE;
		WDTCN = 0xAD;
	}
}

void Cpu_EnableInterrupt (void)
{
	IE_EA = 1;       //开启总中断
}

void Cpu_DisableInterrupt (void)
{
	IE_EA = 0;       //关闭总中断
}

void Cpu_InitHardwarePort(void)
{
	SFRPAGE = 0x00;

	P0	    = 0x6D; //P0.0 is adc VDD_REF; 		 P0.1 is GND_REF;           P0.2 is NULL;     P0.3 is POWER_AD;
					//P0.4 is ID_TX;             P0.5 is ID_IN;             P0.6 is ECHO;     P0.7 is UART1_TX;
	P0MDOUT	=  P0MDOUT_B0__OPEN_DRAIN | P0MDOUT_B1__OPEN_DRAIN | P0MDOUT_B2__OPEN_DRAIN | P0MDOUT_B3__OPEN_DRAIN
			 | P0MDOUT_B4__OPEN_DRAIN | P0MDOUT_B5__OPEN_DRAIN | P0MDOUT_B6__OPEN_DRAIN | P0MDOUT_B7__PUSH_PULL;
	P0MDIN 	=  P0MDIN_B0__ANALOG  | P0MDIN_B1__ANALOG  | P0MDIN_B2__ANALOG  | P0MDIN_B3__ANALOG
			 | P0MDIN_B4__DIGITAL | P0MDIN_B5__ANALOG  | P0MDIN_B6__ANALOG | P0MDIN_B7__DIGITAL;
	P0SKIP 	=  P0SKIP_B0__SKIPPED | P0SKIP_B1__SKIPPED | P0SKIP_B2__SKIPPED | P0SKIP_B3__SKIPPED
			 | P0SKIP_B4__SKIPPED | P0SKIP_B5__SKIPPED | P0SKIP_B6__SKIPPED | P0SKIP_B7__SKIPPED;


	P1	    = 0x41;//P1.0 is UART1_RX;     P1.1 is NULL;  P1.2 is NULL;      P1.3 is NULL;
				   //P1.4 is NULL;  P1.5 is DRIVE_L;  P1.6 is DRIVE_H;
	P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__OPEN_DRAIN | P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__PUSH_PULL
			| P1MDOUT_B4__OPEN_DRAIN | P1MDOUT_B5__PUSH_PULL  | P1MDOUT_B6__PUSH_PULL  | P1MDOUT_B7__PUSH_PULL;
	P1MDIN =  P1MDIN_B0__DIGITAL     | P1MDIN_B1__DIGITAL     | P1MDIN_B2__DIGITAL     | P1MDIN_B3__DIGITAL
			| P1MDIN_B4__DIGITAL     | P1MDIN_B5__DIGITAL     | P1MDIN_B6__DIGITAL     | P1MDIN_B7__DIGITAL;
	P1SKIP =  P1SKIP_B0__SKIPPED     | P1SKIP_B1__SKIPPED     | P1SKIP_B2__SKIPPED     | P1SKIP_B3__SKIPPED
			| P1SKIP_B4__SKIPPED     | P1SKIP_B5__SKIPPED 	  | P1SKIP_B6__SKIPPED;


	XBR0 = XBR0_URT0E__DISABLED| XBR0_SPI0E__DISABLED | XBR0_SMB0E__DISABLED
			| XBR0_CP0E__DISABLED | XBR0_CP0AE__DISABLED | XBR0_CP1E__DISABLED
			| XBR0_CP1AE__DISABLED | XBR0_SYSCKE__DISABLED;

	XBR1 = XBR1_PCA0ME__DISABLED | XBR1_ECIE__DISABLED | XBR1_T0E__DISABLED
			| XBR1_T1E__DISABLED | XBR1_T2E__DISABLED;

	XBR2 = XBR2_WEAKPUD__PULL_UPS_DISABLED | XBR2_XBARE__ENABLED
			| XBR2_URT1E__DISABLED | XBR2_URT1RTSE__DISABLED
			| XBR2_URT1CTSE__DISABLED;
//	DATA_LOW;
	}


void Cpu_Init_Vect(void)
{

}

