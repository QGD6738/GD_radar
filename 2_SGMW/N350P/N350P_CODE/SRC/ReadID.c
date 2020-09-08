#include "cpu.h"
#include "ADC.h"
#include "Timer.h"
#include "Serial.h"
#include "ReadID.h"
#include "DetectObj.h"

eID ID;

WORD RL_MIN_VALUE;      // GND
WORD RL_MAX_VALUE;

WORD RML_MIN_VALUE;	// +12V
WORD RML_MAX_VALUE;

WORD RR_MIN_VALUE;	// NC
WORD RR_MAX_VALUE;


void ID_PowerSample (void)
{
    ADC_CSR = 0X03|0X00;         //03通道是电源ad通道
    ADC_Calue();

    RL_MIN_VALUE = 900;
    RL_MAX_VALUE = 1025;
    RR_MIN_VALUE = (WORD)((DWORD)(SampValue-35)*100/263*62*4/100-200);
    RR_MAX_VALUE = (WORD)((DWORD)(SampValue-35)*100/263*62*4/100+100);
    
    //RL_MIN_VALUE = (WORD)((DWORD)(SampValue-35)*100/158*62*4/100);//62/113
    //RL_MAX_VALUE = 1025;//(WORD)((DWORD)(SampValue-35)*100/113*62*4/100);

    //RR_MIN_VALUE = 0;  //(WORD)((DWORD)(SampValue-35)*100/263*62*4/100);//62/263
    //RR_MAX_VALUE = (WORD)((DWORD)(SampValue-35)*100/158*62*4/100);
    
}
BYTE Debug_data;
void ID_GetSensorId (void)
{
    ADC_CSR = 0X04|0X00;         //04通道是IDad通道
    ADC_Calue();
    
    if(SampValue >= RR_MIN_VALUE && SampValue <= RR_MAX_VALUE)
    {
        ID = ID_RR; 
    }
    else if(SampValue >= RL_MIN_VALUE && SampValue <= RL_MAX_VALUE)
    {
        ID = ID_RL; 
    }
}

void ID_SyncTheSoftware (void)
{
    
}
