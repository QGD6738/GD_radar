#ifndef ADC_H
#define ADC_H
#include "cpu.h"

#define MIN_POWER_VOLTAGE   ((WORD)(((DWORD)((900 -65)/4/5)*1024)/100))  //412
#define MAX_POWER_VOLTAGE   ((WORD)(((DWORD)((1600-65)/4/5)*1024)/100))  //785


extern WORD SampValue;
extern void ADC_HardwareInit(void);
extern void ADC_PowerSample (void);
extern void ADC_Start(void);
extern void ADC_Stop(void);
extern void ADC_Calue(void);
extern void ADC_InitDetect(void);

#endif

