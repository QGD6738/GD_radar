#ifndef TIMER_H
#define TIMER_H
#include "cpu.h"

extern  BOOL  bTimeFlag;
extern  BOOL  bLevel;
extern  WORD  wTime;
extern  BYTE  bSensorCheckFlag;
extern  WORD  wSensorCheckTime;
extern  WORD  wDelayCnt;
  
extern  void Time_InitVariable(void);
extern  void Time_Init32msTimer(void);
extern  void Time_Init1msTimer(void);
extern  void Time_SpkInitTheTimer(void);
extern  void Time_InitTheDelayTimer(void); 
extern	void Time_InitAdcTriggerTimer(void);
extern  void Delay1us(WORD byTime);

extern	void Time_StartAdcTrigger(void);
extern	void Time_StopAdcTrigger(void);
extern  void EnableThe32msTimerInterrupt();
extern  void DisableThe32msTimerInterrupt();

extern  void Spk_InitThePWM(void);
extern  void StartThe32msTimer (void);
extern  void StopThe32msTimer (void);
extern  void Start1msTimer (void);
extern  void Stop1msTimer (void);

#endif

