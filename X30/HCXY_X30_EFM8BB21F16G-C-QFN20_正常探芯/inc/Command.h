#ifndef _COMMAND_H_
#define	_COMMAND_H_

#define TEST_CODE_VEHICLE    TRUE
#define TEST_CODE_FIXED_ID   TRUE
#define TEST_CODE_DETDATA    TRUE

#define START_UART_DELAY	10
#define		SENSOR_NUMBER_3
//#define		SENSOR_NUMBER_5

#define		MASTER_RX_TIME_OVER_MAX		10
#define		SINGLE_SENSOR_TIME_OVER_MAX	80//120 //standard values is 80ms
#define		MASTER_WORK_TIME_MAX		(130)
#define		MASTER_TXANDRX_TIME_OVER_MAX	(20+MASTER_RX_TIME_OVER_MAX)

#define  TIMEOUT_THRE_SENSOR    75      /*Unit:ms*/
#define  TIMEOUT_THRE_CAR       32      /*Unit:ms*/
#define  SENSOR_STOP_SCAN_TIME  100     /*Unit:ms*/
#define  SENSOR_STOP_SCAN_REC_TIME  (SENSOR_STOP_SCAN_TIME-20)     /*Unit:ms*/


#define  	SENSOR_EER_THRE_SENSOR 10      /*Unit:cnt*/
#define  	SENSOR_EER_THRE_CAR    30      /*Unit:cnt*/


#define CMD_REQUEST_DISTANCE		  	0x00
#define CMD_REQUEST_RINGTIME		  	0x01
#define CMD_REQUEST_ECHO_WIDTH		  	0x02
#define CMD_REQUEST_ECHO_VOL		  	0x03
#define CMD_REQUEST_SOFTWARE_VERSION  	0x04
#define CMD_REQUEST_HARDWARE_VERSION  	0x05
#define CMD_REQUEST_SENSOR_TYPE		 	0x06
#define CMD_REQUEST_THEORY_EEPROM_THRE	0x07
#define CMD_REQUEST_SAVE_EEPROM  		0x08
#define CMD_REQUEST_EEPROM_THRE		 	0x09
#define CMD_REQUEST_ADD_DRIVE_NUMBER  	0x0A
#define CMD_REQUEST_DEC_DRIVE_NUMBER  	0x0B
#define CMD_REQUEST_SAVE_DRIVE_NUMBER  	0x0C
#define CMD_REQUEST_RESERVED_1	  		0x0D
#define CMD_REQUEST_RESERVED_2  		0x0E
#define CMD_REQUEST_RESERVED_3  		0x0F

#define S1   	P1_B0
#define S2   	P0_B7
#define SW_EN	{P1_B1 = 0;}
#define SW_DIS	{P1_B1 = 1;}

#define SW_SET_RL_INPUT     {S2 = 0;S1 = 0;}  //RL  -- SEN2
#define SW_SET_RML_INPUT    {S2 = 0;S1 = 1;}  //RML -- SEN3
#define SW_SET_RMR_INPUT    {S2 = 1;S1 = 1;}  //RMR -- SEN1
#define SW_SET_RR_INPUT     {S2 = 1;S1 = 0;}  //RR  -- SEN4

#define ID_MASTER ID_RML

typedef enum
{
	ID_RL 	= 0,	//SEN 2
	ID_RML 	= 1,	//SEN 3
	ID_RR 	= 2,	//SEN 4
	ID_SUM  = 3,

}E_ID;

typedef enum
{
	MST_DRV_SENSOR_SELFCHECK 			= 0,
	MST_WAIT_SELFCHECK,
	MST_SEND_SELF_TO_BCM,
	MST_DRV_SENSOR,
	MST_WAIT_SENSOR_DATA,
	MST_SEND_DISTANCE_TO_BCM,
	MST_WAIT_VECHILE_CFG,
	CMD_STOP,
	SLV_WAIT_CMD,
	SLV_DISTANCE_BACK ,
	SLV_CALIBRATION	,
	SLV_CALIBRATION_BACK,

}E_CMD;


typedef		struct
{
//	E_ID    bID;		/* Used as a temporary receive cache and return value comparison*/
	WORD	wWaitTime;
	WORD	wBackData;
	E_CMD   CmdStatus;
	BYTE    CmdCalibration;
	BYTE	bOverCnt[ID_SUM];
	BYTE    bSensorErrCnt[ID_SUM];
	BYTE    bSensorRun[ID_SUM];
	BYTE    bUsedSnrNUM;
	BYTE    bVechInfoErrCnt;
	BYTE    bDistance[ID_SUM];
	BYTE    bObsorient[ID_SUM];
	BYTE    bBuzzerInfo;
}stCmd;

extern void Cmd_InitVariable(void);
extern void Cmd_ReadID(void);
extern void Cmd_GetVechileMode(void);
extern BYTE Cmd_ReadLinData(void);
extern void Cmd_Process(void);
extern void Cmd_SaveData(void);
extern void Cmd_ReadOverChannel(void);
extern void Cmd_UpdataDistance(void);
extern void Cmd_SelfCheckUpdataDistance(void);
extern void Cmd_DistancetoComInfo(void);
extern void Cmd_NextSensorSelf(void);
extern void Cmd_NextSensorDet(BYTE ForceCutFlag);
extern void Cmd_ClearBuffer(void);

extern void Cmd_SendReqVechiModetoBCM(void);
extern void Cmd_SendSelfChecktoBCM(void);
extern void Cmd_SendDistancetoBCM(void);
extern BYTE Cmd_VechileCfgCheck(void);
extern BYTE Cmd_RenewVechileCfg(void);
extern void Cmd_SetChannel(void);
extern stCmd xdata Cmd;


#endif
