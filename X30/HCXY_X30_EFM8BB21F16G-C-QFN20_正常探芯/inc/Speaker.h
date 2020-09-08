#ifndef		_SPEAKER_H_
#define		_SPEAKER_H_

#include	"Cpu.h"

//#define		SPEAKER_USED

#define		SPEAKER_PORT			P1_B2
#define		SPEAKER_PORT_DIR_OUT	{P1MDOUT |=0x04;}

#define		SPEAKER_TYPE_NONE	0
#define		SPEAKER_TYPE_CONTIN	1
#define		SPEAKER_TYPE_4HZ	2
#define		SPEAKER_TYPE_2HZ	3

#define		SPEAKER_PERIOD_2HZ	250
#define		SPEAKER_PERIOD_4HZ	125
#define		SPEAKER_PERIOD_0HZ	0

#define     SPEAKER_MAX_DISTANCE_2HZ  	150
#define     SPEAKER_MAX_DISTANCE_4HZ  	100
#define     SPEAKER_MAX_DISTANCE_CONTIN 40

typedef		struct
{
	BYTE	WarningType;
	WORD	WarningPeriod;
	BOOL	bContinous;
	WORD	Counter;
}stSpeaker;

void	Spk_InitHardware (void);
void	Spk_InitVariable (void);
void	Spk_DriveTheSpeaker (void);

extern		stSpeaker xdata Speaker;

#endif
