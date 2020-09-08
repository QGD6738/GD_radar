#ifndef ADC_H
#define ADC_H
#include "cpu.h"

#define MAX_COMPARE(A,B)  ( (A) > (B) ? (A) : (B))
#ifdef MAX_Set
#define MAX_COMPARE(A,B)  ({\
							typeof(A) a_=(A);\
							typeof(B) b_=(B);\
							(a_>b_) ? a_ : b_;\
})
#endif


#define MIN_POWER_VOLTAGE   ((WORD)(((DWORD)((8500 -0)/6/33)*1024)/100))  //358
#define MAX_POWER_VOLTAGE   ((WORD)(((DWORD)((16500-0)/6/33)*1024)/100))  //849
//
//#define MIN_RL_VOLTAGE   ((WORD)(((DWORD)((8500 -0)/6/33)*1024)/100))  //358
//#define MAX_RL_VOLTAGE   ((WORD)(((DWORD)((16500-0)/6/33)*1024)/100))  //849
//
//#define MIN_RML_VOLTAGE   ((WORD)(((DWORD)((8500 -0)/6/33)*1024)/100))  //358
//#define MAX_RML_VOLTAGE   ((WORD)(((DWORD)((16500-0)/6/33)*1024)/100))  //849
//
//#define MIN_RR_VOLTAGE   ((WORD)(((DWORD)((8500 -0)/6/33)*1024)/100))  //358
//#define MAX_RR_VOLTAGE   ((WORD)(((DWORD)((16500-0)/6/33)*1024)/100))  //849
//
//#define MIN_FL_VOLTAGE   ((WORD)(((DWORD)((8500 -0)/6/33)*1024)/100))  //358
//#define MAX_FL_VOLTAGE   ((WORD)(((DWORD)((16500-0)/6/33)*1024)/100))  //849
//
//#define MIN_FR_VOLTAGE   ((WORD)(((DWORD)((8500 -0)/6/33)*1024)/100))  //358
//#define MAX_FR_VOLTAGE   ((WORD)(((DWORD)((16500-0)/6/33)*1024)/100))  //849

#define POWER_VOL_NORMAL	0
#define POWER_VOL_LOW		1
#define POWER_VOL_HIGH		2

#define RML_ADSAMPLE_DEC    50

typedef struct
{
	WORD Power_SampleValue;
	WORD ID_SampleValue;
	WORD PowerValueTest;
	WORD PowerValueSiteTest;
	WORD IDValueTest;
	WORD IDValueSiteTest;
	BOOL Power_OK_Flag;
}stADC;



extern void ADC_InitVariable(void);
extern void ADC_PowerSamplingInit (void);
extern void ADC_IDSamplingInit (void);
extern void ADC_PowerSample(void);
extern void ADC_Start (void);
extern void ADC_Stop (void);
extern void ADC_InitDetect(void);
extern void ADC_ReadID(void);

extern stADC    xdata   ADC;

extern BYTE 	xdata   bySampleTimes;  //Remainning sample times
extern WORD 	xdata   wSampleValue;   //AD value
extern WORD 	xdata   wPowerValue;    //Power value
extern BOOL 	xdata   bWorkFlag;      //Work flag
extern BOOL 	xdata	bDetectFlag;	//Detect flag
#endif
