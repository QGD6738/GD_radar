#ifndef ADC_H
#define ADC_H
#include "cpu.h"

#define MIN_POWER_VOLTAGE   ((WORD)(((DWORD)((700 -0)/4/5)*1024)/100))  //239
#define MAX_POWER_VOLTAGE   ((WORD)(((DWORD)((1600-0)/4/5)*1024)/100))  //546


extern WORD SampValue;
extern void ADC_HardwareInit(void);
extern void ADC_PowerSample (void);
extern void ADC_Start(void);
extern void ADC_Stop(void);
extern void ADC_Calue(void);
extern void ADC_InitDetect(void);

#endif

