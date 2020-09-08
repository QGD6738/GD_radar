/*********************************************************************************************************************
** FILE NAME: main.c
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
#include "ADC.h"
#include "ReadID.h"
#include "Serial.h"
#include "Command.h"
#include "Speaker.h"

void main(void)
{
    Cpu_Clk_Init(); 	        /* Initial the system CLK. */
    Cpu_Init_Vect();            /* Initial the interruput priority. */
    //Cpu_WDG_Init();             /* Initial the WDG. */
    Cpu_InitHardwarePort();
    Time_InitTheDelayTimer();
    Spk_InitTheVariable();
    Spk_InitThePWMModule();

    Delay1us(50000);
    Delay1us(50000);
    Delay1us(50000);
    Delay1us(50000);
    Delay1us(50000);
    ADC_HardwareInit(); 
    ADC_PowerSample();
#if(TEST_CODE_FIXED_ID == FALSE)
    {
        ID_PowerSample();
        ID_GetSensorId();
        DetInfo.bID = ID;
    }
#else
    {
        DetInfo.bID = ID_RL;
    }
#endif
    ADC_HardwareInit(); 
    Time_Init1msTimer();	 /* Initial the system timer: 1ms timer. */
    Time_Init32msTimer();        /* Initial the 32ms timer for echo detecting. */
    DetObj_InitSelfCheckVar();
    Cpu_EnableInterrupt();
    
    while(wSensorCheckTime)
    {
        if(bTimeFlag)
        {
            bTimeFlag = FALSE;
            DetObj_DetTheObj();
            wSensorCheckTime--;
            CLR_WDT;
        }
    }
    
    bSensorCheckFlag = FALSE;
    DetObj_Stop();
    
    if(DetInfo.bID == MASTER_SENSOR)
    {
        Cmd.Cursnr = ID_RL;
        Cmd.CurSta = CMD_DRIVE_SENSOR_SELF;
    }
    else
    {
        Serial_ISP = &GetHeader;
        Cmd.CurSta = CMD_WAIT_REQ;
        EnableInternalCommunicaiton();
    }
    DetInfo.bDetState = WAIT_DET_CMD;
    while(1)
    {
        if(bTimeFlag == TRUE)
        {
            bTimeFlag = FALSE;
            DetObj_DetTheObj();    /* Detect the object. */
        }
        CLR_WDT;                   /* Feed the dog. */
    }
}