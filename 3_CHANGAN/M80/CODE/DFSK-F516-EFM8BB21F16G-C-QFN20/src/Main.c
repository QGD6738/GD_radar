/*********************************************************************************************************************
** FILE NAME: Main.c
**
** PROJECT:  DFSK_F516_RADAR
** CPU:	     EFM8BB21F16G
** COMPILER: SILICON
** -------------------------------------------------------------------------------------------------------------------
** VERSION:  V0.1
** DATE:     2019.01.28
*********************************************************************************************************************/
//#include "Includes.h"
#include "Cpu.h"
#include "Adc.h"
#include "InitDevice.h"
#include "Timer.h"
#include "Serial.h"
#include "Uart.h"
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
    //InitUart0AllVar();
    Srl_InitVariable();
//    DetObj_InitVariable();
    DetObj_InitAllChannelDistance();
    for(i=0;i<ID_SUM;i++)
    {
    	DiagInfo.bSelfCheckResult[i] = 1;
    	Cmd.bSensorErrCnt[i] = 0;
    	Cmd.bSensorRun[i] = 0;
    }
    Cmd.bVechInfoErrCnt = 0;
    DetInfo.bID = ID_RL;
	DetInfo.bDetState = 0;
	DetInfo.bDetTriggerFlag = FALSE;
	CmpData.bCurBfNumber = 0;
	DiagInfo.bErrCounter = 0;
	RawData.bAdcBfIndex = 0;
	NearestData.wUpdataDelay = 0;
	Serial.byCnt = 0;
	Serial.byData = 0;
	Serial.bDataHeader = 0;
	Serial.bValid = 0;
	Serial.CmdHead_Rx = 0;
	Serial.bComparatorFlag = 0;
	Serial_ISP = &Srl_GetHeader;

    bSensorCheckFlag = TRUE;
//    wSensorCheckTime = 300;
//    wSensorCheckTime = 200;
    wSensorCheckTime = 300;

    //test
    for(i=0;i<ID_SUM;i++)
    {
    	 Cmd.Debug_ErrCnt[i] = 0;
    	 Cmd.Debug_OKCnt[i] = 0;
    }
    Cmd.Debug_bVechInfoErrCnt = 0;
    Cmd.Debug_bVechInfoOKCnt = 0;


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
	Cmd_InitVariable();
	Time_InitUart0Timer();
	Time_InitPCA0();				/* Initial the PAC0 timer for communication. */
	Cpu_EnableInterrupt();
	Delay1ms(100);
#if(TEST_CODE_FIXED_ID == FALSE)
	{
			ADC_ReadID();
	}
#else
	{
		DetInfo.bID  = ID_MASTER;
		Cmd.CmdStatus = DEBUG_MODE;
	}
#endif

/*
	if(DetInfo.bID  == ID_MASTER)
	{
		Cmd_GetVechileMode();
	}
*/
	//Selfcheck

	while(wSensorCheckTime)
	{
        if(bTimeFlag)
        {
            bTimeFlag = FALSE;
            DetObj_DetTheObj();
            wSensorCheckTime--;
            CLR_WDT;
        }
//        wSensorCheckTime = 300;
	}

	bSensorCheckFlag = FALSE;


	if(DetInfo.bID  == ID_MASTER)
	{
		CommunicaitonMode(COM_Internal);
		Cmd.wWaitTime = MASTER_WORK_TIME_MAX;
		Cmd.CmdStatus = MST_DRV_SENSOR_SELFCHECK;
		Cmd.bID = ID_RL;

#ifdef SPEAKER_USED
		Spk_InitVariable();
		Spk_InitHardware();
#endif
	}
	else
	{
		Cmd.CmdStatus = SLV_WAIT_CMD;
		CommunicaitonMode(COM_Internal);
	}
	DetObj_InitAllChannelDistance();
	while (1)
	{
		if(bTimeFlag)
		{
			bTimeFlag = FALSE;
			DetObj_DetTheObj();
			if(DetInfo.bID  == ID_MASTER)
			{
#ifdef SPEAKER_USED
				Spk_DriveTheSpeaker();
#endif
			}


//		CommunicaitonMode(COM_Uart);
////		The delay time need to adjust
////		Delay1ms(START_UART_DELAY);
//		Cmd_SendDistancetoBCM();
//		Delay1ms(20);
//		CommunicaitonMode(COM_Internal);
//		Delay1ms(20);
//			CommunicaitonMode(COM_Uart);
//			//The delay time need to adjust
//			Delay1ms(START_UART_DELAY);
//			Cmd_SendReqVechiModetoBCM();
//			Delay1ms(20);
//			CommunicaitonMode(COM_Internal);
//			Delay1ms(20);
//			NearestData.bNearestDistance = 20;
//			DiagInfo.bSelfCheckResult[0] = 1;
//			DiagInfo.bKeepFlag = 1;
//			Cmd.wBackData =(((WORD)NearestData.bNearestDistance)|
//					        ((WORD)DiagInfo.bSelfCheckResult[0]<<8)|
//							((WORD)DiagInfo.bKeepFlag<<9));
//			Srl_SendData(HEAD_1000, DetInfo.bID, ID_LEN_BIT, Cmd.wBackData, 10);
//			Delay1ms(20);
//			Cmd.bID = 0;
//			Srl_SendData(HEAD_800, Cmd.bID, ID_LEN_BIT, 0, 0);
//			Delay1ms(20);

			//驱动探头发送自检信息
//			Cmd.bID = ID_RL;
//			Srl_SendData(HEAD_2000, Cmd.bID, ID_LEN_BIT, 0, 0);
//			Delay1ms(20);


		}

		CLR_WDT;
	}
}
