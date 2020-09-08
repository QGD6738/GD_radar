#ifndef COMMAND_H
#define COMMAND_H
#include "ReadID.h"

#define  TIMEOUT_THRE_SENSOR    160     /*Unit:ms*/
#define  TIMEOUT_THRE_CAR       32      /*Unit:ms*/
#define  SENSOR_STOP_SCAN_TIME  120     /*Unit:ms*/   

#define  SENSOR_EER_THRE_SENSOR 10      /*Unit:cnt*/
#define  SENSOR_EER_THRE_CAR    10      /*Unit:cnt*/
     
typedef enum 
{
  VEHICLE_TYPE_E100     = 0x01,
  VEHICLE_TYPE_E300_E50 = 0x02,
  VEHICLE_TYPE_N350P    = 0x03,
  VEHICLE_TYPE_CN110P   = 0x04,
  VEHICLE_TYPE_MDS201   = 0x05,
}EtVehicleType;

#define  VEHICLE_TYPE           VEHICLE_TYPE_N350P

typedef struct VECHILEINFOS
{
    BYTE SpeedValid : 1;
    BYTE ReverseGear : 1;
    BYTE HandbrakeOn : 1;
    BYTE KeyOn : 1;
    BYTE ReservedBit : 1;
    BYTE ChkSum : 1;
    BYTE Reserved : 2;
}VECHILEINFOS;

typedef union UNION_VECHILEINFO
{
     VECHILEINFOS VechicleInfo;
     BYTE         VechicleData;
}UNION_VECHILEINFO;
extern  UNION_VECHILEINFO VechInfo;

typedef enum 
{
  CMD_DRIVE_SENSOR_SELF = 0,  //master
  CMD_WAIT_SENSOR_SELF,
  CMD_SEND_SELF_DATA_TO_CAR,
  
  CMD_SEND_DISTANCE_TO_CAR,
  CMD_WAIT_VEHICLE_CONFIG,
  CMD_DRIVE_SENSOR_WORK,
  CMD_WAIT_SENSOR_DATA,
  CMD_STOP,
  CMD_WAIT_REQ,      //slave
  CMD_SET,           //3ms set cmd
}EtCmdSta;


typedef enum 
{
    REQ_DISTANCE        =   0,
    REQ_RINGTIME        =   1,
    REQ_ID_SELF_DATA    =   2,
    REQ_ECHO_WIDE       =   3,
    REQ_ECHO_HEIGHT     =   4,
    REQ_EEPROM_SAMP     =   5,
    REQ_HANDSHAKE       =   6,
    REQ_RESERVED2       =   7,
}EtReq;

typedef enum 
{
  REQ_STA_FAULT         = 0,
  REQ_STA_KEEP          = 1,
  REQ_STA_RESERVED1     = 2,
  REQ_STA_RESERVED2     = 3,
}EtReqSta;


typedef enum 
{
    SELF_SENSOR_OK = 0,
    SELF_SENSOR_RINGTIME_ERR,
    SELF_SENSOR_NO_CONNECT,
}EtSelfSta;

typedef enum 
{
    Spk_Self_Type = 0,
    Spk_Self_Chime,
    Spk_Det,
}EtSpkSta;


typedef struct
{
    EtCmdSta CurSta;
    BYTE Cursnr;
    
    DWORD dTxData;
    BYTE bTimeOutCnt;
    
    BYTE bVechInfoErrCnt;
    BYTE bSensorErrCnt[SN_NUM];
    BYTE bSensorRun[SN_NUM];
    BYTE bSelfCheckResult[SN_NUM];
    EtSpkSta SpkState;
}stCmd;

extern stCmd Cmd;
extern void Cmd_DetNextSensor(BYTE ForceCutFlag);
extern void Cmd_DetectObj (void);
extern void Cmd_ReturnForCmd(BYTE Value, EtReqSta REQ_STA);
extern BYTE Cmd_RenewalVehicleInfo(void);

#endif