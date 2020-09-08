#include "DetectObj.h"
#include "speaker.h"
#include "Command.h"
volatile StSpkInfo SpkInfo;
void Spk_InitTheVariable(void)
{
    SpkInfo.bSpkVolFlag = V_LOW;		/* Initial the speaker driver signal LOW. */
    SpkInfo.bSoundDistance = NO_OBJ;		/* There is no object to chime when power up. */
    SpkInfo.wSoundLastDistanceFreq = SPEAKER_SOUND_LOW_NO_OBJ;
    SpkInfo.wSpkSoundTime = 0;

    SpkInfo.wSpkHighTime = SPEAKER_SOUND_HIGH_2HZ;	/*       */
    SpkInfo.wSpkLowTime = SPEAKER_SOUND_LOW_2HZ;	/*  */
    SpkInfo.bSpkContinuousFlag = FALSE;			/*  */
    SpkInfo.bSpkPosition = SPK_POSITON_REAR;
}

void Spk_InitThePWMModule(void)
{
    STOP_THE_SPEAKER;
}

void Spk_DrvTheSpeaker(void)
{
    BYTE bNoDistanceFlag,bLongAlarmNeedFlag;
    WORD wSoundDistanceFreq;

    bNoDistanceFlag = FALSE;
    bLongAlarmNeedFlag = FALSE;
    wSoundDistanceFreq = SPEAKER_SOUND_LOW_NO_OBJ;

    SpkInfo.bSoundDistance = NearestData.bNearestDistance;
 /*  if(NearestData.bNearestChannel == SN_LRS  || NearestData.bNearestChannel == SN_RRS)
    {
        if(SpkInfo.bSoundDistance > 40 && SpkInfo.bSoundDistance <= 60)
        {
            SpkInfo.bSoundDistance += 61;
        }
    } */
    if(SpkInfo.bSoundDistance > MAX_DETECT_DISTANCE)
    {
        /* Dosn't sound. */
	SpkInfo.bSoundDistance = NO_OBJ;
	SpkInfo.wSpkHighTime = SPEAKER_SOUND_HIGH_NO_OBJ;
	SpkInfo.wSpkLowTime = SPEAKER_SOUND_LOW_NO_OBJ;
	bNoDistanceFlag = TRUE;
	SpkInfo.bSpkContinuousFlag = FALSE;
    }
    else
    {
        if(SpkInfo.bSoundDistance > SOUND_4HZ_DISTANCE)
        {
            /* Sound 2Hz */
            SpkInfo.wSpkHighTime = SPEAKER_SOUND_HIGH_2HZ;
            SpkInfo.wSpkLowTime = SPEAKER_SOUND_LOW_2HZ;
            SpkInfo.bSpkContinuousFlag = FALSE;
	}
        else if(SpkInfo.bSoundDistance > SOUND_CONTINEOUS)
        {
            SpkInfo.wSpkHighTime = SPEAKER_SOUND_HIGH_4HZ;
            SpkInfo.wSpkLowTime = SPEAKER_SOUND_LOW_4HZ;
            SpkInfo.bSpkContinuousFlag = FALSE;
	} else
        {
            /* Sound continuous. */
            SpkInfo.wSpkHighTime = SPEAKER_SOUND_HIGH_LONG_ALARM;
            SpkInfo.wSpkLowTime = SPEAKER_SOUND_LOW_LONG_ALARM;
            bLongAlarmNeedFlag = TRUE;
            SpkInfo.bSpkContinuousFlag = TRUE;/*the Long_Alarm Flag*/
	}
//        if(SpkInfo.bSoundDistance > SOUND_5_DISTANCE)
//        {
//            SpkInfo.wSpkHighTime = SPEAKER_SOUND_HIGH_6;
//            SpkInfo.wSpkLowTime = SPEAKER_SOUND_LOW_6;
//            SpkInfo.bSpkContinuousFlag = FALSE;
//	}
//        else if(SpkInfo.bSoundDistance > SOUND_4_DISTANCE)
//        {
//            SpkInfo.wSpkHighTime = SPEAKER_SOUND_HIGH_5;
//            SpkInfo.wSpkLowTime = SPEAKER_SOUND_LOW_5;
//            SpkInfo.bSpkContinuousFlag = FALSE;
//	} 
//        else if(SpkInfo.bSoundDistance > SOUND_3_DISTANCE)
//        {
//            SpkInfo.wSpkHighTime = SPEAKER_SOUND_HIGH_4;
//            SpkInfo.wSpkLowTime = SPEAKER_SOUND_LOW_4;
//            SpkInfo.bSpkContinuousFlag = FALSE;
//	} 
//        else if(SpkInfo.bSoundDistance > SOUND_2_DISTANCE)
//        {
//            SpkInfo.wSpkHighTime = SPEAKER_SOUND_HIGH_3;
//            SpkInfo.wSpkLowTime = SPEAKER_SOUND_LOW_3;
//            SpkInfo.bSpkContinuousFlag = FALSE;
//	} 
//        else if(SpkInfo.bSoundDistance > SOUND_CONTINEOUS)
//        {
//            SpkInfo.wSpkHighTime = SPEAKER_SOUND_HIGH_2;
//            SpkInfo.wSpkLowTime = SPEAKER_SOUND_LOW_2;
//            SpkInfo.bSpkContinuousFlag = FALSE;
//	} else
//        {
//            /* Sound continuous. */
//            SpkInfo.wSpkHighTime = SPEAKER_SOUND_HIGH_LONG_ALARM;
//            SpkInfo.wSpkLowTime = SPEAKER_SOUND_LOW_LONG_ALARM;
//            bLongAlarmNeedFlag = TRUE;
//            SpkInfo.bSpkContinuousFlag = TRUE;/*the Long_Alarm Flag*/
//	}  
    }
    if(SpkInfo.wSpkSoundTime > 0)
    {
        SpkInfo.wSpkSoundTime--;
	if(SpkInfo.bSpkVolFlag == V_LOW)
	{
            wSoundDistanceFreq = SpkInfo.wSpkLowTime;/*Get the Low_time speak frequence.*/
        }
        else
        {
            wSoundDistanceFreq = SpkInfo.wSpkHighTime;/*Get the High_time speak frequence.*/
	}
        if(wSoundDistanceFreq != SpkInfo.wSoundLastDistanceFreq)
	{
            /*first come in the frequence is 500*/
            if( ((SpkInfo.wSoundLastDistanceFreq - SpkInfo.wSpkSoundTime) < (SpkInfo.wSoundLastDistanceFreq >> 1)) /*The Freq is change to 250*/
             && ((SpkInfo.wSoundLastDistanceFreq - SpkInfo.wSpkSoundTime) < wSoundDistanceFreq))
            {
		SpkInfo.wSpkSoundTime += (wSoundDistanceFreq - SpkInfo.wSpkSoundTime);
		SpkInfo.wSoundLastDistanceFreq = wSoundDistanceFreq;
            }
	}
   }else if(SpkInfo.wSpkSoundTime == 0)
        {
            if(bNoDistanceFlag == FALSE)
            {
		if(SpkInfo.bSpkVolFlag == V_LOW)
		{
                    SpkInfo.bSpkVolFlag = V_HIGH;
                    SpkInfo.wSpkSoundTime = SpkInfo.wSpkHighTime;
                    SpkInfo.wSoundLastDistanceFreq = SpkInfo.wSpkSoundTime;
                    START_THE_SPEAKER;	/*(SPEAKER_DRIVER_PORT_LOW,	SPEAKER_DRIVER_PORT_OUTPUT)*/
                    SPEAKER_DRIVER_PORT_HIGH;
		}
                else
                {
                    SpkInfo.bSpkVolFlag = V_LOW;
                    SpkInfo.wSpkSoundTime = SpkInfo.wSpkLowTime;
                    SpkInfo.wSoundLastDistanceFreq = SpkInfo.wSpkSoundTime;
                    if(bLongAlarmNeedFlag == FALSE)
                    {
			STOP_THE_SPEAKER;
                    }
		}
            }
            else
            {
                /* There is no obstacle */
		SpkInfo.bSpkVolFlag = V_LOW;
		SpkInfo.wSpkSoundTime = SpkInfo.wSpkLowTime;
		SpkInfo.wSoundLastDistanceFreq = SpkInfo.wSpkSoundTime;
		STOP_THE_SPEAKER;
            }
    }
}

void Spk_JudgeThePowerUpChimeType(void)
{
    BYTE i;
    SpkInfo.wSpkLowTime = 500;
    SpkInfo.wSpkHighTime = 500;

    for(i=0;i<SN_NUM;i++)
    {
        if( Cmd.bSelfCheckResult[i] != SELF_SENSOR_OK )
            DiagInfo.bErrSnNum++;
    }
    i = DiagInfo.bErrSnNum;
    DiagInfo.wPowerUpChimeTime = 0;//1000 - 3;

    if(0 == DiagInfo.bErrSnNum)
    {
        DiagInfo.wPowerUpChimeTime = 500-3;
    }
    else if(Cmd.bSelfCheckResult[ID_RL] != SELF_SENSOR_OK)      //RL is master sensor
    {
        DiagInfo.wPowerUpChimeTime = 0;
    }
    else if(Cmd.bSelfCheckResult[ID_RR] != SELF_SENSOR_OK)
    {
        DiagInfo.wPowerUpChimeTime = 3000-3;
    }
    SPEAKER_DRIVER_PORT_HIGH;
    START_THE_SPEAKER;
    SpkInfo.bSpkVolFlag = V_HIGH;
    SpkInfo.wSpkSoundTime = SpkInfo.wSpkHighTime;
    
}

void Spk_PowerUpChime(void)
{
    if(SpkInfo.wSpkSoundTime > 0)												/* ?????????start? */
    {
	SpkInfo.wSpkSoundTime--;													/* ????????w??*/
    }
    if(SpkInfo.wSpkSoundTime == 0)
    {
	/* Judge if now are sounding. */
        if(SpkInfo.bSpkVolFlag == V_LOW)
	{
            /* Output the LOW voltage, and need to change HIGH voltage. */
            SPEAKER_DRIVER_PORT_HIGH;
            SpkInfo.bSpkVolFlag = V_HIGH;
            SpkInfo.wSpkSoundTime = 500;
	}
        else
        {
            /* Output the HIGH voltage, and need to change LOW voltage. */
            SPEAKER_DRIVER_PORT_HIGH;												/* ??????c???????Y? */
            SpkInfo.bSpkVolFlag = V_HIGH;										/* ??????c???????? */
            SpkInfo.wSpkSoundTime = 500;		/* ??????c?????*/
	}
    }
}

void DONT_SPEAKER(void)
{
  SpkInfo.bSoundDistance = NO_OBJ;
  SpkInfo.wSpkHighTime = SPEAKER_SOUND_HIGH_NO_OBJ;
  SpkInfo.wSpkLowTime = SPEAKER_SOUND_LOW_NO_OBJ;
  SpkInfo.bSpkContinuousFlag = FALSE;
  SPEAKER_DRIVER_PORT_LOW;
}
