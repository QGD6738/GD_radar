/*********************************************************************************************************************
** FILE NAME: Timer.c
**
** PROJECT:  HCXY_X30_RADAR
** CPU:	     EFM8BB21F16G
** COMPILER: SILICON
** -------------------------------------------------------------------------------------------------------------------
** VERSION:  V0.1
** DATE:     2019.02.21
*********************************************************************************************************************/
#include "cpu.h"
#include "Timer.h"
#include "DetectObj.h"
#include "Serial.h"

BOOL xdata bTimeFlag;
BOOL xdata b50msFlag;
WORD xdata w50msCnt;
BOOL xdata bLevel;
WORD xdata wTime;
BYTE xdata bSensorCheckFlag;
WORD xdata wSensorCheckTime;


/*********************************************************************************************************************
** Init the timer variable
*********************************************************************************************************************/
void Time_InitVariable(void)
{
	bTimeFlag = 0;
	b50msFlag = 0;
	w50msCnt  = 0;
	bLevel = 0;
	wTime = 0;
	//bSensorCheckFlag = 0;
	//wSensorCheckTime = 0;
}


/*********************************************************************************************************************
** Init the PCA0 count
*********************************************************************************************************************/
void Time_InitPCA0(void)
{
    SFRPAGE_TEMP = SFRPAGE;
    SFRPAGE = 0x00;

	PCA0CN0_CF   = 0; //Clear interrupt flag;
	PCA0CN0_CCF0 = 0; //Clear interrupt flag;
	PCA0CN0_CCF1 = 0; //Clear interrupt flag;
	PCA0CN0_CCF2 = 0; //Clear interrupt flag;

	PCA0MD	 	 = 0x01;/*bit1/2/3 = 000; Set clock is system/12; cnt = 12/49 = 0.2448us;
						  Measurement max pulse is 16ms
						  bit0 = 1; Set PCA0 overflow interrupt; CF is enable
						*/
	PCA0PWM 	 = 0x00;//No use
	PCA0CLR		 = 0x00;//No use
	PCA0POL		 = 0x00;//No use
	PCA0CENT 	 = 0x00;//No use
	PCA0L		 = 0x00;//PCA0 count
	PCA0H		 = 0x00;//PCA0 count

	PCA0CPM0	 = 0xB0;/*bit7 = 1; Set 16 bit count mode
						  bit6 = 0; Close Comparator function
						  bit5 = 1; Capture Positive Function Enable
						  bit4 = 1; Capture Negative Function Enable
						  bit3 = 0; Match Function disable
						  bit2 = 0; Toggle Functiondisable
						  bit1 = 0; Pulse Width Modulation Mode disable
						  bit0 = 0; Capture/Compare Flag Interrupt Enable
						*/
	PCA0CPM1	 = 0xB0;

	PCA0CPM2	 = 0x00;//channel not used

	EIE1	 	 = 0x10;//Enable interruput
	EIP1		|= 0x10;//11; Set Interrupt Priority; 11 is highest; 00 is lowest;
	EIP1H		|= 0x10;
	PCA0CN0_CR   = 1;   //Start PCA0;

	SFRPAGE = SFRPAGE_TEMP;
}

//void Start_PCA0_Comparator(void)
//{
//	PCA0 	  = 0xFFF0;
//	Serial.bComparatorFlag = TRUE;
//}
//void Stop_PCA0_Comparator(void)
//{
//	DATA_HIGH;
//	Serial.bComparatorFlag = FALSE;
//}

void Start_PCA0_Capture(BYTE CH)
{
    SFRPAGE_TEMP = SFRPAGE;
    SFRPAGE = 0x00;
    PCA0CN0_CR   = 0;   //Stop PCA0;
	if(CH == 0)
	{
		PCA0CPM0 |= 0x31;
	}
	if(CH == 1)
	{
		PCA0CPM1 |= 0x31;
	}
	if(CH == 2)
	{
		PCA0CPM2 |= 0x31;
	}
	PCA0CN0_CR   = 1;   //Start PCA0;
    SFRPAGE = SFRPAGE_TEMP;
}

void Stop_PCA0_Capture(BYTE CH)
{
    SFRPAGE_TEMP = SFRPAGE;
    SFRPAGE = 0x00;
    PCA0CN0_CR   = 0;   //Stop PCA0;
	if(CH == 0)
	{
		PCA0CPM0 &= (~0x31);
	}
	if(CH == 1)
	{
		PCA0CPM1 &= (~0x31);
	}
	if(CH == 2)
	{
		PCA0CPM2 &= (~0x31);
	}
	PCA0CN0_CR   = 1;   //Start PCA0;
    SFRPAGE = SFRPAGE_TEMP;
}
/*********************************************************************************************************************
** Set the 1ms timer.
*********************************************************************************************************************/
void Time_Init1msTimer(void) //use time 2
{
    SFRPAGE_TEMP = SFRPAGE;
    SFRPAGE = 0x00;

	TMR2CN0_T2XCLK0 = 0;	//bit0/bit1 = 00; Set clock is system/12; cnt =12/49us=0.2449us;
	TMR2CN0_T2XCLK1 = 0;
	TMR2CN0_TF2CEN  = 0;	//Disable capture
	TMR2CN0_T2SPLIT = 0;	//Set 16 bit mode
	TMR2L			= 0xFF;
	TMR2H			= 0xFF;
	TMR2RLL			= (BYTE)((0xFFFF-4083)>>0);	//Set reload value(1000us £ºcnt = 1000/12*49 = 4083)
	TMR2RLH			= (BYTE)((0xFFFF-4083)>>8);
	IE_ET2  	   	= 1; 	//Enable interrupt;
	IP		   	   &= (~0x20); //10; Set Interrupt Priority; 11 is highest; 00 is lowest;
	IPH		   	   |= ( 0x20);
	TMR2CN0_TR2 	= 1;	//Start T2

    SFRPAGE = SFRPAGE_TEMP;
}


/*********************************************************************************************************************
** Initial the 32ms timer.
*********************************************************************************************************************/
void Time_Init32msTimer(void) //use time 0
{
	CKCON0  |= 0x04; 	//Timer 0 high byte uses the system clock
	CKCON0	|= 0x02;	//Set Timer0/1 clock is system/48; cnt = 0.98us
	TMOD	&= (~0x04);	//bit3; Set 0 is timer mode;set 1 is count mode
	TMOD 	|= 1;		//bit0/bit1; Set 0 is 13 bit;Set 1 is 16 bit; Set 2 is reload 8 bit; Set 3 is two 8 bit;
	TL0		 = 0x00;
	TH0		 = 0x00;
	IE_ET0   = 0; 		//Disable interrupt;
	IP		|= 0x02;    //11; Set Interrupt Priority; 11 is highest; 00 is lowest;
	IPH	    |= 0x02;

	TCON_TR0 = 0;   	//stop T0
}


/*********************************************************************************************************************
** OPEN the 32ms timer interruput.
*********************************************************************************************************************/
void EnableThe32msTimerInterrupt()
{
	TCON_TF0 = 0; 		//Clear interrupt flag;
	IE_ET0 = 1; 		//Enable interrupt;
}

/*********************************************************************************************************************
** CLOSE the 32ms timer interruput.
*********************************************************************************************************************/
void DisableThe32msTimerInterrupt()
{
	IE_ET0 = 0; 		//Disable interrupt;
}


/*********************************************************************************************************************
** Initial the Uart0 Baud Rate timer.
*********************************************************************************************************************/
void Time_InitUart0Timer(void)  //Timer 1
{
	SFRPAGE_TEMP = SFRPAGE;
//	CKCON0  |= 0x04; 	//Timer 0 high byte uses the system clock
//	CKCON0	|= 0x02;	//Set Timer0/1 clock is system/48; cnt = 0.98us

	CKCON0  &= (~0x08); 	//Timer 1 high byte uses the SCA clock
	TMOD	&= (~0x40);		//bit3; Set 0 is timer mode;set 1 is count mode
	TMOD 	|= 0x20;		//bit0/bit1; Set 0 is 13 bit;Set 1 is 16 bit; Set 2 is reload 8 bit; Set 3 is two 8 bit;
	TMOD	&= (~0x10);

	TL1		 = 0x96;
	TH1		 = 0x96;

	SFRPAGE = 0x10;
	IE_ET1   = 0; 		//Disable interrupt;
	IP		|= 0x08;    //11; Set Interrupt Priority; 11 is highest; 00 is lowest;
	IPH	    |= 0x08;
	SFRPAGE = SFRPAGE_TEMP;

	TCON_TR1 = 0;   	//stop T1
}

void StartTheUart0Timer(void)
{
	TCON_TF1 = 0;
	TCON_TR1 = 1;	//start T1
}

void StopTheUart0Timer(void)
{
	TCON_TF1 = 0;
	TCON_TR1 = 0;	//stop T1

}


void Time_InitTheDelayTimer(void) //time 4
{
    SFRPAGE_TEMP = SFRPAGE;
    SFRPAGE = 0x10;
	TMR4CN0_T4XCLK0 = 0;	//bit0/bit1 = 00; Set clock is system/12; cnt =12/49us=0.2449us;
	TMR4CN0_T4XCLK1 = 0;
	TMR4CN0_TF4CEN  = 0;	//Disable capture
	TMR4CN0_T4SPLIT = 0;	//Set 16 bit mode
	TMR4L			= 0xFF;
	TMR4H			= 0xFF;
	TMR4RLL			= 0x00;
	TMR4RLH			= 0x00;
	EIP2		   &= (~0x10);//00; Set Interrupt Priority; 11 is highest; 00 is lowest;
	EIP2H		   &= (~0x10);
	EIE2	  	   &= (~0x10); //Disable interrupt;

	TMR4CN0_TR4 	= 0;	//Stop T3

    SFRPAGE = SFRPAGE_TEMP;
}

void Time_InitAdcTriggerTimer(void) //time 3
{
    SFRPAGE_TEMP = SFRPAGE;
    SFRPAGE = 0x00;

	TMR3CN0 = 0x00; /* bit0/bit1 = 00; Set clock is system/12; cnt =12/49us=0.2449us;
					   bit2 = 0; T3 stop
					   bit3 = 0; Set 16 bit mode
	 	 	 	 	 */
	TMR3L	= 0xFF;
	TMR3H	= 0xFF;
	TMR3RLL	= (BYTE)((0xFFFF-((COUNT_TIME_UNIT*49)/12))>>0);	//Set reload value(35us £ºcnt = 35/12*49 = 142)
	TMR3RLH	= (BYTE)((0xFFFF-((COUNT_TIME_UNIT*49)/12))>>8);
	EIP1   |= 0x80;		//11; Set Interrupt Priority; 11 is highest; 00 is lowest;
	EIP1H  |= 0x80;
	EIE1   &= (~0x80);//bit8; Disable interrupt
	TMR3CN0&= (~0x04);//Stop T3

    SFRPAGE = SFRPAGE_TEMP;
}

void Time_StartAdcTrigger(void)
{
    SFRPAGE_TEMP = SFRPAGE;
    SFRPAGE = 0x00;

	TMR3CN0&= (~0x80);	//bit7; Clear interrupt flag
	TMR3L	= 0xFF;
	TMR3H	= 0xFF;
	TMR3CN0|= 0x04;
	EIE1   |= 0x80;		//bit7; Enable interrupt

    SFRPAGE = SFRPAGE_TEMP;
}

void Time_StopAdcTrigger(void)
{
    SFRPAGE_TEMP = SFRPAGE;
    SFRPAGE = 0x00;

	TMR3CN0 &= (~0x04);
	EIE1    &= (~0x80);//bit8; Disable interrupt

    SFRPAGE = SFRPAGE_TEMP;
}

void StartThe32msTimer (void)
{
    SFRPAGE_TEMP = SFRPAGE;
    SFRPAGE  = 0x00;

	TL0		 = 0x00;
	TH0		 = 0x00;
	TCON_TR0 = 1;   //start T0

    SFRPAGE  = SFRPAGE_TEMP;
}

void StopThe32msTimer (void)
{
    SFRPAGE_TEMP = SFRPAGE;
    SFRPAGE  = 0x00;

	TCON_TR0 = 0;   //stop T0

    SFRPAGE  = SFRPAGE_TEMP;
}

void Start1msTimer (void)
{
    SFRPAGE_TEMP = SFRPAGE;
    SFRPAGE = 0x00;
	TMR2L	= 0xFF;
	TMR2H	= 0xFF;
	IE_ET2 	= 1;		//Start
	TMR2CN0_TR2 = 1;	//Start T2

    SFRPAGE = SFRPAGE_TEMP;
}

void Stop1msTimer (void)
{
    SFRPAGE_TEMP = SFRPAGE;
    SFRPAGE = 0x00;

	IE_ET2  = 0;		//Stop
	TMR2CN0_TR2 = 0;	//Stop T2

    SFRPAGE = SFRPAGE_TEMP;
}

/*********************************************************************************************************************
** Delay unit:us
*********************************************************************************************************************/
void Delay1us(WORD byTime) //use time 4
{
    WORD Temp;

    SFRPAGE_TEMP = SFRPAGE;
    SFRPAGE = 0x10;

    byTime<<= 2;
    Temp 	= 0xFFFF-byTime;
    TMR4L  	= (BYTE)(Temp&0x00FF);
    TMR4H 	= (BYTE)((Temp&0xFF00)>>8);
	TMR4CN0_TR4 = 1;	//Start T3
    while(TMR4CN0_TF4H == 0)
    {
        CLR_WDT;
    }
    TMR4CN0_TR4 = 0;	//Stop T3
    TMR4CN0_TF4H = 0;

    SFRPAGE = SFRPAGE_TEMP;
}

/*********************************************************************************************************************
** Delay unit:ms
*********************************************************************************************************************/
void Delay1ms(WORD byTime)
{
	WORD i;
	for(i=0;i<byTime;i++)
	{
		Delay1us(1000);
	}
}


/***********************************************************************
** 1ms interrupt serve function.
************************************************************************/
SI_INTERRUPT (TIMER2_ISR, TIMER2_IRQn)//Timer2 use 1ms
{
    bTimeFlag = TRUE; //1024us
    if(++w50msCnt >= 50)
	{
    	w50msCnt = 0;
    	b50msFlag = TRUE;
	}
    DetObj_RealTimeClock();
    TMR2CN0_TF2H = 0; //Clear interrupt flag
}

/***********************************************************************
** 32ms interrupt serve function.
************************************************************************/
SI_INTERRUPT (TIMER0_ISR, TIMER0_IRQn)//Timer0 use 64ms
{

}


/***********************************************************************
** Timer4 Delay interrupt serve function.
************************************************************************/
SI_INTERRUPT (TIMER4_ISR, TIMER4_IRQn)//Timer4 use delay
{
    SFRPAGE_TEMP = SFRPAGE;
    SFRPAGE = 0x10;
	TMR4CN0_TF4H = 0;
    SFRPAGE = SFRPAGE_TEMP;
}


/***********************************************************************
** Timer3 Adc triger sampling interrupt serve function.
************************************************************************/
SI_INTERRUPT (TIMER3_ISR, TIMER3_IRQn)//Timer3 use adc sampling
{
    if(RawData.wRawDataIndex < (DETECT_BUFFER_LENGTH-1))
        RawData.wRawDataIndex++;
    TMR3CN0 &= (~0x80);	//bit7; Clear interrupt flag
}
