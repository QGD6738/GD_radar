/*********************************************************************************************************************
** FILE NAME: DetectObj.c
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
#include "ADC.h"
#include "Command.h"
#include "Serial.h"

//*********************************************************************************************************************
//** Declear the global variable
//*********************************************************************************************************************/

#define INTERVAL_LEN    6
const BYTE  DetectInterval[INTERVAL_LEN] =
{
 (1),(2),(4),(7),(3)
};

BYTE xdata EachSNMaxDistance[SN_NUM];

StNearestData 	xdata 	NearestData;
StDetInfo 		xdata   DetInfo;
StEchoRawData 	xdata 	RawData;
StAnalysisData 	xdata   AnsData;
StCmpData 		xdata  	CmpData;
StNoise			xdata 	Noise;
StDiagInfo 		xdata 	DiagInfo;
StCalibrationData xdata  ClrtData;
CALLBACK  		xdata 	DetObj_DataPrcs;

/*const*/ WORD  TableWidth[] =
{
      300,/*	0cm	~	8.7cm	*/
      300,/*	8.7cm	~	17.4cm	*/
      300,/*	17.4cm	~	26.1cm	*/
      250,/*    26.1cm	~	34.8cm	*/
      200,/*    34.8cm	~	43.5cm  */
      200,/*	43.5cm	~	52.2cm	*/
      200,/*	52.2cm	~	60.9cm	*/
      200,/*	60.9cm	~	69.6cm	*/
      200,/*	69.6cm	~	78.3cm	*/
      200,/*	78.3cm	~	87.0cm	*/
      200,/*	87.0cm	~	95.7cm	*/
      200,/*	95.7cm	~	104.4cm	*/
      180,/*	104.4cm	~	113.1cm	*/
      180,/*	113.1cm	~	121.8cm	*/
      180,/*	121.8cm	~	130.5cm	*/
      150,/*	130.5cm	~	139.2cm	*/
      150,/*	139.2cm	~	147.9cm	*/
      150,/*	147.9cm	~	156.6cm	*/
      120,/*	156.6cm	~	165.3cm */
      120,/*	165.3cm	~	174.0cm */
};

const BYTE  TableHeight[] =
{
      70, /*	0cm	~	8.7cm	*/
      70, /*	8.7cm	~	17.4cm	*/
      70, /*	17.4cm	~	26.1cm	*/
      70, /*    26.1cm	~	34.8cm	*/
      36, /*    34.8cm	~	43.5cm  */
      34, /*	43.5cm	~	52.2cm	*/
      32, /*	52.2cm	~	60.9cm	*/
      28, /*	60.9cm	~	69.6cm	*/
      26, /*	69.6cm	~	78.3cm	*/
      24, /*	78.3cm	~	87.0cm	*/
      24, /*	87.0cm	~	95.7cm	*/
      24, /*	95.7cm	~	104.4cm	*/
      24, /*	104.4cm	~	113.1cm	*/
      20, /*	113.1cm	~	121.8cm	*/
      18, /*	121.8cm	~	130.5cm	*/
	  18, /*	130.5cm	~	139.2cm	*/
	  18, /*	139.2cm	~	147.9cm	*/
	  18, /*	147.9cm	~	156.6cm	*/
	  16, /*	156.6cm	~	165.3cm */
      16, /*	165.3cm	~	174.0cm */
};


void Null (void)
{

}
void DetObj_InitSelfCheckVar(void)
{
    BYTE i;
    bSensorCheckFlag = TRUE;
    DetInfo.bDetTimes = 2;
    wSensorCheckTime = 300;
    CmpData.bFirstDataFlag = TRUE;
    NearestData.bRingTimeFlag = 0x0F;		//
    for(i=0;i<SN_NUM;i++)
    {
         Cmd.bSensorRun[i] = TRUE;
         NearestData.bEachNearestDistance[i] = 0xFF;;
    }
    NearestData.bNearestDistance = 0xFF;
    DetInfo.bDetState = DET_OBJ_DETECT_INIT;
    DetObj_Stop();
}

void DetObj_InitVariable(void)
{
	WORD i,j;
	DetInfo.bDetState = 0;
	DetInfo.bID = 0;
	DetInfo.bDetTriggerFlag = 0;
	//DetInfo.bDetSnNum = 0;
	DetInfo.bDetTimes = 0;
	DetInfo.bDetWaitTime = 0;
	DetInfo.bDetNoiseDelayTime = 0;
	//DetInfo.wDetRealTime = 0;

	//DiagInfo.bErrSnNum = 0;
	//DiagInfo.wPowerUpChimeTime = 0;
	DiagInfo.bErrCounter = 0;
	DiagInfo.bKeepFlag = 0;
	for(i=0;i<SN_NUM;i++)
	{
		DiagInfo.bSelfCheckResult[i] = 0;
	}

	RawData.wStartTime = 0;
	RawData.bAdcBfIndex = 0;
	for(i=0;i<10;i++)
	{
		RawData.bAdcBuffer[i] = 0;
	}
	RawData.wRawDataIndex = 0;
	for(i=0;i<DETECT_BUFFER_LENGTH;i++)
	{
		RawData.bRawDataBuffer[i] = 0;
	}

	AnsData.bDetSampThre = 0;
	AnsData.bObsSigStartFlag = 0;
    //AnsData.wObsSigStartCnt = 0;
    //AnsData.wObsSigOverCnt = 0;
	AnsData.wObsSigExitCnt = 0;
	AnsData.bObsSigVolInto = 0;
	AnsData.bObsSigVolExit = 0;
	AnsData.wObsSigTimeInto = 0;
	AnsData.wObsSigTimeWidth = 0;
	AnsData.bObsSigVolHeight = 0;
#ifdef  EEPROM
	AnsData.bSampSN = 0;
#endif

	CmpData.bFirstDataFlag = 0;
    CmpData.bCurBfNumber = 0;
	CmpData.CCD.bDataValid = 0;
	CmpData.CCD.wDetTime = 0;
	CmpData.CCD.wRingTime = 0;
	CmpData.CCD.bDataNum = 0;
	CmpData.CCD.bSampThre = 0;
	for(i=0;i<OBJ_NUM;i++)
	{
		CmpData.CCD.bObsDistance[i] = 0;
	}
	for(i=0;i<DETECT_CMP_BUFFER_LENGTH;i++)
	{
		CmpData.EPD[i].bDataValid = 0;
		CmpData.EPD[i].wDetTime = 0;
		CmpData.EPD[i].wRingTime = 0;
		CmpData.EPD[i].bDataNum = 0;
		CmpData.EPD[i].bSampThre = 0;
		for(j=0;j<OBJ_NUM;j++)
		{
			CmpData.EPD[i].bObsDistance[j] = 0;
		}
		CmpData.Ns[i].bEchoRaisedTimes = 0;
		CmpData.Ns[i].bNoiseRaisedTimes = 0;
		CmpData.Ns[i].dObsIntegralArea = 0;
		CmpData.Ns[i].dNseIntegralArea = 0;
		CmpData.Ns[i].bNoiseMuchFlag = 0;
		CmpData.Ns[i].eObsNsType = OBS_BLUE;
	}

	NearestData.bRingTimeFlag = 0;
	NearestData.wUpdataDelay = 0;
	NearestData.bNearestDistance = 0;
	for(i=0;i<OBJ_NUM;i++)
	{
		NearestData.bEachNearestDistance[i] = 0;
	}
	NearestData.wRingTime = 0;
	NearestData.bRingTimeBounceFlag = 0;

	ClrtData.wRingTime = 0;
	ClrtData.wTimeWidth = 0;
	ClrtData.bVolHeight = 0;
	ClrtData.bDistance = 0;

}

void DetObj_InitAllChannelDistance(void)
{
    BYTE xdata i;
    DetInfo.bDetTimes = 2;
    CmpData.bFirstDataFlag = TRUE;
    NearestData.bRingTimeFlag = 0x0F;
    for(i=0;i<SN_NUM;i++)
    {
         NearestData.bEachNearestDistance[i] = 0xFF;
         Cmd.bDistance[i] = 0xFF;
         Cmd.bObsorient[i] = 0x00;
    }
    Cmd.bBuzzerInfo = 0x00;
    NearestData.bNearestDistance = 0xFF;
    DetInfo.bDetState = WAIT_DET_CMD;
    DetObj_Stop();
}

void DetObj_RealTimeClock(void)
{
    DetInfo.wDetRealTime++;
    if(NearestData.wUpdataDelay > 0)
    {
        NearestData.wUpdataDelay--;
    }
}
//
//void DetObj_SelfCheck(void)
//{
//    switch(DetInfo.bDetState)
//    {
//        case DET_OBJ_DETECT_INIT:
//            if(DetInfo.bDetWaitTime > 0 )
//            {
//                DetInfo.bDetWaitTime--;
//            }
//            else
//            {
//                if(DetInfo.bDetTimes > 0)
//                {
//                    DetInfo.bDetTimes --;
//                }
//                else
//                {
//                    DetObj_DiagnosticTheSensor();
//                    DetObj_InitTheEPD();
//                    DetObj_InitTheCCD();
//                }
//                DetObj_ClearTheRawDataBuff();
//                /* Change the detect state. */
//                DetInfo.bDetState = DET_OBJ_NOISE_SAMPLING;
//            }
//            break;
//        case DET_OBJ_NOISE_SAMPLING:
//            DetInfo.bDetWaitTime = DETECT_MAX_SAMPLING;
//            DetObj_ClearTheRawDataBuff();
//            ADC_InitDetect();
//            DetObj_Start();
//            DetInfo.bDetState = DET_OBJ_WAIT_SAMPLING;
//            break;
//        case DET_OBJ_WAIT_SAMPLING:
//            if(DetInfo.bDetWaitTime > 0)DetInfo.bDetWaitTime--;
//            else
//            {
//                DetObj_Stop();
//                DetObj_CalSampNoise();
//                DetInfo.bDetState = DET_OBJ_START_DETECT_OBSTACLE;
//            }
//            break;
//      case DET_OBJ_START_DETECT_OBSTACLE:
//            DetObj_ClearTheRawDataBuff();
//            DetObj_ClearTheNoiseBuff();
//            DetObj_InitTheEPD();
//            DetObj_SendThe58KhzUltraSonic();/* Need polishing send ultrasonic wave used time*/
//            ADC_InitDetect();
//            DetObj_Start();
//            DetInfo.bDetWaitTime = DETECT_MAX_DISTANCE_150CM;
//            CmpData.EPD[CmpData.bCurBfNumber].wDetTime = DetInfo.wDetRealTime;
//            DetInfo.bDetState = DET_OBJ_WAIT_ECHO;
//            break;
//        case DET_OBJ_WAIT_ECHO:
//        {
//            if(DetInfo.bDetWaitTime > 0)DetInfo.bDetWaitTime--;
//            else
//            {
//                DetObj_Stop();
//                DetObj_RawEchoDataAns();
//                DetObj_DiagnosticTheRingTime();
//                DetInfo.bDetNoiseDelayTime = 5;
//                DetInfo.bDetState = DET_OBJ_DETECT_INIT;     /* Change the detect state. */
//            }
//            break;
//        }
//        default:
//        {
//            Cpu_InitHardwarePort();	 /* Initial the hardware config for echo detecting block. */
//            DetInfo.bDetState = DET_OBJ_DETECT_INIT;
//            break;
//        }
//    }
//}

void DetObj_SendThe58KhzUltraSonic(void)
{
    volatile BYTE xdata i;
	//static BYTE xdata i;
    Cpu_DisableInterrupt();
    for(i=PULE_NUM;i>0;i--)             /* Send the 58KHz ultrasonic. */
    {
        DRIVE_PORT_1 = 0;
        DRIVE_PORT_2 = 1;
        Delay1us(6);
        NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
        NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
        DRIVE_PORT_1 = 1;
        DRIVE_PORT_2 = 0;
        Delay1us(6);
        NOP();NOP();NOP();NOP();NOP();//NOP();//NOP();//NOP();//NOP();//NOP();NOP();//NOP();
    }
    DRIVE_PORT_1 = 0;
    DRIVE_PORT_2 = 0;
    RawData.wStartTime = PULE_NUM*17;
    Cpu_EnableInterrupt();
}


/*********************************************************************************************************************
** Diagnostic.
*********************************************************************************************************************/
void DetObj_DiagnosticTheSensor(void)
{
    if(DiagInfo.bErrCounter >  SN_ERR_THRESHOLD)
    {
        DiagInfo.bSelfCheckResult[DetInfo.bID] = 1;
    }
    else
    {
        DiagInfo.bSelfCheckResult[DetInfo.bID] = 0;
    }
}

/*********************************************************************************************************************
** Process the echo.
*********************************************************************************************************************/
void DetObj_PreEchoProcess(void)
{
}

/*********************************************************************************************************************
** Diagnostic the ring time.
*********************************************************************************************************************/
void DetObj_DiagnosticTheRingTime(void)
{
    if(NearestData.bRingTimeFlag > 0)
    {
        if((CmpData.EPD[CmpData.bCurBfNumber].wRingTime > RING_TIME_MIN_VALUE) &&
           (CmpData.EPD[CmpData.bCurBfNumber].wRingTime < RING_TIME_MID_VALUE))
        {
            NearestData.bRingTimeFlag >>= 1;
            NearestData.wRingTime = CmpData.EPD[CmpData.bCurBfNumber].wRingTime;
            if(DiagInfo.bErrCounter > 0)
            {
                DiagInfo.bErrCounter--;
            }
        }
        else if((CmpData.EPD[CmpData.bCurBfNumber].wRingTime >= RING_TIME_MID_VALUE) &&
                (CmpData.EPD[CmpData.bCurBfNumber].wRingTime < RING_TIME_MAX_VALUE_WITH_OBSTACLE))
        {
            NearestData.bRingTimeFlag >>= 1;
            NearestData.wRingTime = RING_TIME_MID_VALUE;
        }
        else
        {
            if(DiagInfo.bErrCounter < 8)
            {
                DiagInfo.bErrCounter++;
            }
        }
		if(NearestData.wRingTime < RING_TIME_RECOVER_VALUE)
		{
			NearestData.wRingTime = RING_TIME_MID_VALUE;
		}
    }
    else
    {
        /* Diagnostic the sensor. */
        if((CmpData.EPD[CmpData.bCurBfNumber].wRingTime > RING_TIME_MAX_VALUE_WITH_OBSTACLE) ||
           (CmpData.EPD[CmpData.bCurBfNumber].wRingTime < RING_TIME_MIN_VALUE))
        {
            if(DiagInfo.bErrCounter < 8)
            {
                DiagInfo.bErrCounter++;
            }
        }
        else
        {
            /* If the ring time is valid. Renew it, or abandon it. */
            if(DiagInfo.bErrCounter > 0)
            {
                DiagInfo.bErrCounter--;
            }
        }
    }
    DetObj_DiagnosticTheSensor();
}

///*********************************************************************************************************************
//** Save interrupt data into buff.
//*********************************************************************************************************************/

void DetObj_Start(void)
{
    Time_InitAdcTriggerTimer();
    Time_StartAdcTrigger();
    ADC_Start();
}

void DetObj_Stop(void)
{
    Time_StopAdcTrigger();
    ADC_Stop();
}

void DetObj_InitTheAnalysisData(void)
{
    AnsData.bObsSigStartFlag = FALSE;
}

/*********************************************************************************************************************
** Initial the current buff.
*********************************************************************************************************************/
void DetObj_InitTheEPD(void)
{
    BYTE i;
    CmpData.EPD[CmpData.bCurBfNumber].bDataValid = FALSE;
    CmpData.EPD[CmpData.bCurBfNumber].wDetTime = 0;
    CmpData.EPD[CmpData.bCurBfNumber].wRingTime = 0xFFFF;
    CmpData.EPD[CmpData.bCurBfNumber].bDataNum = 0;

    ClrtData.bDistance  = 0XFF;
    ClrtData.bVolHeight = 0;
    ClrtData.wTimeWidth = 0;
    ClrtData.wRingTime  = 0;

    for(i=0; i<OBJ_NUM; i++)
    {
        CmpData.EPD[CmpData.bCurBfNumber].bObsDistance[i] = NO_OBJ;
    }
}

/*********************************************************************************************************************
** Initial the compare buff.
*********************************************************************************************************************/
void DetObj_InitTheCCD(void)
{
    BYTE i;
    CmpData.CCD.bDataValid = FALSE;
    CmpData.CCD.wDetTime = 0;
    CmpData.CCD.wRingTime = 0xFFFF;
    CmpData.CCD.bDataNum = 0;
    DiagInfo.bKeepFlag = FALSE;

    CmpData.bCurBfNumber = 0;
    CmpData.bFirstDataFlag = TRUE;
    for(i=0; i<DETECT_CMP_BUFFER_LENGTH; i++)
    {
        CmpData.EPD[i].bDataValid = FALSE;
    }
    for(i=0; i<OBJ_NUM; i++)
    {
        CmpData.CCD.bObsDistance[i] = NO_OBJ;
    }
}

/*********************************************************************************************************************
** Clear the raw data buff.
*********************************************************************************************************************/
void DetObj_ClearTheRawDataBuff(void)
{
    WORD i;
    for(i=0; i<DETECT_BUFFER_LENGTH;i++)
    {
        RawData.bRawDataBuffer[i] = 0;
    }
    RawData.wRawDataIndex = 0;
    RawData.wStartTime = 0;
    DetObj_InitTheAnalysisData();
}

BYTE GetDistance(WORD ObsTime,WORD EchoWidth,BYTE VolHeight)
{
    BYTE ret = NO_OBJ;
    BYTE Index;

    Index = ObsTime >> 9;
    if(EchoWidth >= TableWidth[Index] && VolHeight >= TableHeight[Index])
    {
        ObsTime >>= 7;
        ret = (ObsTime*557) >> 8;
    }
    return ret;
}

/*********************************************************************************************************************
** Clear the noise buff.
*********************************************************************************************************************/
void DetObj_ClearTheNoiseBuff(void)
{
    CmpData.Ns[CmpData.bCurBfNumber].eObsNsType= OBS_BLUE;
    CmpData.Ns[CmpData.bCurBfNumber].bEchoRaisedTimes = 0;
    CmpData.Ns[CmpData.bCurBfNumber].bNoiseRaisedTimes = 0;
    CmpData.Ns[CmpData.bCurBfNumber].dNseIntegralArea = 0;
    CmpData.Ns[CmpData.bCurBfNumber].dObsIntegralArea = 0;
    CmpData.Ns[CmpData.bCurBfNumber].bNoiseMuchFlag = FALSE;
}

void DetObj_CopySamplingBuffer(BYTE* SP, BYTE* EP, BYTE Len)
{
    BYTE i;
    for(i=0; i<Len; i++)
    {
        *(EP+i) = *(SP+i);
    }
}

#define NULL_DATA_LEN    2
void DetObj_CalSampNoise(void)
{
    BYTE xdata i,j;
    BYTE xdata temp;
    BYTE xdata add;
    WORD xdata AdLow,AdMid,AdHigh;

    DetObj_CopySamplingBuffer(RawData.bRawDataBuffer,(&RawData.bRawDataBuffer[RawData.wRawDataIndex+1]),(48+NULL_DATA_LEN));

    add = RawData.wRawDataIndex + 1 + NULL_DATA_LEN;

    for(i=add; i<48+add; i++)
    {
        for(j=i; j<48+add; j++)
        {
            if(RawData.bRawDataBuffer[i] > RawData.bRawDataBuffer[j])
            {
                temp = RawData.bRawDataBuffer[i];
                RawData.bRawDataBuffer[i] = RawData.bRawDataBuffer[j];
                RawData.bRawDataBuffer[j] = temp;
            }
        }
    }
    AdLow = 0;
    AdMid = 0;
    AdHigh = 0;
    for(i=add; i<16+add;i++)
    {
        AdLow  += RawData.bRawDataBuffer[i];
        AdMid  += RawData.bRawDataBuffer[i+16];
        AdHigh += RawData.bRawDataBuffer[i+32];
    }
    AdLow >>= 4;
    AdMid >>= 4;
    AdHigh>>= 4;

    AnsData.bDetSampThre = AdMid + ((AdHigh-AdMid)>>2);

    if(AnsData.bDetSampThre > DETECT_NOISE_BASE_VOL)
    {
    	temp = AnsData.bDetSampThre - DETECT_NOISE_BASE_VOL;
		temp = temp>>2;
		AnsData.bDetSampThre += (temp*temp); //scaled-up
    }

#ifdef EEPROM
    AnsData.bDetSampThre += AnsData.bSampSN;
#endif

    if(AnsData.bDetSampThre > DETECT_NOISE_MAX_VOL)
    {
        AnsData.bDetSampThre = DETECT_NOISE_MAX_VOL;
        CmpData.Ns[CmpData.bCurBfNumber].eObsNsType = OBS_RED;
    }
    else if(AnsData.bDetSampThre > DETECT_NOISE_MID_VOL)
    {
        CmpData.Ns[CmpData.bCurBfNumber].eObsNsType = OBS_YELLOW;
    }
    else
    {
        CmpData.Ns[CmpData.bCurBfNumber].eObsNsType = OBS_BLUE;
    }
    CmpData.EPD[CmpData.bCurBfNumber].bSampThre = AnsData.bDetSampThre;

    DetObj_RawNoiseDataAns();

}


void DetObj_RawFilter(void)
{
    WORD xdata i = 0;
    WORD xdata j = 0;
    WORD xdata k = 0;
    BYTE xdata Temp = 0;

#ifdef   START_VALUE_POLISHING
    for(i=0;i<RawData.wRawDataIndex;i++)
    {
        if(RawData.bRawDataBuffer[i] != 0)
        {
            Temp = RawData.bRawDataBuffer[i];
            j = i;
            if(j > START_VALUE_POLISHING_LEN)
            {
                DiagInfo.bErrCounter++;
            }
            break;
        }
    }
    for(i=0; i<j; i++)
    {
        RawData.bRawDataBuffer[i] = Temp;
    }
#endif

#ifdef   SINGLE_POINT_JUMP_ZERO_FILTER
    for(i=1; i<(RawData.wRawDataIndex-1);i++)
    {
        if(RawData.bRawDataBuffer[i] == 0)
        {
            /*if(RawData.bRawDataBuffer[i-1] > (AnsData.bDetSampThre<<1) &&
               RawData.bRawDataBuffer[i+1] > (AnsData.bDetSampThre<<1)   )*/
            {
            	RawData.bRawDataBuffer[i] = RawData.bRawDataBuffer[i-1];
            }
        }
    }
#endif

#ifdef    PEAK_DISTURB_FILTER
    for(i=5; i<(RawData.wRawDataIndex-2);i++)
    {
        if(RawData.bRawDataBuffer[i+1] > RawData.bRawDataBuffer[i]+PEAK_DISTURB_FILTER_HIGH &&
           RawData.bRawDataBuffer[i+2] > RawData.bRawDataBuffer[i]+PEAK_DISTURB_FILTER_HIGH+(PEAK_DISTURB_FILTER_HIGH>>1))
        {
            for(j=i+1; j<(i+(PEAK_DISTURB_FILTER_WIDTH/COUNT_TIME_UNIT));j++)
            {
                if(RawData.bRawDataBuffer[j] < RawData.bRawDataBuffer[i]+(PEAK_DISTURB_FILTER_HIGH>>2))
                {
                    CmpData.Ns[CmpData.bCurBfNumber].bEchoRaisedTimes++;
                    for(k=i; k<j;k++)
                    {
                        RawData.bRawDataBuffer[k] = RawData.bRawDataBuffer[i-1];
                    }
                    break;
                }
            }
        }
    }
#endif
}

void DetObj_RawEchoDataAns(void)
{
    WORD xdata i;
    BYTE xdata Distc;
    WORD xdata EchoWidth;

    WORD xdata ObsStartThre;
    WORD xdata ObsExitThreFixed;


    ObsStartThre = CmpData.EPD[CmpData.bCurBfNumber].bSampThre + OBSTACLE_START_VOL;
    /* subtractive sampling noise */
    ObsExitThreFixed = CmpData.EPD[CmpData.bCurBfNumber].bSampThre -
                      (CmpData.EPD[CmpData.bCurBfNumber].bSampThre >> OBSTACLE_EXIT_VOL_SHIFT);

    DetObj_RawFilter();

    RawData.bRawDataBuffer[RawData.wRawDataIndex-1] = 0; /* the farthest distance exit flag */

    /* Analyze detect data */
    for(i=0; i<RawData.wRawDataIndex;i++)
    {
        CmpData.Ns[CmpData.bCurBfNumber].dObsIntegralArea += RawData.bRawDataBuffer[i];
        if(RawData.bRawDataBuffer[i] > ObsStartThre)
        {
            if(AnsData.bObsSigStartFlag == FALSE)
            {
                //AnsData.wObsSigOverCnt = 0;
                //if(++AnsData.wObsSigStartCnt > 0)
                {
                    AnsData.bObsSigStartFlag = TRUE;
                    AnsData.bObsSigVolInto = RawData.bRawDataBuffer[i];
                    AnsData.wObsSigTimeInto = RawData.wStartTime + i*COUNT_TIME_UNIT;
                    AnsData.wObsSigTimeWidth = 0;
                    AnsData.bObsSigVolHeight = 0;
                    AnsData.wObsSigExitCnt = 0;
                }
            }
        }
        /* this possible rewrite packaging a function , differentiate the different of noise*/
        if(RawData.bRawDataBuffer[i] < (ObsExitThreFixed+(AnsData.bObsSigVolHeight>>OBSTACLE_EXIT_VOL_SHIFT)))
        {
            if(AnsData.bObsSigStartFlag == TRUE)
            {
                //AnsData.wObsSigStartCnt = 0;
                //if(++AnsData.wObsSigOverCnt > 0)
                {
                    AnsData.bObsSigStartFlag = FALSE;
                    if(AnsData.bObsSigVolHeight > (DETECT_NOISE_BASE_VOL<<1)
                    && AnsData.wObsSigTimeWidth > DETECT_NOISE_WIDTH )
                    {
                        CmpData.Ns[CmpData.bCurBfNumber].bEchoRaisedTimes++;
                    }
                    AnsData.bObsSigVolExit = RawData.bRawDataBuffer[i];
                    /* generate obstacle distance */
                    if(CmpData.EPD[CmpData.bCurBfNumber].bDataNum == 0)
                    {
                        CmpData.EPD[CmpData.bCurBfNumber].wRingTime = RawData.wStartTime + AnsData.wObsSigTimeWidth + ((AnsData.bObsSigVolExit)<<0);
                        ClrtData.wRingTime = CmpData.EPD[CmpData.bCurBfNumber].wRingTime;
                        CmpData.EPD[CmpData.bCurBfNumber].bDataNum++;
                        continue;
                    }

#ifdef  OBS_ECHO_WIDTH_COMPEN
                    EchoWidth = AnsData.wObsSigTimeWidth + ((AnsData.bObsSigVolInto + AnsData.bObsSigVolExit) << 0); // y = x;
#else
                    EchoWidth = AnsData.wObsSigTimeWidth;
#endif
                    if(CmpData.EPD[CmpData.bCurBfNumber].bDataNum < OBJ_NUM)
                    {
                        Distc = GetDistance(AnsData.wObsSigTimeInto,EchoWidth,AnsData.bObsSigVolHeight);
                        if(Distc != NO_OBJ)
                        {
                            CmpData.EPD[CmpData.bCurBfNumber].bObsDistance[CmpData.EPD[CmpData.bCurBfNumber].bDataNum] = Distc;
                            if(CmpData.EPD[CmpData.bCurBfNumber].bDataNum == 1)
                            {
                                ClrtData.wTimeWidth = EchoWidth;
                                ClrtData.bVolHeight = AnsData.bObsSigVolHeight;
                               // ClrtData.bDistance = NearestData.bEachNearestDistance[DetInfo.bID];
                            }
                            CmpData.EPD[CmpData.bCurBfNumber].bDataNum++;
                        }
                    }
                }
            }
        }

        if(AnsData.bObsSigStartFlag == TRUE)
        {
            AnsData.wObsSigTimeWidth += COUNT_TIME_UNIT;
            if(AnsData.bObsSigVolHeight < RawData.bRawDataBuffer[i])
            {
                AnsData.bObsSigVolHeight = RawData.bRawDataBuffer[i];
            }
            /*voltage signel height donot increase to time��exit check; use to sampling voltage is relatively low*/
            if(AnsData.bObsSigVolHeight < (ObsStartThre <<1) &&
               AnsData.wObsSigTimeWidth < ((WORD)COUNT_TIME_UNIT<<3)) /* sampling of 8 time */
            {
                if(++AnsData.wObsSigExitCnt >= 4)
                {
                    AnsData.bObsSigStartFlag = FALSE;
                }
            }
        }
    }

    CmpData.EPD[CmpData.bCurBfNumber].bDataValid = TRUE;

}

void DetObj_RawNoiseDataAns(void)
{
    WORD xdata i = 0;

    RawData.bRawDataBuffer[RawData.wRawDataIndex-1] = 0; /* the farthest distance exit flag */

    /* Analyze noise data */
    for(i=0; i<RawData.wRawDataIndex;i++)
    {
        CmpData.Ns[CmpData.bCurBfNumber].dNseIntegralArea += RawData.bRawDataBuffer[i];
        if(RawData.bRawDataBuffer[i] > AnsData.bDetSampThre+OBSTACLE_START_VOL)
        {
            if(AnsData.bObsSigStartFlag == FALSE)
            {
                //AnsData.wObsSigOverCnt = 0;
                //if(++AnsData.wObsSigStartCnt > 1)
                {
                    AnsData.bObsSigStartFlag = TRUE;
                    AnsData.bObsSigVolHeight = 0;
                    AnsData.wObsSigTimeWidth = 0;
                }
            }
        }
        else if(RawData.bRawDataBuffer[i] < (AnsData.bObsSigVolHeight>>OBSTACLE_EXIT_VOL_SHIFT))
        {
            if(AnsData.bObsSigStartFlag == TRUE)
            {
                //AnsData.wObsSigStartCnt = 0;
                //if(++AnsData.wObsSigOverCnt > 0)
                {
                    AnsData.bObsSigStartFlag = FALSE;
                    if(AnsData.bObsSigVolHeight > (DETECT_NOISE_BASE_VOL<<1)
                    && AnsData.wObsSigTimeWidth > DETECT_NOISE_WIDTH )
                    {
                        CmpData.Ns[CmpData.bCurBfNumber].bNoiseRaisedTimes++;
                    }
                }
            }
        }

        if(AnsData.bObsSigStartFlag == TRUE)
        {
            AnsData.wObsSigTimeWidth += COUNT_TIME_UNIT;
            if(AnsData.bObsSigVolHeight < RawData.bRawDataBuffer[i])
            {
                AnsData.bObsSigVolHeight = RawData.bRawDataBuffer[i];
            }
        }
    }
}

/*********************************************************************************************************************
** Detect the obstacle.
*********************************************************************************************************************/
void DetObj_DetTheObj(void)
{
    switch(DetInfo.bDetState)
    {
      case WAIT_DET_CMD:
            /* add your code to here */
    	  	Cmd_Process();
            //DetInfo.bDetTriggerFlag = TRUE;
            if(DetInfo.bDetTriggerFlag == TRUE)//Detect cmd is true
            {
                DetInfo.bDetTriggerFlag = FALSE;
                DetObj_InitTheCCD();
                DetInfo.bDetNoiseDelayTime = 0;//Wait data send
                DetInfo.bDetTimes = DET_CMP_TIMES;
                DetInfo.bDetState = DET_OBJ_DETECT_INIT;
            }

            break;
      case DET_OBJ_DETECT_INIT:
            if(DetInfo.bDetNoiseDelayTime > 0)DetInfo.bDetNoiseDelayTime--;
            else
            {
                DetInfo.bDetState = DET_OBJ_NOISE_SAMPLING;
            }
            break;
      case DET_OBJ_NOISE_SAMPLING:
            DetInfo.bDetWaitTime = DETECT_MAX_SAMPLING;
            DetObj_ClearTheRawDataBuff();
            DetObj_ClearTheNoiseBuff();
			ADC_InitDetect();
			DetObj_Start();
			DetInfo.bDetState = DET_OBJ_WAIT_SAMPLING;
            break;
      case DET_OBJ_WAIT_SAMPLING:
            if(DetInfo.bDetWaitTime > 0)DetInfo.bDetWaitTime--;
            else
            {
                DetObj_Stop();
                DetObj_CalSampNoise();
                DetInfo.bDetState = DET_OBJ_START_DETECT_OBSTACLE;
            }
            break;
      case DET_OBJ_START_DETECT_OBSTACLE:
            DetObj_ClearTheRawDataBuff();
            DetObj_InitTheEPD();
            DetObj_SendThe58KhzUltraSonic();/* Need polishing send ultrasonic wave used time*/
            ADC_InitDetect();
            DetObj_Start();
            DetInfo.bDetWaitTime = DETECT_MAX_DISTANCE_150CM;
            CmpData.EPD[CmpData.bCurBfNumber].wDetTime = DetInfo.wDetRealTime;

            DetInfo.bDetState = DET_OBJ_WAIT_ECHO;
            break;
      case DET_OBJ_WAIT_ECHO:
            if(DetInfo.bDetWaitTime > 0)DetInfo.bDetWaitTime--;
            else
            {
                DetObj_Stop();
                DetObj_RawEchoDataAns();
                DetInfo.bDetState = DET_OBJ_HANDLE_NOISE;
            }
            break;
      case DET_OBJ_HANDLE_NOISE:
            DetObj_DiagnosticTheRingTime();
            /*  Much disturb keep befor status arithmetic */
            if(
                CmpData.Ns[CmpData.bCurBfNumber].eObsNsType == OBS_RED ||
                  (
                  CmpData.Ns[CmpData.bCurBfNumber].bEchoRaisedTimes >= 22
              ||  CmpData.Ns[CmpData.bCurBfNumber].bNoiseRaisedTimes>= 3
              ||  CmpData.Ns[CmpData.bCurBfNumber].dObsIntegralArea >= ((DWORD)75*DETECT_MAX_DISTANCE_150CM*1000/COUNT_TIME_UNIT)
              ||  CmpData.Ns[CmpData.bCurBfNumber].dNseIntegralArea >= ((DWORD)20*DETECT_MAX_NOISE_TIME*1000/COUNT_TIME_UNIT)
                  )
               )
            {
                DetInfo.bDetTimes = 0;
                DetObj_HoldTheObstaclestate();
                CmpData.Ns[CmpData.bCurBfNumber].bNoiseMuchFlag = TRUE;
            }
            else
            {
                DetObj_CompareTheData();
            }
            DetInfo.bDetState = DET_OBJ_GENERATE_RETURN;
            break;
      case DET_OBJ_GENERATE_RETURN:
            DetObj_ProcessTheRingTime();
            if(bSensorCheckFlag == TRUE)DetInfo.bDetTimes++;
            if(DetInfo.bDetTimes > 0)
            {
                DetInfo.bDetTimes --;
                DetInfo.bDetNoiseDelayTime = DetectInterval[DetInfo.wDetRealTime%INTERVAL_LEN];
                CmpData.bCurBfNumber = (CmpData.bCurBfNumber+1)%DETECT_CMP_BUFFER_LENGTH;
                DetInfo.bDetState = DET_OBJ_DETECT_INIT;
            }
            else
            {
                DetObj_GenerateTheNearestObstacle();
                DetInfo.bDetState = WAIT_DET_CMD;
                if(DetInfo.bID == ID_MASTER)
                {
                	Cmd.CmdStatus = MST_CUT_SNR;
                }
                else
                {
                	if(Cmd.CmdStatus == SLV_CALIBRATION_BACK)  //workfrock command
                	{
                		Cmd.CmdStatus = SLV_CALIBRATION_BACK;
                	}
                	else//detect
                	{
                		Cmd.CmdStatus = SLV_DISTANCE_BACK;
                	}
                }
            }
            break;
      default:
            DetInfo.bDetState = DET_OBJ_DETECT_INIT;
            break;
    }
}

void DetObj_HoldTheObstaclestate(void)
{
    BYTE xdata i;
    CmpData.bFirstDataFlag = FALSE;
    NearestData.bRingTimeBounceFlag = FALSE;
    CmpData.EPD[CmpData.bCurBfNumber].bDataNum = 1;
    NearestData.wUpdataDelay = DETECT_DELAY_UPDATE_HIGH;
    CmpData.CCD.wRingTime = NearestData.wRingTime;
    DiagInfo.bKeepFlag = TRUE;
    CmpData.CCD.bObsDistance[0] = NearestData.bEachNearestDistance[DetInfo.bID];
    for(i=1; i<OBJ_NUM; i++)
    {
        CmpData.CCD.bObsDistance[i] = NO_OBJ;
    }
}

/*********************************************************************************************************************
** Generate the nearest obstacle.
*********************************************************************************************************************/
void DetObj_GenerateTheNearestObstacle(void)
{
    BYTE xdata i,bDis,bDiff;
    BYTE bFrockDis;

    /* Frock data */
    bFrockDis = 0xFF;
    for(i=0;i<CmpData.CCD.bDataNum;i++)
    {
        if(bFrockDis > CmpData.CCD.bObsDistance[i])
        {
        	bFrockDis= CmpData.CCD.bObsDistance[i];
        }
    }
    ClrtData.bDistance = bFrockDis;


    /* Get the nearest obstacle of this time detected. */
    bDis = NO_OBJ;
    for(i=0;i<CmpData.CCD.bDataNum;i++)
    {
        if(CmpData.CCD.bObsDistance[i] > EachSNMaxDistance[DetInfo.bID])
        {
            CmpData.CCD.bObsDistance[i] = NO_OBJ;
        }
    }

    for(i=0;i<CmpData.CCD.bDataNum;i++)
    {
        if(bDis > CmpData.CCD.bObsDistance[i])
        {
            bDis = CmpData.CCD.bObsDistance[i];
        }
    }
    /* use for Continue alarming to ring time is solder projection*/
    if(bDis < 25)
    {
        if(bDis > 20)
        {
            bDis -= 3;
        }
        else /* ring time break */
        {
            if(  (bDis != OBSTACLE_IN_RING_TIME_VALUE)
               &&(bDis != OBSTACLE_IN_CONTINUOUS_DISTANCE_DELAY)
               &&(bDis != OBSTACLE_IN_CONTINUOUS))
            {
                /* hold the state */
                bDis = NearestData.bEachNearestDistance[DetInfo.bID];
            }
        }
    }

    if(bDis > MAX_DETECT_DISTANCE)
    {
        if(NearestData.wUpdataDelay == 0)
        {
            NearestData.bEachNearestDistance[DetInfo.bID] = NO_OBJ;
        }
    }
    else
    {
        if(bDis < CONTINUOUS_CHIME_DISTANCE)
        {
            if(bDis < 25)
                NearestData.bEachNearestDistance[DetInfo.bID] = OBSTACLE_IN_CONTINUOUS;
            else
                NearestData.bEachNearestDistance[DetInfo.bID] = bDis;
            NearestData.wUpdataDelay = DETECT_DELAY_UPDATE_HIGH;
        }
        else if(bDis < MID_DETECT_DISTANCE)
        {
            NearestData.bEachNearestDistance[DetInfo.bID] = bDis;
            NearestData.wUpdataDelay= DETECT_DELAY_UPDATE_LOW;
        }
        else
        {
            NearestData.bEachNearestDistance[DetInfo.bID] = bDis;
            NearestData.wUpdataDelay= DETECT_DELAY_UPDATE_LOW;
        }
    }

    /* Get the nearest obstacle of all the sensor detected. */
    bDis = NO_OBJ;
    for(i=0;i<SN_NUM;i++)
    {
        if(bDis > NearestData.bEachNearestDistance[i])
        {
            bDis = NearestData.bEachNearestDistance[i];
        }
    }
    /* Compare the distance to judge if the obstacle has been moved. */
    if(bDis > NearestData.bNearestDistance)
    {
        /* The obstacle move far away from the vehicle. */
        /* Compute the difference. */
        bDiff = bDis - NearestData.bNearestDistance;
        if(bDiff > OBSTACLE_MOVE_FAR_AWAY)
        {
            /* Need to change the distance display. */
            NearestData.bNearestDistance = bDis;
        }
    }
    else
    {
        /* The obstacle move close to the vehicle. */
        bDiff = NearestData.bNearestDistance - bDis;
        if(bDiff > OBSTACLE_MOVE_CLOSE)
        {
            /* Need to change the distance display. */
            NearestData.bNearestDistance = bDis;
        }
    }


}


/*********************************************************************************************************************
** Process the ring time.
*********************************************************************************************************************/
void DetObj_ProcessTheRingTime(void)
{
     WORD xdata wRingTimeDiff;

    if(CmpData.Ns[CmpData.bCurBfNumber].bNoiseMuchFlag == TRUE)return;
    if(CmpData.CCD.wRingTime == 0xFFFF)return; /* Current sensor is failure */

    if(NearestData.bRingTimeBounceFlag == FALSE)
    {
        if(CmpData.CCD.wRingTime > NearestData.wRingTime)
        {
            /* Compute the difference between the ring time saved and this time detected. */
            wRingTimeDiff = CmpData.CCD.wRingTime - NearestData.wRingTime;
            if(wRingTimeDiff > RING_TIME_CHANGE_THRESHOLD)
            {
                if(CmpData.CCD.wRingTime > RING_TIME_MAX_VALUE)
                {
                    CmpData.CCD.bObsDistance[0] = OBSTACLE_IN_RING_TIME_VALUE;
                    if(CmpData.CCD.bDataNum < OBJ_NUM)
                    {
                        CmpData.CCD.bDataNum++;
                    }
                }
                else
                {
                    if(wRingTimeDiff > OBSTALE_IN_RING_TIME_THRESHOLD)
                    {
                        CmpData.CCD.bObsDistance[0] = OBSTACLE_IN_RING_TIME_VALUE;
                        if(CmpData.CCD.bDataNum < OBJ_NUM)
                        {
                            CmpData.CCD.bDataNum++;
                        }
                    }
                    else
                    {
                        CmpData.CCD.bObsDistance[0] = NearestData.bEachNearestDistance[DetInfo.bID];
                        if(CmpData.CCD.bDataNum == 0 )
                        {
                            CmpData.CCD.bDataNum ++;
                        }
                    }
                }
            }
        }
        else
        {
        }
    }
}


/*********************************************************************************************************************
** Compare the obstacle received after sending two times of the ultrasonic.
*********************************************************************************************************************/
void DetObj_CompareTheData(void)
{
    WORD  xdata RingTimeDiff;
    BYTE  xdata DistanceDiff;
    BYTE  xdata bObsDistance[OBJ_NUM];
    BYTE  xdata bEPDObsDistance[OBJ_NUM];
    BYTE  xdata k = 0;
    BYTE  xdata i,j;

    if(CmpData.bFirstDataFlag == TRUE)
    {
        CmpData.bFirstDataFlag = FALSE;
        /* Save the data first time received. */
        j = 0;
        for(i=0;i<CmpData.EPD[CmpData.bCurBfNumber].bDataNum;i++)
        {
            if(CmpData.EPD[CmpData.bCurBfNumber].bObsDistance[i] != NO_OBJ)
            {
                CmpData.CCD.bObsDistance[j] = CmpData.EPD[CmpData.bCurBfNumber].bObsDistance[i];
                j++;
            }
        }
        CmpData.CCD.bDataNum = j;
        CmpData.CCD.wRingTime = CmpData.EPD[CmpData.bCurBfNumber].wRingTime;

        if(NearestData.bRingTimeFlag == 0)
        {
            if(CmpData.CCD.bDataNum ==0)
            {
                if(CmpData.EPD[CmpData.bCurBfNumber].wRingTime < (NearestData.wRingTime + RENEW_RING_TIME_THRESHOLD))
                {
                    DetInfo.bDetTimes = 0;/*exit*/
                }
            }
        }
    }
    else
    {
        /* Compare the ring time. */
        if(CmpData.EPD[CmpData.bCurBfNumber].wRingTime > CmpData.CCD.wRingTime)
        {
            RingTimeDiff = CmpData.EPD[CmpData.bCurBfNumber].wRingTime - CmpData.CCD.wRingTime;
        }
        else
        {
            RingTimeDiff = CmpData.CCD.wRingTime - CmpData.EPD[CmpData.bCurBfNumber].wRingTime;
        }
        if(RingTimeDiff < RENEW_RING_TIME_THRESHOLD)/*if the value is so small maybe inferrence the EMC*/
        {
            NearestData.bRingTimeBounceFlag = FALSE;
            CmpData.CCD.wRingTime = CmpData.EPD[CmpData.bCurBfNumber].wRingTime;
        }
        else
        {
            /* hold the distance */
            CmpData.CCD.bObsDistance[0] = NearestData.bEachNearestDistance[DetInfo.bID];
            if(CmpData.CCD.bDataNum < OBJ_NUM)
            {
                CmpData.CCD.bDataNum++;
            }
            NearestData.bRingTimeBounceFlag = TRUE;
            return; /* return; keep distance, dont compare data, Possible loss of distance*/
        }

        for(i=0; i<OBJ_NUM; i++)
        {
            bEPDObsDistance[i] = CmpData.EPD[CmpData.bCurBfNumber].bObsDistance[i];
        }
        k = 0;
        for(i=0;i<CmpData.CCD.bDataNum;i++)
        {
            for(j=0;j<CmpData.EPD[CmpData.bCurBfNumber].bDataNum;j++)
            {
                if(bEPDObsDistance[j] != NO_OBJ)
                {
                    /* Compute the difference */
                    if(CmpData.CCD.bObsDistance[i] > bEPDObsDistance[j])
                    {
                        DistanceDiff = CmpData.CCD.bObsDistance[i] - bEPDObsDistance[j];
                    } else {
                        DistanceDiff = bEPDObsDistance[j] - CmpData.CCD.bObsDistance[i];
                    }
                    if(DistanceDiff < COMPARE_DIFFERENCE)
                    {
                        /* The data is valid. */
                        bObsDistance[k] = bEPDObsDistance[j];
                        bEPDObsDistance[j] = NO_OBJ;
                        k++;
                    }
                }
            }
        }

        /* Save the data to buff. */
        for(i=0;i<k;i++)
        {
            CmpData.CCD.bObsDistance[i] = bObsDistance[i];
        }
        CmpData.CCD.bDataNum = k;

        if(CmpData.CCD.bDataNum == 0 &&
           CmpData.CCD.wRingTime < (NearestData.wRingTime + RING_TIME_CHANGE_THRESHOLD))
        {
            /* Change to next sensor. */
            DetInfo.bDetTimes = 0;
        }
    }

#ifdef RINGTIME_RENEW
    static BYTE  xdata RingTimeCnt = 0;
    static DWORD xdata RingTimeBuffer = 0;

    if(RingTimeCnt < 16)
    {
        RingTimeCnt++;
        RingTimeBuffer += CmpData.EPD[CmpData.bCurBfNumber].wRingTime;
    }
    else
    {
        RingTimeBuffer >>= 4;
        if(RingTimeBuffer < NearestData.wRingTime)
            NearestData.wRingTime  = RingTimeBuffer;
        RingTimeBuffer = 0;
        RingTimeCnt = 0;
    }
#endif

}
