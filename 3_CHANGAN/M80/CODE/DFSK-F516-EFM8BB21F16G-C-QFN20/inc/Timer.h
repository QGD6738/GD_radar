#ifndef TIMER_H
#define TIMER_H
#include "cpu.h"

extern  BOOL xdata bTimeFlag;
extern  BOOL xdata b50msFlag;
extern  WORD xdata w50msCnt;
extern  BOOL xdata b20msFlag;
extern  WORD xdata w20msCnt;
extern  BOOL xdata bLevel;
extern  WORD xdata wTime;
extern  WORD xdata wTimeMs;
extern  BYTE xdata bSensorCheckFlag;
extern  WORD xdata wSensorCheckTime;

extern  void Time_InitVariable(void);
extern  void Time_InitPCA0(void);
extern  void Start_PCA0_Comparator(void);
extern  void Stop_PCA0_Comparator(void);
extern  void Start_PCA0_Capture(BYTE CH);
extern  void Stop_PCA0_Capture(BYTE CH);
extern  void Time_Init32msTimer(void);
extern  void Time_Init1msTimer(void);
extern  void Time_SpkInitTheTimer(void);
extern  void Time_InitTheDelayTimer(void);
extern	void Time_InitAdcTriggerTimer(void);
extern  void Time_InitUart0Timer(void);
extern  void Delay1us(WORD byTime);
extern  void Delay1ms(WORD byTime);

extern	void Time_StartAdcTrigger(void);
extern	void Time_StopAdcTrigger(void);
extern  void EnableThe32msTimerInterrupt();
extern  void DisableThe32msTimerInterrupt();
extern  void StartTheUart0Timer(void);
extern  void StopTheUart0Timer(void);

extern  void Spk_InitThePWM(void);
extern  void StartThe32msTimer (void);
extern  void StopThe32msTimer (void);
extern  void Start1msTimer (void);
extern  void Stop1msTimer (void);

#endif

