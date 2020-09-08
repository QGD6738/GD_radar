#ifndef _UART_H_
#define _UART_H_

#include "cpu.h"
#include "SI_EFM8BB2_Register_Enums.h"
#include "UartCfg.h"

/* SFR page used to access UART1 registers */
#define UART1_SFR_PAGE          (0x20u)

#if (EFM8PDL_UART1_AUTO_PAGE == STD_ON)
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


#define UART_SCON1              (*(volatile uint8*)0xC8u)
#define UART_SMOD1              (*(volatile uint8*)0x93u)
#define UART_SBUF1              (*(volatile uint8*)0x92u)
#define UART_SBCON1             (*(volatile uint8*)0x94u)
#define UART_SBRLH1             (*(volatile uint8*)0x96u)
#define UART_SBRLL1             (*(volatile uint8*)0x95u)
#define UART_UART1FCN0          (*(volatile uint8*)0x9Du)
#define UART_UART1FCN1          (*(volatile uint8*)0xD8u)
#define UART_UART1FCT           (*(volatile uint8*)0xFAu)
#define UART_UART1LIN           (*(volatile uint8*)0x9Eu)

/* bitmask of SCON1 */
#define UART_RX_EN                SCON1_REN__BMASK /* Receive Enable */
#define UART_TX_IF                SCON1_TI__BMASK  /* UART1 TX Interrupt */
#define UART_RX_IF                SCON1_RI__BMASK  /* UART1 RX Interrupt */
#define UART_RXOVR_EF             SCON1_OVR__BMASK /* UART1 Receive Fifo Overrun Error*/
#define UART_PARITY_EF            SCON1_PERR__BMASK/* UART1 Parity Error */

#define UART_OVR_NOT_SET          SCON1_OVR__NOT_SET
#define UART_PERR_NOT_SET         SCON1_PERR__NOT_SET
#define UART_REN_RECEIVE_DISABLED SCON1_REN__RECEIVE_DISABLED
#define UART_TBX_LOW              SCON1_TBX__LOW
#define UART_RBX_LOW              SCON1_RBX__LOW
#define UART_TI_NOT_SET           SCON1_TI__NOT_SET
#define UART_RI_NOT_SET           SCON1_RI__NOT_SET

/* bitmask of SBCON1 */
#define UART_BPS_DIV_BY_1        SBCON1_BPS__DIV_BY_1


/* bitmask of SMOD1 */
#define UART_DL_8_BITS           SMOD1_SDL__8_BITS
#define UART_SBL_SHORT           SMOD1_SBL__SHORT
#define UART_MCE_MULTI_DISABLED  SMOD1_MCE__MULTI_DISABLED
#define UART_SPT_ODD_PARITY      SMOD1_SPT__ODD_PARITY
#define UART_PE_PARIY_DISABLED   SMOD1_PE__PARITY_DISABLED
#define UART_XBE_DISABLED        SMOD1_XBE__DISABLED


/* bitmask of UART1FCN0 */
#define UART_RFRQE_ENABLED       UART1FCN0_RFRQE__ENABLED
#define UART_TFRQE               UART1FCN0_TFRQE__BMASK
#define UART_TFLSH				 UART1FCN0_TFLSH__BMASK
#define UART_TXTH				 UART1FCN0_TXTH__FMASK
#define UART_RFRQE               UART1FCN0_RFRQE__BMASK
#define UART_RFLSH               UART1FCN0_RFLSH__BMASK
#define UART_RXTH                UART1FCN0_RXTH__FMASK
#define UART_RXTH_ZERO           UART1FCN0_RXTH__ZERO   /* No RX FIFO */
#define UART_RFRQE_ENABLED       UART1FCN0_RFRQE__ENABLED
#define UART_TXTH_ZERO           UART1FCN0_TXTH__ZERO   /* No TX FIFO */
#define UART_TFRQE_DISABLED      UART1FCN0_TFRQE__DISABLED

/* bitmask of UART1FCN1 */
#define UART_TIE_ENABLED         UART1FCN1_TIE__ENABLED
#define UART_RIE_ENABLED         UART1FCN1_RIE__ENABLED
#define UART_TFRQ_IF             UART1FCN1_TFRQ__BMASK /* UART1 TX Fifo Interrupt Request */
#define UART_RFRQ_IF             UART1FCN1_RFRQ__BMASK /* UART1 RX Fifo Interrupt Request */
#define UART_TFRQ                UART1FCN1_TFRQ__BMASK
#define UART_TXHOLD              UART1FCN1_TXHOLD__BMASK
#define UART_TXNF                UART1FCN1_TXNF__BMASK
#define UART_TIE                 UART1FCN1_TIE__BMASK
#define UART_TFRQ_SET            UART1FCN1_TFRQ__SET
#define UART_TXHOLD_CONTINUE     UART1FCN1_TXHOLD__CONTINUE
#define UART_TIE_DISABLED        UART1FCN1_TIE__DISABLED
#define UART_RXTO                UART1FCN1_RXTO__FMASK
#define UART_RIE                 UART1FCN1_RIE__BMASK
#define UART_RFRQ_SET            UART1FCN1_RFRQ__SET
#define UART_RXTO_TIMEOUT_16     UART1FCN1_RXTO__TIMEOUT_16
#define UART_RIE_DISABLED        UART1FCN1_RIE__DISABLED

/* bitmask of UART1LIN */
#define UART_BREAKDNIE_ENABLED   UART1LIN_BREAKDNIE__ENABLED
#define UART_AUTOBDE_ENABLED     UART1LIN_AUTOBDE__ENABLED
#define UART_AUTOBDE_DISABLED    UART1LIN_AUTOBDE__DISABLED
#define UART_SYNCDIE_ENABLED     UART1LIN_SYNCDIE__ENABLED
#define UART_SYNCDIE_DISABLED    UART1LIN_SYNCDIE__DISABLED
#define UART_AUTOBAUD_IF         UART1LIN_SYNCD__SYNC_DONE

extern void		UART1_WriteByte(uint8 dataTx);
extern void		UART1_BreakTrigger(void);
extern void		UART1_Init(void);
extern void		UART1_Moudle_Dis(void);
extern void		UART1_INTRX_Dis(void);
extern void		UART1_INTRX_En(void);
extern void		UART1_RxDataHook(uint8 databyte);
extern void		UART1_RxErrorHook(void);

#endif /* _UART1_H_ */

