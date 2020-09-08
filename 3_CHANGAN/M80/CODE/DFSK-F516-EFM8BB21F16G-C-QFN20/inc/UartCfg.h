#ifndef _UARTCFG_H_
#define _UARTCFG_H_

#define EFM8PDL_UART0_AUTO_PAGE             STD_ON
#define UART_SYS_CLOCK                      49000000 /* 49MHz */

#define Uart0_NULL_PTR                      ((void*)0)

#define BIT_SHIFLEFT(VALUE,SHIFT)		    ((VALUE&(0x01<<SHIFT))>>SHIFT)
#define FRAME_TAIL(A)						((A)|0x0F)

#define VechileDataLen   3
typedef struct {

	WORD Header               :8;
	WORD Header_EVEN          :1;
	WORD Stuff1               :7;

	WORD VechiMode            :8;
	WORD VechiMode_EVEN       :1;
	WORD Stuff2               :7;

	WORD Tail            	  :8;
	WORD Tail_EVEN            :1;
    WORD Stuff3               :7;

    WORD Checksum             :8;
    WORD Checksum_EVEN        :1;
    WORD Stuff4               :7;

}BCM_VechileMode_bit_t;


typedef struct {

	WORD Header               :8;
	WORD Header_EVEN          :1;
	WORD Stuff1               :7;

	WORD VehicleSpeed         :1;
    WORD ReversGearSts        :1;
    WORD ParkingGearSts       :1;
    WORD FrontSwitchSts       :1;
    WORD Reserve              :4;
    WORD VechileInfo_EVEN     :1;
    WORD Stuff2               :7;

    WORD Tail            	  :8;
    WORD Tail_EVEN            :1;
    WORD Stuff3               :7;

    WORD Checksum             :8;
    WORD Checksum_EVEN        :1;
    WORD Stuff4               :7;

}BCM_VechileInfo_bit_t;

typedef struct {

    WORD Header               :8;
    WORD Header_EVEN          :1;
    WORD Stuff1               :7;

    WORD ReqMode              :8;
    WORD ReqMode_EVEN         :1;
    WORD Stuff2               :7;

    WORD Tail            	  :8;
    WORD Tail_EVEN            :1;
    WORD Stuff3               :7;

    WORD Checksum             :8;
    WORD Checksum_EVEN        :1;
    WORD Stuff4               :7;

}RPAS_ReqVechiMode_bit_t;


typedef struct {

	WORD Header               :8;
	WORD Header_EVEN          :1;
	WORD Stuff1               :7;

	WORD RRErrorFlag          :1;
	WORD RMRErrorFlag         :1;
	WORD RMLErrorFlag         :1;
	WORD RLErrorFlag          :1;
	WORD FRErrorFlag          :1;
	WORD FLErrorFlag          :1;
	WORD Reserve              :2;
    WORD SelfCheck_EVEN       :1;
    WORD Stuff2               :7;

    WORD Tail            	  :8;
    WORD Tail_EVEN            :1;
    WORD Stuff3               :7;

    WORD Checksum             :8;
    WORD Checksum_EVEN        :1;
    WORD Stuff4               :7;

}RPAS_SelfCheck_bit_t;

typedef struct {

	WORD Header               :8;
	WORD Header_EVEN          :1;
	WORD Stuff0               :7;

	WORD RPADetectionRR      :4;
	WORD RPADetectionRMR       :4;
	WORD RPADetection1_EVEN   :1;
	WORD Stuff1               :7;

	WORD RPADetectionRML      :4;
	WORD RPADetectionRL       :4;
	WORD RPADetection2_EVEN   :1;
	WORD Stuff2               :7;

	WORD RPADetectionFR       :4;
	WORD RPADetectionFL       :4;
	WORD RPADetection3_EVEN   :1;
	WORD Stuff3               :7;

	WORD ObsorientRR          :1;
	WORD ObsorientRMR         :1;
	WORD ObsorientRML         :1;
	WORD ObsorientRL          :1;
	WORD ObsorientFR          :1;
	WORD ObsorientFL          :1;
	WORD Reserve              :2;
	WORD RPADetection4_EVEN   :1;
	WORD Stuff4               :7;

	WORD RPASpeakerInfo    	  :8;
    WORD RPASpeakerInfo_EVEN  :1;
    WORD Stuff5               :7;

    WORD Tail       		  :8;
    WORD Tail_EVEN            :1;
    WORD Stuff6               :7;

    WORD Checksum             :8;
    WORD Checksum_EVEN        :1;
    WORD Stuff7               :7;

}RPAS_Distance_bit_t;

typedef union
{
    BCM_VechileMode_bit_t _bit;
    WORD _Byte[4];
}BCM_Mode_buff;

typedef union
{
	BCM_VechileInfo_bit_t _bit;
	WORD _Byte[4];
}BCM_Info_buff;

typedef union
{
	RPAS_ReqVechiMode_bit_t _bit;
	WORD _Word[4];
}RPAS_ReqMode_buff;

typedef union
{
	RPAS_SelfCheck_bit_t _bit;
	WORD _Byte[4];
}RPAS_Self_buff;

typedef union
{
	RPAS_Distance_bit_t _bit;
    WORD _Byte[8];
}RPAS_Distance_buff;

//typedef union
//{
//	RPAS_Distance_bit_t _bit;
//    BYTE    _Byte[8];
//}Radar_Status_buff;

typedef enum
{
	NULL_ERR        = 0,
	NO_ERR      	= 1,
	EVEN_ERR 	    = 2,
	CHECKSUN_ERR 	= 3,
	FRAME_ERR       = 4,
}E_UartErr;


typedef enum
{
    UART0_FRM_NULL      = 0,
	UART0_FRM_REQMODE   = 1,
	UART0_FRM_VECHMOD   = 2,
	UART0_FRM_SENDSELF  = 3,
	UART0_FRM_DISTANCE  = 4,
	UART0_FRM_VECHINFO  = 5,

}E_Uart0_FrameType;
typedef struct
{
    BYTE                   CurrentFrameIdx;
    E_Uart0_FrameType      CurrentFrameType;
    BYTE                   CurrentFrameHead;
    BYTE                   CurrentFrameTail;
    BYTE                   CurrentDatalen;
    BYTE     			   CurrentFrmProsrSt;
    WORD*                  CurrentDataPtr;
    BYTE                   CurrentByteIdx;
    WORD                   CurrentCheckSum;
}Uart0_FrameInfoType;

typedef struct
{
    WORD        ReceiveBuff[4];
    WORD        ReceiveByteBuff[4];
    WORD*       pReceiveData;
    BYTE        ReceiveDatalen;
    BYTE        ReceiveByteIdx;
    WORD        ReceiveCheckSum;
    E_UartErr   ReceiveERRFlag;

}Uart0_ReceiveInfoType;

typedef struct
{
	E_Uart0_FrameType         CommState;
    BOOL                      WaitMasterACK;

    Uart0_FrameInfoType*      pCurFrameInfo;
    Uart0_ReceiveInfoType*    pReceiveFrameInfo;

}Uart0_ContextType;

extern BCM_Mode_buff       xdata  MsgBCM_Mode;
extern BCM_Info_buff       xdata  MsgBCM_Info;
extern RPAS_ReqMode_buff   xdata  MsgRadar_ReqMode;
extern RPAS_Self_buff      xdata  MsgRadar_Self;
extern RPAS_Distance_buff  xdata  MsgRadar_Distance;

//extern stUart0_FrameInfoType 	Uart0_FrameInfoType;
//extern stUart0_ReceiveInfoType 	Uart0_ReceiveInfoType;
extern Uart0_ContextType 		gslinContext;
extern Uart0_ReceiveInfoType 	gUart0ReceivePduInfo;

//static Uart0_FrameInfoType 		gCurrentPduInfo;
//static Uart0_ReceiveInfoType 	gReceivePduInfo;

extern   BYTE   CalcByteEVEN(BYTE value);
extern   BYTE   CalcFrameChecksum(E_Uart0_FrameType mode);
extern   void   UART0_DataInit(void);
extern   void   ReceiveFramereCorresponding(WORD start);
extern   void   InitUart0ReceiveVar(void);
extern   void   InitUart0AllVar(void);
#endif
