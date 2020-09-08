#include "DetectObj.h"
#include "speaker.h"
#include "Command.h"
volatile StSpkInfo SpkInfo;
void Spk_InitTheVariable(void)
{
    SpkInfo.bSpkVolFlag = V_LOW;		/* Initial the speaker driver signal LOW. */
    SpkInfo.bSoundDistance = NO_OBJ-1;		/* There is no object to chime when power up. */
    SpkInfo.wSpkSoundTime = 0;

    SpkInfo.wSpkHighTime = SPEAKER_SOUND_HIGH_NO_OBJ;
    SpkInfo.wSpkLowTime = SPEAKER_SOUND_LOW_NO_OBJ;
    SpkInfo.bSpkPosition = SPK_POSITON_REAR;
}

void Spk_InitThePWMModule(void)
{
    STOP_THE_SPEAKER;
}

void Spk_DrvTheSpeaker(void)
{
    static BYTE bNoDistanceFlag = TRUE;
    static BYTE bLongAlarmNeedFlag = TRUE;

    if(SpkInfo.bSoundDistance != NearestData.bNearestDistance)
    {
        SpkInfo.bSoundDistance = NearestData.bNearestDistance;
        bNoDistanceFlag = FALSE;
        bLongAlarmNeedFlag = FALSE;
        if(SpkInfo.bSoundDistance > SOUND_4_DISTANCE)
        {
            /* Dosn't sound. */
            SpkInfo.bSoundDistance = NO_OBJ;
            SpkInfo.wSpkHighTime = SPEAKER_SOUND_HIGH_NO_OBJ;
            SpkInfo.wSpkLowTime = SPEAKER_SOUND_LOW_NO_OBJ;
            bNoDistanceFlag = TRUE;
        }
        else
        {
            if(SpkInfo.bSoundDistance > SOUND_3_DISTANCE)
            {
                SpkInfo.wSpkHighTime = SPEAKER_SOUND_HIGH_4;
                SpkInfo.wSpkLowTime = SPEAKER_SOUND_LOW_4;
            } 
            else if(SpkInfo.bSoundDistance > SOUND_2_DISTANCE)
            {
                SpkInfo.wSpkHighTime = SPEAKER_SOUND_HIGH_3;
                SpkInfo.wSpkLowTime = SPEAKER_SOUND_LOW_3;
            } 
            else if(SpkInfo.bSoundDistance > SOUND_CONTINEOUS)
            {
                SpkInfo.wSpkHighTime = SPEAKER_SOUND_HIGH_2;
                SpkInfo.wSpkLowTime = SPEAKER_SOUND_LOW_2;
            } else
            {
                /* Sound continuous. */
                SpkInfo.wSpkHighTime = SPEAKER_SOUND_HIGH_LONG_ALARM;
                SpkInfo.wSpkLowTime = SPEAKER_SOUND_LOW_LONG_ALARM;
                bLongAlarmNeedFlag = TRUE;
            }
        }
    }
    
    if(SpkInfo.wSpkSoundTime > 0)
    {
        SpkInfo.wSpkSoundTime--;
    }
    else if(SpkInfo.wSpkSoundTime == 0)
    {
        if(bNoDistanceFlag == FALSE)
        {
            if(SpkInfo.bSpkVolFlag == V_LOW)
            {
                SpkInfo.bSpkVolFlag = V_HIGH;
                SpkInfo.wSpkSoundTime = SpkInfo.wSpkHighTime;
                START_THE_SPEAKER;
            }
            else
            {
                SpkInfo.bSpkVolFlag = V_LOW;
                SpkInfo.wSpkSoundTime = SpkInfo.wSpkLowTime;
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

      if(i == 0)
        DiagInfo.wPowerUpChimeTime = 500-3;
      else if(i == 1)
        DiagInfo.wPowerUpChimeTime = 2000-3;
      else 
        DiagInfo.wPowerUpChimeTime = 3000-3;

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
            START_THE_SPEAKER;
            SpkInfo.bSpkVolFlag = V_HIGH;
            SpkInfo.wSpkSoundTime = SpkInfo.wSpkHighTime;
	}
        else
        {
            /* Output the HIGH voltage, and need to change LOW voltage. */
            STOP_THE_SPEAKER;												/* ??????c???????Y? */
            SpkInfo.bSpkVolFlag = V_LOW;										/* ??????c???????? */
            SpkInfo.wSpkSoundTime = SpkInfo.wSpkLowTime;		/* ??????c?????*/
	}
    }
}

void DONT_SPEAKER(void)
{
  SpkInfo.bSoundDistance = NO_OBJ;
  SpkInfo.wSpkHighTime = SPEAKER_SOUND_HIGH_NO_OBJ;
  SpkInfo.wSpkLowTime = SPEAKER_SOUND_LOW_NO_OBJ;
  SPEAKER_DRIVER_PORT_LOW;
}
