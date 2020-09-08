#include "Serial.h"
#include "Command.h"
#include "ReadID.h"
#include "DetectObj.h"
#include "Timer.h"

stSrl Srl;
SERIAL_ISP  Serial_ISP = GetHeader;

void EnableInternalCommunicaiton (void)
{   
    if(DetInfo.bID < 4)        //Rear sensors
    {
        PD_CR2_C26  = V_HIGH;    //开启外部中端---data
    }
    else		       //Front sensors
    {
        PD_CR2_C23  = V_HIGH;    //开启外部中端---id
    }
    Time_Init32msTimer();
}
void DisableInternalCommunicaiton (void)
{
    if(DetInfo.bID < 4)	       //Rear sensors
    {
        PD_CR2_C26  = V_LOW;     //关闭外部中端---data
    }
    else	               //Front sensors
    {
        PD_CR2_C23  = V_LOW;     //关闭外部中端---id
    }
}

/*
static WORD ISP_TIME[30];
static WORD ISP_LEVEL[30];
static WORD ISP_INDEX;
*/

#pragma vector = 0X08 /*0X08 = PD INPUT*/
__interrupt void Serial_Port_ISP(void)
{

    bLevel = (BOOL)DATA_RX;
    wTime = TIM1_CNTRH; 
    wTime = wTime<<8|TIM1_CNTRL;
    
    TIM1_CR1 &= (~0x01);
    TIM1_CNTRH = 0X00;
    TIM1_CNTRL = 0X00;
    TIM1_CR1 |= 0x01;

    (*Serial_ISP)(bLevel,wTime);
    
    //test
    /*
    if(ISP_INDEX < 30)
    {
        ISP_TIME[ISP_INDEX] = wTime;
        ISP_LEVEL[ISP_INDEX] = bLevel;
        ISP_INDEX++;
    }
    else
    {
        ISP_INDEX = 0;
    }*/
    
}

static BYTE ReadFlag;
static BYTE RecBitCnt;
void GetHeader(BOOL bLevel,WORD wTime)
{
    if(bLevel)
    {
        Srl.dData = 0;
        Srl.bCnt  = 0;
        Srl.bValid = FALSE;
        //Srl.RcvSta = E_HEAD_NULL;
        if(wTime > HEAD_SENSOR_BACK-HEAD_ERROR && wTime < HEAD_SENSOR_BACK+HEAD_ERROR)  //640us
        {
            if(DetInfo.bID == MASTER_SENSOR)
            {
                Serial_ISP = &GetData;
                if(Srl.bBackFlag == E_HEAD_SENSOR_BACK)
                {
                    RecBitCnt= 11;
                    //Srl.RcvSta = E_HEAD_SENSOR_BACK;
                }
                else
                {
                    RecBitCnt = 6;
                    //Srl.RcvSta = E_HEAD_VEHICLE_BACK;
                }
            }
        }
        if(wTime > HEAD_SENSOR_DRIVE-HEAD_ERROR && wTime < HEAD_SENSOR_DRIVE+HEAD_ERROR)//1000us
        {
            Serial_ISP = &GetData;
            RecBitCnt = 6;
            //Srl.RcvSta = E_HEAD_SENSOR_DRIVE;
        }
        /*
        if(wTime > HEAD_SENSOR_SET-HEAD_ERROR && wTime < HEAD_SENSOR_SET+HEAD_ERROR)    //3000us
        {
            Serial_ISP = &GetData;
            RecBitCnt = 8;
            //Srl.RcvSta = E_HEAD_SENSOR_SET;
            Cmd.CurSta = CMD_SET;
        }*/
        
    }
}

void GetData(BOOL bLevel,WORD wTime)
{
    if(bLevel)
    {
        if(wTime > (DATA_NARROW-DATA_ERROR) && wTime < (DATA_NARROW+DATA_ERROR))
        {
            if(ReadFlag == TRUE)
            {
                Srl.dData |= ((DWORD)0x00000001<<Srl.bCnt);
                if(++Srl.bCnt == RecBitCnt)
                {
                    Serial_ISP = GetHeader;
                    Srl.bValid = TRUE;
                }
            }
            else
            {
                COMMUNICATION_ERR;
            }
        }
        else if(wTime > (DATA_WIDE-DATA_ERROR) && wTime < (DATA_WIDE+DATA_ERROR))
        {
            if(ReadFlag == FALSE)
            {
                if(++Srl.bCnt == RecBitCnt)
                {
                    Serial_ISP = GetHeader;
                    Srl.bValid = TRUE;
                }
            }
            else
            {
                COMMUNICATION_ERR;
            }
        }
        else
        {
            COMMUNICATION_ERR;
        }
    }
    else
    {
        if(wTime > (DATA_NARROW-DATA_ERROR) && wTime < (DATA_NARROW+DATA_ERROR))
        {
            ReadFlag = FALSE;
        }
        else if(wTime > (DATA_WIDE-DATA_ERROR) && wTime < (DATA_WIDE+DATA_ERROR))
        {
            ReadFlag = TRUE;
        }
        else
        {
            COMMUNICATION_ERR;
        }
    }
}

void SendData(DWORD *pData, BYTE Len, WORD HeadWidth)
{
    BYTE i;
    BYTE TX_HIGH,TX_LOW;
    BYTE Temp[30];
    
    for(i=0;i<Len;i++)
    {
        if(*pData & ((DWORD)0x00000001<<i))
        {
            Temp[i] = 1;
        }
        else
        {
            Temp[i] = 0;
        }
    }
    
    
    Cpu_DisableInterrupt();
    
    TX_HIGH = PD_ODR;
    TX_HIGH = TX_HIGH & (~0x20);
    TX_LOW  = TX_HIGH | ( 0x20);
    
    /* Sync header */
    PD_ODR = TX_LOW;
    Delay1us(HeadWidth);
    PD_ODR = TX_HIGH;
    for(i=0; i<Len; i++)
    {
        if(Temp[i])
        {
            Delay1us(300);
            PD_ODR = TX_LOW;
            Delay1us(120);
            PD_ODR = TX_HIGH;
        }
        else
        {
            Delay1us(120);
            PD_ODR = TX_LOW;
            Delay1us(300);
            PD_ODR = TX_HIGH;
        }
    }
    Delay1us(100);
    Cpu_EnableInterrupt();
}