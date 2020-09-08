#ifndef _H_CPU
#define	_H_CPU

#include <SI_EFM8BB2_Register_Enums.h>                  // SFR declarations

/*******************************************************************************
*   Macro
*******************************************************************************/
#define STD_HIGH     (1)
#define STD_LOW      (0)

#define STD_ACTIVE   (1)
#define STD_IDLE     (0)

#define STD_ON       (1)
#define STD_OFF      (0)

#define PORT_INPUT     0
#define PORT_OUTPUT    1
#define V_LOW          0
#define V_HIGH         1
#define FALSE          0
#define TRUE           1
#define FUN_OFF        0
#define FUN_ON         1


#define BIT0 0X01
#define BIT1 0X02
#define BIT2 0X04
#define BIT3 0X08
#define BIT4 0X10
#define BIT5 0X20
#define BIT6 0X40
#define BIT7 0X80

#define CLR_WDT     {WDTCN = 0xA5;}

typedef   unsigned char  UINT8;
typedef   unsigned int   UINT16;
typedef   unsigned long  UINT32;

typedef   unsigned char  uint8;
typedef   unsigned int   uint16;
typedef   unsigned long  uint32;

typedef   unsigned char  BOOL;
typedef   unsigned char  BYTE;
typedef   unsigned int   WORD;
typedef   unsigned long  DWORD;
typedef   unsigned int   uint;
typedef   unsigned char  uchar;

typedef   void (*CALLBACK)(void);

extern  void Cpu_Clk_Init();
extern  void Cpu_WDG_Init(BOOL Switch);
extern  void Cpu_InitHardwarePort(void);
extern  void Cpu_EnableInterrupt(void);
extern  void Cpu_DisableInterrupt(void);
extern  void Cpu_Init_Vect(void);

extern BYTE SFRPAGE_TEMP;

#endif /* _H_CPU */
