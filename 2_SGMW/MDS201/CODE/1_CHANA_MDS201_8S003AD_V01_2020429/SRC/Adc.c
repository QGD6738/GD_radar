/*********************************************************************************************************************
** FILE NAME: main.c
**
** PROJECT:  DetTemplate
** CPU:	     STM8S003F3
** COMPILER: ST
** -------------------------------------------------------------------------------------------------------------------
** VERSION:  V1.0
** DATE:     2018.09.10
*********************************************************************************************************************/

#include "ADC.h"
#include "Timer.h"
#include "DetectObj.h"



CALLBACK ADC_DetObjTypeProcess;
WORD    SampValue;
WORD    PowerValue;
BOOL    WorkFlag;
BYTE    bCsrBuffer;


/*********************************************************************************************************************
** Init ADC.
*********************************************************************************************************************/
void ADC_HardwareInit(void)
{
    //ADC_CSR  = 0X23;    //AIN0--AIN4��0x20:EOC�ж�ʹ��  
    ADC_CSR  = 0X24;    //AIN0--AIN4��0x20:EOC�ж�ʹ��  
    ADC_CR1  = 0X40;    //0X40:���� f=16M/8  0X42:����   f=16M/8
    ADC_CR2  = 0X08;    //�����Ҷ��� ��0X20:ɨ��ʧʹ��
    ADC_CR3  = 0X80;    //0X80:���ݻ���ʹ��
    ADC_TDRH = 0X00;
    ADC_TDRL = 0X06;    //AIN0--6�˿�ʩ���ش�����ֹ
    ADC_AWCRH= 0x00;    //ADCģ�⿴�Ź���ֹ
    ADC_AWCRL= 0x00;    //ADCģ�⿴�Ź���ֹ
    ADC_CR1 |= 0X01;
}

void ADC_Calue(void)
{
    WORD AD_VALUE;
    BYTE AdTimes;
    
    SampValue = 0;
    for(AdTimes = 8;AdTimes > 0;AdTimes --)
    {
        ADC_Start();
        Delay1us(20);
        AD_VALUE = ADC_DRL;
        AD_VALUE |= (uint)(ADC_DRH<<8);
        SampValue += AD_VALUE;
    }
    ADC_CSR = 0X03|0X00; //03ͨ���ǵ�Դadͨ��
    SampValue = SampValue>> 3;
}


void ADC_PowerSample (void)
{
    ADC_CSR = 0X03|0X00;            //03ͨ���ǵ�Դadͨ��
    WorkFlag = FALSE;
    while(!WorkFlag)
    {
        ADC_Calue();
        PowerValue = SampValue;
        
        if(PowerValue >  MIN_POWER_VOLTAGE&& PowerValue < MAX_POWER_VOLTAGE)
        {
            WorkFlag = TRUE;
        }
        else
        {
            WorkFlag = FALSE;
        }  
    }
 
}


void ADC_Start (void)
{
    ADC_CR1 |= 0X01;
    NOP();
    ADC_CR1 |= 0X01;
}

void ADC_Stop (void)
{
    ADC_CR1 &= (~0X01);
}


void ADC_InitDetect(void)
{
    ADC_CSR  = 0X22;     //ECHO = AIN2��0x20:EOC�ж�ʹ�� 
    ADC_CR3  = 0X80;     //0X80:���ݻ���ʹ��
    ADC_CR1  = 0X12;     //����ת����bit4,5,6 ����ad����ʱ��
    ADC_CR2  = 0x00;     //bit3=0��������룬bit1=0��ֹɨ��ģʽ
    ADC_Start ();
    bCsrBuffer = 0X22;
}

/*********************************************************************************************************************
** AD interrupt.
*********************************************************************************************************************/

#pragma vector = ADC1_EOC_vector
#pragma optimize = none
__interrupt void ADC_ISP (void)
{
    BYTE i;
    BYTE Max = 0;
    BYTE Min = 0xFF;
    
    RawData.bAdcBuffer[0] = ADC_DB0RH;
    //RawData.bClrAdLow = ADC_DB0RL;
    RawData.bAdcBuffer[1] = ADC_DB1RH;
    //RawData.bClrAdLow = ADC_DB1RL;
    RawData.bAdcBuffer[2] = ADC_DB2RH;
    //RawData.bClrAdLow = ADC_DB2RL;
    RawData.bAdcBuffer[3] = ADC_DB3RH;
    //RawData.bClrAdLow = ADC_DB3RL;
    RawData.bAdcBuffer[4] = ADC_DB4RH;
    //RawData.bClrAdLow = ADC_DB4RL;
    RawData.bAdcBuffer[5] = ADC_DB5RH;
    //RawData.bClrAdLow = ADC_DB5RL;
    RawData.bAdcBuffer[6] = ADC_DB6RH;
    //RawData.bClrAdLow = ADC_DB6RL;
    RawData.bAdcBuffer[7] = ADC_DB7RH;
    //RawData.bClrAdLow = ADC_DB7RL;
    //RawData.bAdcBuffer[8] = ADC_DB8RH;
    //RawData.bClrAdLow = ADC_DB8RL;
    //RawData.bAdcBuffer[9] = ADC_DB9RH;
    //RawData.bClrAdLow = ADC_DB9RL;
    ADC_CSR = bCsrBuffer;//����10buffer = 27us 8buffer = 16us
    for(i=0; i<8;i++)
    {
        if(Max < RawData.bAdcBuffer[i])
            Max = RawData.bAdcBuffer[i];
        if(Min > RawData.bAdcBuffer[i])
            Min = RawData.bAdcBuffer[i];
    }
    
    RawData.bRawDataBuffer[RawData.wRawDataIndex] = Max - Min;
}



