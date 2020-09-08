#ifndef _SERIAL_H_
#define _SERIAL_H_
#include "CPU.h"

#define	    DATA_RX     P0_B5
#define     DATA_TX     P0_B4
#define     DATA_LOW    (DATA_TX = 1)
#define     DATA_HIGH   (DATA_TX = 0)

#define		ID_LEN_BIT   		3
#define		DATA_LEN_BIT   		8
#define		TEST_CMD_LEN_BIT   	4
#define	    SELF_LEN_BIT		4

#define DATA_WIDTH_1	 (WORD)((DWORD)400*408/100-10)
#define DATA_WIDTH_2	 (WORD)((DWORD)600*408/100-10)
#define HEAD_800		 (WORD)((DWORD)800*408/100-10)
#define HEAD_1000		 (WORD)((DWORD)1000*408/100-10)
#define HEAD_1500		 (WORD)((DWORD)1500*408/100-10)
#define HEAD_2000		 (WORD)((DWORD)2000*408/100-10)

#define DATA_WIDTH_1_MAX (WORD)((DWORD)(400 + 40)*408/100)//VALUE*49/12 = 4.0833
#define DATA_WIDTH_1_MIN (WORD)((DWORD)(400 - 40)*408/100)

#define DATA_WIDTH_2_MAX (WORD)((DWORD)(600 + 50)*408/100)
#define DATA_WIDTH_2_MIN (WORD)((DWORD)(600 - 50)*408/100)

#define HEAD_800_MAX     (WORD)((DWORD)(800 + 60)*408/100)/*控制器发送命令捕获后为  500us*/
#define HEAD_800_MIN     (WORD)((DWORD)(800 - 60)*408/100)

#define HEAD_1000_MAX     (WORD)((DWORD)(1000 + 70)*408/100)/*控制器发送命令捕获后为  500us*/
#define HEAD_1000_MIN     (WORD)((DWORD)(1000 - 70)*408/100)

#define HEAD_1500_MAX    (WORD)((DWORD)(1500 + 95)*408/100)/*控制器发送命令捕获后为  500us*/
#define HEAD_1500_MIN    (WORD)((DWORD)(1500 - 95)*408/100)

#define HEAD_2000_MAX    (WORD)((DWORD)(2000 + 150)*408/100)/*控制器发送命令捕获后为  500us*/
#define HEAD_2000_MIN    (WORD)((DWORD)(2000 - 150)*408/100)

#define INIT_RX_DATA	{	  Serial.byCnt  = 0;\
    					  	  Serial.byData = 0;\
							  Serial.bValid = FALSE;\
							  Serial.CmdHead_Rx  = NO_CMD;\
						}

typedef     void (*SERIAL_ISP) (BOOL bLevel,WORD wTime);

typedef enum
{
    NO_CMD   		= 0,
    CMD_HEAD_800 	= 1,
	CMD_HEAD_1000 	= 2,
	CMD_HEAD_1500 	= 3,
	CMD_HEAD_2000 	= 4,
}ECMD_RX;

typedef enum
{
	NO_Com         = 0,
	COM_Internal   = 1,
	COM_Uart       = 2,
}ECOM_MODE;


typedef struct
{
    BYTE     byCnt;
    WORD     byData;
    BOOL     bDataHeader;
    BOOL     bValid;
    ECMD_RX  CmdHead_Rx;

    BYTE	 bComparatorFlag;
    BYTE	 bIO_800usFlag;
    BYTE	 bSendMaxIndex;
    BYTE	 bSendIndex;
    WORD 	 wSendBuffer[35];
    //test
    //WORD     Time[16];
    //BYTE     Index;

}StSerial;

extern StSerial 	xdata Serial;
extern SERIAL_ISP   xdata Serial_ISP;

extern void EnableInternalCommunicaiton (void);
extern void DisableInternalCommunicaiton (void);

extern void Srl_InitVariable(void);
extern void Srl_SendData(WORD Head, BYTE byId, BYTE byIdLen, WORD byData, BYTE byDataLen);
extern void Srl_GetHeader(BOOL bLevel,WORD wTime);
extern void Srl_GetData(BOOL bLevel,WORD wTime);
extern BYTE RxDataParity(void);

extern void CommunicationMode(ECOM_MODE commode);

#endif
