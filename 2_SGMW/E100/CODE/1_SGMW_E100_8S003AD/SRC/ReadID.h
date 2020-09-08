#ifndef READID_H
#define READID_H
#include "cpu.h"

#define USED_SN_NUM             2
#define VECHICLE_INFO_RX        TRUE
#define MASTER_SENSOR           ID_RL
typedef enum
{
    ID_RL  = 0,         //	BZ-
    ID_RR  = 1,         //	DATA
    SN_NUM = 2,
    INIT   = 255,
}eID;


extern eID ID;
extern void ID_PowerSample(void);
extern void ID_SyncTheSoftware(void);
extern void ID_GetSensorId(void);


#endif