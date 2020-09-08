#ifndef SERIAL_H
#define SERIAL_H
#include "CPU.h"


#define	    DATA_RX     PD_IDR_IDR6
#define     DATA_TX     PD_ODR_ODR5
#define     DATA_LOW    (DATA_TX = 1)
#define     DATA_HIGH   (DATA_TX = 0)

#define	    ID_RX       PD_IDR_IDR3
#define	    ID_TX       PD_ODR_ODR4
#define	    ID_LOW      (ID_TX = 1)
#define	    ID_HIGH     (ID_TX = 0)

#define     HEAD_ERROR                100    /* Unit£ºus*/
#define     HEAD_SENSOR_BACK          640    /* Unit£ºus*/
#define     HEAD_VEHICLE_BACK         640    /* Unit£ºus*/
#define     HEAD_SENSOR_DRIVE         1000   /* Unit£ºus*/
#define     HEAD_SENSOR_SET           3000   /* Unit£ºus*/

#define     HEAD_RETURN_WORK          1500   /* Unit£ºus*/
#define     HEAD_RETURN_SELF          2000   /* Unit£ºus*/

#define     DATA_ERROR                50        
#define     DATA_NARROW               120
#define     DATA_WIDE                 300
typedef void (*SERIAL_ISP) (BOOL bLevel,WORD wTime);

#define     COMMUNICATION_ERR         {/*Srl.RcvSta = E_HEAD_NULL;*/ \
                                       Srl.dData = 0; \
                                       Srl.bCnt  = 0; \
                                       Serial_ISP = &GetHeader; }
typedef enum EtRcvSta
{
    E_HEAD_NULL = 0,
    E_HEAD_SENSOR_BACK,
    E_HEAD_VEHICLE_BACK,
    E_HEAD_SENSOR_DRIVE,
    E_HEAD_SENSOR_SET,
}EtRcvSta;

typedef struct
{
    //EtRcvSta RcvSta;
    DWORD dData;
    BYTE  bCnt;
    BOOL  bValid;
    BOOL  bBackFlag;
}stSrl;

extern stSrl Srl;
extern SERIAL_ISP  Serial_ISP;

extern void EnableInternalCommunicaiton(void);
extern void DisableInternalCommunicaiton (void);
extern void GetHeader(BOOL bLevel,WORD wTime);
extern void GetData(BOOL bLevel,WORD wTime);

extern void SendData(DWORD *pData, BYTE Len, WORD HeadWidth);
#endif