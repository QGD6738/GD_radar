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
#define SPEAKER_SOUND_LOW_NO_OBJ	500

#define	SPEAKER_SOUND_HIGH_2HZ		65	/* Unit: 260ms */
#define SPEAKER_SOUND_LOW_2HZ		195

#define	SPEAKER_SOUND_HIGH_4HZ		65	/* Unit: 162ms */
#define SPEAKER_SOUND_LOW_4HZ		97

//#define	SPEAKER_SOUND_HIGH_2		50	/* Unit: 162ms */
//#define SPEAKER_SOUND_LOW_2		50
//#define	SPEAKER_SOUND_HIGH_3		50	/* Unit: 162ms */
//#define SPEAKER_SOUND_LOW_3		150
//#define	SPEAKER_SOUND_HIGH_4		50	/* Unit: 162ms */
//#define SPEAKER_SOUND_LOW_4		350
//#define	SPEAKER_SOUND_HIGH_5		50	/* Unit: 162ms */
//#define SPEAKER_SOUND_LOW_5		550
//#define	SPEAKER_SOUND_HIGH_6		50	/* Unit: 162ms */
//#define SPEAKER_SOUND_LOW_6		750

//#define SPEAKER_SOUND_2HZ		500
#define SPEAKER_SOUND_HIGH_LONG_ALARM 	250
#define SPEAKER_SOUND_LOW_LONG_ALARM	0

#define	SOUND_CONTINEOUS		40	/* Unit: cm */
#define	SOUND_4HZ_DISTANCE		100	/* Unit: cm */
#define	SOUND_2HZ_DISTANCE		150	/* Unit: cm */


#define	SOUND_CONTINEOUS		40	/* Unit: cm */
#define	SOUND_2_DISTANCE		60	/* Unit: cm */
#define	SOUND_3_DISTANCE		80	/* Unit: cm */
#define	SOUND_4_DISTANCE		100	/* Unit: cm */
#define	SOUND_5_DISTANCE		120	/* Unit: cm */
#define	SOUND_6_DISTANCE		150	/* Unit: cm */



#define SPK_POSTION_FRONT       0
#define SPK_POSITON_REAR        1
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
    WORD	wSoundLastDistanceFreq;	/* Distance now the speaker sound frequence. */
    BYTE	bSpkContinuousFlag;	/* When the obstacle less than 30cm, the flag is TRUE untill the distance more than 60cm.*/
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

