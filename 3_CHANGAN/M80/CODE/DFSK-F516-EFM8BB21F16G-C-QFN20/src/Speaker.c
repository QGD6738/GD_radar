#include	"Cpu.h"
#include	"Speaker.h"
#include	"DetectObj.h"
stSpeaker	xdata Speaker;

void	Spk_InitHardware (void)
{
	SPEAKER_PORT_DIR_OUT;
	SPEAKER_PORT = V_HIGH;
}

void	Spk_InitVariable (void)
{
	Speaker.WarningType = SPEAKER_TYPE_NONE;
	Speaker.WarningPeriod = SPEAKER_PERIOD_0HZ;
	Speaker.bContinous = TRUE;
	Speaker.Counter = 0;
}

void	Spk_DriveTheSpeaker (void)
{
	BYTE WarningClass = NearestData.bNearestDistance;

	if(WarningClass > SPEAKER_MAX_DISTANCE_2HZ)
	{
		WarningClass = SPEAKER_TYPE_NONE;
	}
	else if(WarningClass > SPEAKER_MAX_DISTANCE_4HZ)
	{
		WarningClass = SPEAKER_TYPE_2HZ;
	}
	else if(WarningClass > SPEAKER_MAX_DISTANCE_CONTIN)
	{
		WarningClass = SPEAKER_TYPE_4HZ;
	}
	else
	{
		WarningClass = SPEAKER_TYPE_CONTIN;
	}

	Speaker.Counter ++;
	if(WarningClass != Speaker.WarningType &&
	   Speaker.Counter >= Speaker.WarningPeriod)
	{
		if(WarningClass == SPEAKER_TYPE_NONE)
		{
			Speaker.Counter = 0;
			Speaker.WarningPeriod = SPEAKER_PERIOD_0HZ;
			Speaker.bContinous = TRUE;
			SPEAKER_PORT = V_HIGH;
		}
		else if(WarningClass == SPEAKER_TYPE_2HZ)
		{
			Speaker.Counter = 0;
			Speaker.WarningPeriod = SPEAKER_PERIOD_2HZ;
			Speaker.bContinous = FALSE;
		}
		else if(WarningClass == SPEAKER_TYPE_4HZ)
		{
			Speaker.Counter = 0;
			Speaker.WarningPeriod = SPEAKER_PERIOD_4HZ;
			Speaker.bContinous = FALSE;
		}
		else if(WarningClass == SPEAKER_TYPE_CONTIN)
		{
			Speaker.Counter = 0;
			Speaker.WarningPeriod = SPEAKER_PERIOD_0HZ;
			Speaker.bContinous = TRUE;
			SPEAKER_PORT = V_LOW;
		}
		Speaker.WarningType = WarningClass;
	}
	if(!Speaker.bContinous)
	{
		if(Speaker.Counter >= Speaker.WarningPeriod)
		{
 			SPEAKER_PORT = !SPEAKER_PORT;
			Speaker.Counter = 0;
		}
	}

}
