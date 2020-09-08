/*********************************************************************************************************************
** FILE NAME: Main.c
**
** PROJECT:  HCXY_X30_RADAR
** CPU:	     EFM8BB21F16G
** COMPILER: SILICON
** -------------------------------------------------------------------------------------------------------------------
** VERSION:  V0.1
** DATE:     2019.02.21
*********************************************************************************************************************/
#include "Cpu.h"
#include "Adc.h"
#include "InitDevice.h"
#include "Timer.h"
#include "Uart.h"
#include "Serial.h"
#include "Command.h"
#include "DetectObj.h"
#include "EFM8BB2_FlashPrimitives.h"

//-----------------------------------------------------------------------------
// SiLabs_Startup() Routine
// ----------------------------------------------------------------------------
// This function is called immediately after reset, before the initialization
// code is run in SILABS_STARTUP.A51 (which runs before main() ). This is a
// useful place to disable the watchdog timer, which is enable by default
// and may trigger before main() in some instances.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
#define BUFFER_LENGTH  5
SI_SEGMENT_VARIABLE(buffer[BUFFER_LENGTH], uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(errorCount, uint32_t, SI_SEG_XDATA) = 0;

void SiLabs_Startup (void)
{
}

void InitGlobalVariable(void)
{
	BYTE i;
    InitUart1AllVar();

    DetObj_InitVariable();
    DetObj_InitAllChannelDistance();
    //bSensorCheckFlag = FALSE;
    //wSensorCheckTime = 300;
    //b50msFlag = FALSE;
    //DetInfo.bID  = ID_RL;
    //DetInfo.bDetSnNum = ID_SUM;
    

    for(i=0;i<DetInfo.bDetSnNum;i++)
    {
    	Cmd.bSensorErrCnt[i] = 0;
		Cmd.bDistance[i] = 0xFF;
    }
	Cmd.CmdStatus = MST_DRV_SENSOR;
	Cmd.wWaitTime = 0;
	/*
	Cmd.bBuzzerInfo = 
    Cmd.bObsorient[] = 
    Cmd.bOverCnt[] = 
	Cmd.bSensorRun[] = 
	Cmd.bUsedSnrNUM = 
	Cmd.bVechInfoErrCnt = 
	Cmd.CmdCalibration = 
	Cmd.wBackData = 
	*/

}

BYTE Debug_RD1 = 0x00,Debug_RD2 = 0;
void main (void)
{
	Cpu_Clk_Init();
	Cpu_InitHardwarePort();
	Cpu_WDG_Init(FUN_ON);
	Time_InitTheDelayTimer();
	Time_Init1msTimer();			/* Initial the system timer: 1ms timer. */
	Time_Init32msTimer();       	/* Initial the 32ms timer for echo detecting. */
	InitGlobalVariable();
	ADC_InitVariable();
	Time_InitVariable();
	UART1_Init();
	Cpu_EnableInterrupt();
	Delay1ms(200);
	Delay1ms(300);

	//Selfcheck
//	while(wSensorCheckTime)
//	{
//        if(bTimeFlag)
//        {
//            bTimeFlag = FALSE;
//            DetObj_DetTheObj();
//            wSensorCheckTime--;
//            CLR_WDT;
//        }
//	}
	//bSensorCheckFlag = FALSE;
	DetObj_InitAllChannelDistance();

	while (1)
	{
		if (bTimeFlag /*&& ADC.Power_OK_Flag*/)
		{
			bTimeFlag = FALSE;
			DetObj_DetTheObj();
			if(DetInfo.bID  == ID_MASTER)
			{
#ifdef SPEAKER_USED
				Spk_DriveTheSpeaker();
#endif
			}

		}
//		if(b50msFlag)
//		{
//			b50msFlag = FALSE;
//			/* Add adc detect code... */
//			ADC_PowerSample();
//		}

//		Cmd_SendDistancetoBCM();
//		Delay1ms(100);
//		Delay1ms(100);


		CLR_WDT;
	}
}
