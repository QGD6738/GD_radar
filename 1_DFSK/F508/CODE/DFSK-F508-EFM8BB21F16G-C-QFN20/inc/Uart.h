#ifndef UART_H
#define UART_H
#include "cpu.h"
#include "UartCfg.h"


/* SFR page used to access UART1 registers */
#define UART0_SFR_PAGE          (0x20u)

#if (EFM8PDL_UART0_AUTO_PAGE == STD_ON)
/* declare variable needed for autopage enter/exit */
#define DECL_PAGE uint8_t savedPage
/* enter autopage section */
#define SET_PAGE(p)         do                                                   \
	                        {                                                    \
	                          savedPage = SFRPAGE;  /* save current SFR page */  \
	                          SFRPAGE = (p);        /* set SFR page */           \
	                        } while(0)
/* exit autopage section */
#define RESTORE_PAGE        do                                                   \
		                    {                                                    \
		                      SFRPAGE = savedPage;  /* restore saved SFR page */ \
		                    } while(0)
#else
#define DECL_PAGE
#define SET_PAGE(x)
#define RESTORE_PAGE
#endif

typedef enum
{
	Rec_ERR        	= 0,
	Rec_OK      	= 1,
	Rec_VechiMode 	= 2,
	Rec_VechiInfo 	= 3,
	Reserve       	= 4,
}E_UartReceiveFlag;

extern BOOL Uart0TX_NOTOK_Flag;
extern BOOL Uart0TX_ALL_Flag;
extern E_UartReceiveFlag Uart0RX_OK_Flag;

extern   void   Uart0_Init(void);
extern   void   UART0_INTRX_Dis(void);
extern   void   UART0_INTRX_En(void);
extern   void   UART0_ModeDataL(void);
extern   void   UART0_ModeDataH(void);
extern   void   UART0_Moudle_Dis(void);
extern   void   UART0_WriteByte(WORD dataTx);
extern   void   UART0_DataInit(void);
extern   void   UART0_RXData(WORD rxdata);

#endif
