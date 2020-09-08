/*********************************************************************************************************************
** FILE NAME: Timer.c
**
** PROJECT:  DetTemplate
** CPU:	     STM8S003F3
** COMPILER: ST
** -------------------------------------------------------------------------------------------------------------------
** VERSION:  V1.0
** DATE:     2018.09.10
*********************************************************************************************************************/
#include "cpu.h"
#include "Timer.h"
#include "DetectObj.h"


BOOL  bTimeFlag;
BOOL  bLevel; 
WORD  wTime;
BYTE  bSensorCheckFlag;
WORD  wSensorCheckTime;
WORD  wDelayCnt;

/*********************************************************************************************************************
** Init the timer variable
*********************************************************************************************************************/
void Time_InitVariable(void)
{
    bLevel       = 0;
    wTime        = 0;         	        /* Initial the power up flag. */
    bTimeFlag    = FALSE;		/* Clear the 1ms timer flag. */
}
/*********************************************************************************************************************
** Set the 1ms timer.
** Disable it when initial.
*********************************************************************************************************************/
void Time_Init1msTimer(void) //use time 0
{
    TIM4_PSCR = 0x07;     //125khz = 16M / 2^7分频  即8us/CNT
    TIM4_ARR = 0x7D;      //125 = 7D =1MS
    TIM4_IER = 0x01;      //使能TIM4溢出中断 
    TIM4_CR1 = 0x81;      //使能TIM4
}


/*********************************************************************************************************************
** Initial the 32ms timer.
*********************************************************************************************************************/
void Time_Init32msTimer(void) //use time 1
{
    TIM1_CNTRH = 0X00;          //计时器初始值
    TIM1_CNTRL = 0X00;
    TIM1_PSCRH = 0x00;          //预分频值16 即：16M/16 = 1us/cnt
    TIM1_PSCRL = 0x0F;
    TIM1_ARRH  = (uchar)0xFF;   //65535 = 65.535ms
    TIM1_ARRL  = (uchar)0xFF;  
    TIM1_CR1  &= (~0x10);       //向上计数    
    TIM1_CR1 |= 0x80;           //预装载使能
    TIM1_IER &= (~0x01) ;       //0X01:更新中断失能
    TIM1_CR1 |= 0x01;           //计数器使能
} 

/*********************************************************************************************************************
** OPEN the 32ms timer interruput.
*********************************************************************************************************************/
void EnableThe32msTimerInterrupt()
{
    TIM1_SR1 &= (~0X01); TIM1_IER |= 0x01 ; //0X01:更新中断使能
}

/*********************************************************************************************************************
** CLOSE the 32ms timer interruput.
*********************************************************************************************************************/
void DisableThe32msTimerInterrupt()
{
    TIM1_IER &= 0xFE ; //0X00:更新中断失能
}


void Time_InitTheDelayTimer(void) //time 2
{
    TIM2_CR1   = 0x0C ;//0X08更新中断后，计数器停止,0X04:允许中断
    TIM2_PSCR  = 0x04 ;//16M/16=1us/cnt
    TIM2_IER   = 0x00 ;//0X01:允许更新中断
    TIM2_CR1  |= 0x00 ;//0x01:使能计数器计数
}

void Time_InitAdcTriggerTimer(void) //time 3
{
    TIM1_CNTRH = 0x00;  //计时器初始值
    TIM1_CNTRL = 0x00;
    TIM1_PSCRH = 0x00; //预分频值1 即：0.0625us/cnt
    TIM1_PSCRL = 0x00;
    TIM1_ARRH  = (uchar)((COUNT_TIME_UNIT*16)>>8);
    TIM1_ARRL  = (uchar)((COUNT_TIME_UNIT*16));
    TIM1_CR1  |= (0x10);  //向下计数 
    //TIM1_CR1  &= (~0x10);  //向上计数 
    TIM1_CR1  |= 0x80;    //预装载使能
    TIM1_IER  &= (~0x01); //0X01:更新中断失能
    TIM1_CR1  &= (~0x01); //0x01:计数器失能
}

void Time_StartAdcTrigger(void)
{
    TIM1_SR1  &= (~0X01);
    TIM1_CNTRH = 0X00;
    TIM1_CNTRL = 0X00;
    TIM1_IER  |= 0x01; //0X01:更新中断使能
    TIM1_CR1  |= 0x01; //0x01:计数器使能
}

void Time_StopAdcTrigger(void)
{
    TIM1_SR1 &= (~0X01);
    TIM1_IER &= (~0x01) ; //0X01:更新中断失能
    TIM1_CR1 &= (~0x01) ; //0x01:计数器使能
}

void StartThe32msTimer (void)
{
    TIM1_CR1 |= 0x01;
}

void StopThe32msTimer (void)
{
    TIM1_CR1 &= (~0x01);
    TIM1_CNTRH = 0X00;
    TIM1_CNTRL = 0X00;
}

void Start1msTimer (void)
{ 
    TIM4_CR1 |= 0x01;
}

void Stop1msTimer (void)
{
    TIM4_CR1 &= (~0x01);
}
/*********************************************************************************************************************
** Delay unit:ms
*********************************************************************************************************************/
#pragma optimize = none
void Delay1us(WORD byTime) //use time 3
{
    WORD Temp;
    BYTE High,Low;
  
    Temp = 0XFFFF-byTime+10;
    High = (BYTE)((Temp&0xFF00)>>8);
    Low  = (BYTE)(Temp&0x00FF);
    
    TIM2_CNTRH = High;
    TIM2_CNTRL = Low;
    
    TIM2_CR1  |= 0x01 ;//使能计数器计数
    while((TIM2_SR1&0X01) == 0)
    {
        CLR_WDT;
    }
    TIM2_SR1 &= (~0x01);             
}


/***********************************************************************
** 1ms interrupt serve function.
************************************************************************/
#pragma vector = TIM4_OVR_UIF_vector
__interrupt void TIM4_1MS_ISP (void)
{	
    bTimeFlag = TRUE; //1024us
    DetObj_RealTimeClock();
    //TIM4_SR &= (~0X01);
    TIM4_SR = 0x00;
}

/***********************************************************************
** Speaker interrupt serve function.
************************************************************************/
//#pragma vector = TIM2_CAPCOM_CC1IF_vector
//__interrupt void TIM2_Speaker_ISP (void)
//{
//  TIM2_SR1 &= (~0X0E);
//}


/***********************************************************************
** TIM2 OVR TEST .
************************************************************************/
#pragma vector = TIM2_OVR_UIF_vector
__interrupt void TIM2_OVR_ISP (void)
{
    TIM2_SR1 &= (~0X01);
}

/***********************************************************************
** TIM1 CAP TEST .
************************************************************************/
#pragma vector = TIM1_CAPCOM_CC1IF_vector
__interrupt void TIM1_CAP_ISP (void)
{
    TIM1_SR1 &= (~0X1E);
}

/***********************************************************************
** TIM1 OUT TEST .
************************************************************************/
#pragma vector = TIM1_OVR_UIF_vector
__interrupt void TIM1_OVR_ISP (void)
{
    if(RawData.wRawDataIndex < (DETECT_BUFFER_LENGTH-1))
        RawData.wRawDataIndex++;
    //TIM1_SR1 &= (~0X01);
    TIM1_SR1 = 0x00;
}
