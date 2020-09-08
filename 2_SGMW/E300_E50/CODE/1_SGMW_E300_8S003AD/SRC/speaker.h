/* 
 * File:   speaker.h
 * Author: Xianghong. Lai
 *
 * Created on 2014.12.22, 
 */

#ifndef SPEAKER_H
#define	SPEAKER_H

#include "cpu.h"
#include "Serial.h"

#define	SPEAKER_DRIVER_PORT		ID_TX
#define	SPEAKER_DRIVER_PORT_HIGH	(SPEAKER_DRIVER_PORT = V_HIGH)
#define	SPEAKER_DRIVER_PORT_LOW		(SPEAKER_DRIVER_PORT = V_LOW)
#define	START_THE_SPEAKER               (SPEAKER_DRIVER_PORT_HIGH)
#define	STOP_THE_SPEAKER                (SPEAKER_DRIVER_PORT_LOW)


/*********************************************************************************************************************
** Define the enum constant.
*********************************************************************************************************************/
#define	SPEAKER_DELAY_TIME_WHEN_OBSTACLE_DISPEAR  2//1000 - 2	/* Unit: ms */
/*the standard of Ford*/
#define SPEAKER_SOUND_HIGH_NO_OBJ	0
#define SPEAKER_SOUND_LOW_NO_OBJ	200
#define SPEAKER_SOUND_HIGH_LONG_ALARM 	200
#define SPEAKER_SOUND_LOW_LONG_ALARM	0
#define	SPEAKER_SOUND_HIGH_2		100
#define SPEAKER_SOUND_LOW_2		100
#define	SPEAKER_SOUND_HIGH_3		100
#define SPEAKER_SOUND_LOW_3		200
#define	SPEAKER_SOUND_HIGH_4		100
#define SPEAKER_SOUND_LOW_4		300

#define	SOUND_CONTINEOUS		40	/* Unit: cm */
#define	SOUND_2_DISTANCE		60	/* Unit: cm */
#define	SOUND_3_DISTANCE		100	/* Unit: cm */
#define	SOUND_4_DISTANCE		150	/* Unit: cm */

#define SPK_POSTION_FRONT               0
#define SPK_POSITON_REAR                1

/*********************************************************************************************************************
** Define the macro function.
*********************************************************************************************************************/

/*********************************************************************************************************************
** Define the struct.
*********************************************************************************************************************/
typedef struct
{
    BYTE	bSpkVolFlag;	/* The speak driving signal voltage level is HIGH or LOW. */
    BYTE	bSoundDistance;	/* Distance now the speaker sound. */
    WORD	wSpkSoundTime;	/* The time speaker sound. */
    WORD	wSpkHighTime;	/* Drive the speaker voltage level is high. */
    WORD	wSpkLowTime;	/* Drive the speaker voltage level is low. */
    BYTE        bSpkPosition;
} StSpkInfo;
/*********************************************************************************************************************
** Declare the global variable.
*********************************************************************************************************************/
extern volatile StSpkInfo SpkInfo;

/*********************************************************************************************************************
** Declare the global function.
*********************************************************************************************************************/
void Spk_InitTheVariable(void);
void Spk_DrvTheSpeaker(void);
void Spk_JudgeThePowerUpChimeType(void);
void Spk_PowerUpChime(void);
void Spk_InitThePWMModule(void);
void DONT_SPEAKER(void);


#endif	/* SPEAKER_H */

