#ifndef DETECTOBJ_H
#define	DETECTOBJ_H

#include "cpu.h"
#include "ADC.h"
#include "ReadID.h"
/* eeprom */
#define EEPROM
#ifdef  EEPROM
  #define  DEFAULT_RINGTIME                             1300   /* Uint:us */
  #define  DEFAULT_SAMIP_VOL                            0
#endif

/* Obstacle parameter*/
//#define RINGTIME_RENEW
#define	NO_OBJ					        0xFF    /* There is no obstacle. */
#define	PULE_NUM			  	        20      /* Send the ultrasonic pulse number. */
#define	OBJ_NUM					        10	/* Save the obstacle used for comparing */
#define DET_CMP_TIMES                                   2       /* Compare times */
#define	DETECT_MAX_SAMPLING     		        2       /* Sampling time */
#define	DETECT_MAX_DISTANCE_120CM		        9	/* Wait 9ms to detect 120cm obstacle. */
#define	DETECT_MAX_DISTANCE_150CM		        10	/* Wait 10ms to detect 150cm obstacle. */
#define	DETECT_MAX_NOISE_TIME			        3	/* Unit: ms. */
#define SN_ERR_THRESHOLD                                2       /* Uint:sampling time */

#define	DETECT_DELAY_UPDATE_HIGH		        1200	/* Unit: ms */
#define DETECT_DELAY_UPDATE_MID			        800     /* Unit: ms */
#define DETECT_DELAY_UPDATE_LOW			        500     /* Unit: ms */
#define	OBSTACLE_MOVE_FAR_AWAY              	        7	/* Unit: cm */
#define	OBSTACLE_MOVE_CLOSE                 	        5	/* Unit: cm */

#define	OBSTACLE_IN_RING_TIME_VALUE		        6	/* Unit: cm */
#define	OBSTACLE_IN_CONTINUOUS_DISTANCE_DELAY	        5       /* Unit: cm */
#define	OBSTACLE_NOT_IN_CONTINUOUS_DISTANCE_DELAY       3       /* Unit: cm */
#define	OBSTACLE_NOT_IN_CONTINUOUS_WITHIN_60CM	        4       /* Unit: cm */
#define OBSTACLE_IN_CONTINUOUS			        2       /* Unit: cm */

#define	OBSTALE_IN_RING_TIME_THRESHOLD      		350	/* Unit: us */
#define	RING_TIME_CHANGE_THRESHOLD			300	/* Unit: us */
#define	RENEW_RING_TIME_THRESHOLD			120	/* Unit: us */
#define	COMPARE_DIFFERENCE				8	/* Unit: cm */

#define	RING_TIME_MAX_VALUE				2700    /* Unit: us */
#define RING_TIME_MID_VALUE				2350    /* Unit: us */
#define	RING_TIME_MIN_VALUE				500     /* Unit: us */
#define	RING_TIME_MAX_VALUE_WITH_OBSTACLE		((DETECT_MAX_DISTANCE_150CM-1) * 2000)	/* Unit: us */

#define	MAX_DETECT_DISTANCE			        150	/* Unit: cm */
#define MID_DETECT_DISTANCE			        100     /* Unit: cm */
#define	CONTINUOUS_CHIME_DISTANCE           	        40	/* Unit: cm */

#define	SN_DISTANCE_RL                      		150	/* Unit: cm */
#define SN_DISTANCE_RML                     		150     /* Unit: cm */
#define	SN_DISTANCE_RMR                     		150	/* Unit: cm */
#define	SN_DISTANCE_RR                      		150	/* Unit: cm */
#define	SN_DISTANCE_FL                      		150	/* Unit: cm */
#define	SN_DISTANCE_FR                      		150	/* Unit: cm */


/* Obstacle ad parameter*/
#define OBS_ECHO_WIDTH_COMPEN
#define COUNT_TIME_UNIT                                 (35)    /* Uint:us */
#define DETECT_NOISE_BASE_VOL                           6       /* Ad value */        
#define DETECT_NOISE_MID_VOL                            30      /* Ad value */
#define DETECT_NOISE_MAX_VOL                            128     /* Ad value */
#define DETECT_CMP_BUFFER_LENGTH                        2
#define DETECT_BUFFER_LENGTH                   	        320
#define OBSTACLE_START_VOL                              4       /* Ad value */
#define OBSTACLE_EXIT_VOL_SHIFT                         2       /* Uint:shift bit */
#define DETECT_NOISE_WIDTH				120	/* unit:us*/

/* Filter */
#define START_VALUE_POLISHING
#define SINGLE_POINT_JUMP_ZERO_FILTER
#define PEAK_DISTURB_FILTER    
#ifdef  START_VALUE_POLISHING
    #define START_VALUE_POLISHING_LEN                   8
#endif
#ifdef  PEAK_DISTURB_FILTER
    #define PEAK_DISTURB_FILTER_HIGH                    32      /* Unit:AD amplitude value*/
    #define PEAK_DISTURB_FILTER_WIDTH                   300     /* Unit:us */
#endif




/*********************************************************************************************************************
** Declear the enum constant.
*********************************************************************************************************************/
typedef enum EtDetState
{
    WAIT_DET_CMD = 0,
    DET_OBJ_DETECT_INIT,
    DET_OBJ_NOISE_SAMPLING,
    DET_OBJ_WAIT_SAMPLING,
    DET_OBJ_START_DETECT_OBSTACLE,
    DET_OBJ_WAIT_ECHO,
    DET_OBJ_HANDLE_NOISE,
    DET_OBJ_GENERATE_RETURN,
} EtDetState;

typedef enum EtOBS_TYPE
{
    OBS_BLUE = 0,
    OBS_YELLOW,
    OBS_ORANGE,
    OBS_RED,
}EtOBS_TYPE;

typedef struct
{
    EtDetState  bDetState;	/* It denote the state of the system. */
    BYTE  bID;
    BYTE  bDetTriggerFlag;      /* Recever detrct cmd */
    BYTE  bDetSnNum;		/* The sensor number of sending wave. */
    BYTE  bDetTimes;		/* Remain times to send ultrasonic. */
    BYTE  bDetWaitTime;		/* Time for waiting echo. */
    BYTE  bDetNoiseDelayTime;	/* Detect delay time to change compare times. */
    WORD  wDetRealTime;         /* Detect real time*/
} StDetInfo;

typedef struct
{
    BYTE  bErrSnNum;		/* It is used to denote the error sensor number. */
    WORD  wPowerUpChimeTime;	/* Power up chime time to denote if the system is ok or not.*/
    BYTE  bErrCounter;		/* It is used to count the error. */
    BYTE  bSelfCheckResult[SN_NUM];
} StDiagInfo;

typedef struct
{
    WORD  wStartTime;
    BYTE  bAdcBuffer[10];
    WORD  wRawDataIndex;
    BYTE  bRawDataBuffer[DETECT_BUFFER_LENGTH];  	/* sampling raw buffer */
} StEchoRawData;

typedef struct
{
    BYTE  bDetSampThre;
    BOOL  bObsSigStartFlag;
    
    //WORD  wObsSigStartCnt;
    //WORD  wObsSigOverCnt;
    WORD  wObsSigExitCnt;
    BYTE  bObsSigVolInto;
    BYTE  bObsSigVolExit;
    WORD  wObsSigTimeInto;
    WORD  wObsSigTimeWidth;
    BYTE  bObsSigVolHeight;
#ifdef  EEPROM
    BYTE  bSampSN;
#endif  
} StAnalysisData;

typedef struct
{
    BYTE  bDataValid;                 /* Data volid flag*/
    WORD  wDetTime;                   /* Detect time */ 
    WORD  wRingTime;                  /* Ring time */
    BYTE  bDataNum;		      /* Obstacle number */
    BYTE  bSampThre;
    BYTE  bObsDistance[OBJ_NUM];      /* Obstacle distance */
} StEchoPrcsData;

typedef struct
{
    BYTE  bEchoRaisedTimes;
    BYTE  bNoiseRaisedTimes;
    
    DWORD dObsIntegralArea;
    DWORD dNseIntegralArea;
    BOOL  bNoiseMuchFlag;
    EtOBS_TYPE  eObsNsType;           /* Obstacle noise type*/
} StNoise;

typedef struct
{
    BYTE  bFirstDataFlag;	/* It is used to denote if the data is the first received or the second. */
    BYTE  bCurBfNumber;		/* Buffer number */
    StEchoPrcsData CCD;         /* compare cache data */
    StEchoPrcsData EPD[DETECT_CMP_BUFFER_LENGTH];
    StNoise        Ns[DETECT_CMP_BUFFER_LENGTH];
} StCmpData;

typedef struct
{
    BYTE  bRingTimeFlag;		/* It is used to judge if the ring time has been detcted. */
    WORD  wUpdataDelay;
    BYTE  bNearestDistance;		/* The nearest obstacle. */
    BYTE  bEachNearestDistance[SN_NUM];	/* The nearest obstacle of each sensor detected. */
    WORD  wRingTime;
    BYTE  bRingTimeBounceFlag;
} StNearestData;

typedef struct
{
    WORD  wRingTime;
    WORD  wTimeWidth;
    BYTE  bVolHeight;
    BYTE  bDistance;
}StCalibrationData;


void Null (void);
void DetObj_RealTimeClock(void);
void DetObj_InitSelfCheckVar(void);
void DetObj_SelfCheck(void);
void DetObj_SendThe58KhzUltraSonic(void);
void DetObj_CompareTheData(void);
void DetObj_ProcessTheRingTime(void);
void DetObj_DiagnosticTheRingTime(void);
void DetObj_GenerateTheNearestObstacle(void);
void DetObj_DiagnosticTheSensor(void);
void DetObj_Start(void);
void DetObj_Stop(void);
void DetObj_InitTheCCD(void);
void DetObj_InitTheEPD(void);
void DetObj_ClearTheRawDataBuff(void);
void DetObj_ClearTheNoiseBuff(void);
BYTE GetDistance(WORD ObsTime,WORD EchoWidth,BYTE VolHeight);
void DetObj_HoldTheObstaclestate(void);
void DetObj_DetTheObj(void);
void DetObj_SendPulse(void);
void DetObj_RawFilter(void);
void DetObj_CalSampNoise(void);
void DetObj_RawEchoDataAns(void);
void DetObj_RawNoiseDataAns(void);
void DetObj_SaveDataIntoBuff(BYTE bPortState, WORD wTime);
void DetObj_CopySamplingBuffer(BYTE* SP, BYTE* EP, BYTE Len);

extern const BYTE	EachSNMaxDistance[SN_NUM];
extern StNearestData 	NearestData;
extern StDetInfo 	DetInfo;
extern StEchoRawData	RawData;
extern StAnalysisData   AnsData;
extern StCmpData 	CmpData;
extern StDiagInfo 	DiagInfo;
extern StCalibrationData ClrtData;
extern CALLBACK  	DetObj_DataPrcs;

#endif	/* DETECTOBJ_H */




