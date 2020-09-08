#ifndef _H_CPU
#define	_H_CPU

#include <iostm8s003f3.h>
#include "Config.h"

#define PORT_INPUT     0
#define PORT_OUTPUT    1
#define V_LOW          0
#define V_HIGH         1
#define FALSE          0
#define TRUE           1

#define BIT0 0X01
#define BIT1 0X02
#define BIT2 0X04
#define BIT3 0X08
#define BIT4 0X10
#define BIT5 0X20
#define BIT6 0X40
#define BIT7 0X80

/*************ÄÚ²¿¼Ä´æÆ÷**************/
//#define SWIF        0X08 
#define SWEN        0X02
#define SWIEN       0X04
#define CSSDIE      0x04
#define CSSEN       0x01
#define HSEEN       0X01

#define HSIDIV1     0X00
#define HSIDIV2     0X08
#define HSIDIV4     0X10
#define HSIDIV8     0X18
#define CPUDIV1     0X00
#define CPUDIV2     0X01
#define CPUDIV4     0X02
#define CPUDIV8     0X03
#define CPUDIV16    0X04
#define CPUDIV32    0X05
#define CPUDIV64    0X06
#define CPUDIV128   0X07

#define PCKEN17     0x80
#define PCKEN15     0x20 
#define PCKEN14     0x10 
#define PCKEN23     0x08   
#define CCOEN       0x01

#define NOP()       {asm("nop");}
#define CLR_WDT     {IWDG_KR = 0xAA;}

typedef   unsigned char  UINT8;
typedef   unsigned int   UINT16;
typedef   unsigned long  UINT32;

typedef   unsigned char  uint8_t;
typedef   unsigned int   uint16_t;
typedef   unsigned long  uint32_t;

typedef   unsigned char  BOOL;
typedef   unsigned char  BYTE;
typedef   unsigned int   WORD;
typedef   unsigned long  DWORD;
typedef   unsigned int   uint; 
typedef   unsigned char  uchar; 

typedef   void (*CALLBACK)(void);

extern  void Cpu_Clk_Init(void);
extern  void Cpu_WDG_Init(void);
extern  void Cpu_InitHardwarePort(void);
extern  void Cpu_EnableInterrupt(void);
extern  void Cpu_DisableInterrupt(void);
extern  void Cpu_Init_Vect(void);


#endif /* _H_CPU */
