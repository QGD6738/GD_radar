#ifndef _DETLINCFG_H_
#define _DETLINCFG_H_

#include "Cpu.h"


#define LIN_NO_OBJ					0xFF
#define LIN_VEHICLE_VALID	    	(WORD)((DWORD)15 * 1776/100)	// 1/0.0563 = 17.76

//TX
#define LIN_NETWORK_OK				0
#define LIN_NETWORK_ERR				1

#define LIN_RPAS_OFF				0
#define LIN_RPAS_ON					1

#define LIN_SPEAKER_OFF				0
#define LIN_SPEAKER_LONG_BEEP		1
#define LIN_SPEAKER_2HZ				2
#define LIN_SPEAKER_4HZ				3
#define LIN_SPEAKER_RESERVED		4

#define LIN_WORK_FRE_750			0
#define LIN_WORK_FRE_1000			1
#define LIN_WORK_FRE_1500			2
#define LIN_WORK_FRE_RESERVED		3

#define LIN_SENSOR_OK				0
#define LIN_SENSOR_ERR				1

#define LIN_POWER_NORMOL			0
#define LIN_POWER_ERR				1

//RX
#define LIN_REVERSGEAR_NO_PRESS		0
#define LIN_REVERSGEAR_PRESS		1
#define LIN_REVERSGEAR_RESERVED		2

#define LIN_SYS_POWER_OFF			0
#define LIN_SYS_POWER_ACC			1
#define LIN_SYS_POWER_ON			2
#define LIN_SYS_POWER_CRANK			3

#define LIN_TRAILER_OFF				0
#define LIN_TRAILER_ON				1

#define LIN_REAR_LIB_CLOSE			0
#define LIN_REAR_LIB_OPEN			1



/*******************************************************************************
****************************Tx Signals******************************************
*******************************************************************************/
typedef struct
{
	BYTE ResponseError;
	BYTE RPAStaus;
	BYTE RPASpeakerInfo;
	BYTE RPAworkfrq;
	BYTE RPADetectionROL;
	BYTE RPADetectionRML;
	BYTE RPADetectionRMR;
	BYTE RPADetectionROR;
	BYTE RORErrorFlag;
	BYTE RMRErrorFlag;
	BYTE RMLErrorFlag;
	BYTE ROLErrorFlag;
	BYTE PowerLowVoltage_RPAS;
	BYTE PowerHighVoltage_RPAS;
} StLinTxSig;


/*******************************************************************************
****************************Rx Signals******************************************
*******************************************************************************/
typedef struct
{
	BYTE AmbientTemp;
	WORD VehicleSpeed;
	BYTE SystemPowerMod;
	BYTE ReversGearSts;
	BYTE TrailerSts;
	BYTE RearLidsts;
} StLinRxSig;


extern StLinTxSig 		xdata 	LinTxSig;
extern StLinRxSig 		xdata 	LinRxSig;

#endif


