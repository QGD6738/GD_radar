#include "cpu.h"
#include "Timer.h"
#include "Serial.h"
#include "Uart.h"
#include "Command.h"
#include "DetectObj.h"
#include "Adc.h"
#include "EFM8BB2_FlashPrimitives.h"

stCmd xdata Cmd;
const Self_Straighten[6]={0,3,2,0,5,4};
BYTE c5times = 0;

void Cmd_InitVariable(void)
{
	Cmd.bID = ID_RL;
	Cmd.wWaitTime = MASTER_WORK_TIME_MAX;
	Cmd.wBackData = 0;
	Cmd.CmdStatus = MST_DRV_SENSOR_SELFCHECK;
	//Cmd.CmdCalibration;
	//Cmd.bOverCnt[ID_SUM];
	//Cmd.bSensorErrCnt[ID_SUM];
	//Cmd.bSensorRun[ID_SUM];
	Cmd.bUsedSnrNUM = 2;
	//Cmd.bVechInfoErrCnt;
	//Cmd.bDistance[ID_SUM];
	//Cmd.bObsorient[ID_SUM];
	Cmd.bBuzzerInfo = 3;
	//Cmd.Debug_ErrCnt[ID_SUM];
	//Cmd.Debug_OKCnt[ID_SUM];
	//Cmd.Debug_bVechInfoErrCnt = 0;
	//Cmd.Debug_bVechInfoOKCnt = 0;
}


void Cmd_ReadID(void)
{


}

void Cmd_GetVechileMode(void)
{
	BYTE mode_temp;
	mode_temp = 0;
	Cmd.wWaitTime = 170;

	CommunicaitonMode(COM_Internal);
	//The delay time need to adjust
	Delay1us(START_UART_DELAY);
	while(Cmd.wWaitTime)
	{
		if(bTimeFlag)
		{
			bTimeFlag = FALSE;
			Cmd.wWaitTime--;
			if(Cmd.wWaitTime%50 == 0)
			{
				Cmd_SendReqVechiModetoBCM();
			}
			if(Uart0RX_OK_Flag == Rec_VechiMode)
			{
				if((MsgBCM_Mode._bit.VechiMode&0xF0) == 0x30)
				{
					Cmd.bUsedSnrNUM = 3;
				}
				else if((MsgBCM_Mode._bit.VechiMode&0xF0) == 0x50)
				{
					Cmd.bUsedSnrNUM = 5;
				}
				mode_temp = MsgBCM_Mode._bit.VechiMode;
				Flash_WriteVechileMode(mode_temp);
				break;
			}
		}
		CLR_WDT;
	}

	if(Cmd.wWaitTime == 0)
	{
		mode_temp = Flash_ReadVechileMode();
		Cmd.bUsedSnrNUM = (mode_temp >> 4);
		if(Cmd.bUsedSnrNUM != 3 && Cmd.bUsedSnrNUM != 5)
		{
			Cmd.bUsedSnrNUM = 5;
		}
	}
	CommunicaitonMode(NO_Com);
}



void Cmd_Process(void)
{
	BYTE self_temp,cnt;
	static BYTE i = 0;

    if(bSensorCheckFlag == TRUE)
    {
        DetInfo.bDetTriggerFlag = TRUE;
        return;
    }

	switch(Cmd.CmdStatus)
		{
			case MST_DRV_SENSOR_SELFCHECK:
			{
				Srl_SendData(HEAD_2500, Cmd.bID, ID_LEN_BIT, 0, 0);
				Cmd.wWaitTime = MASTER_TXANDRX_TIME_OVER_MAX_TEST;
				Cmd.CmdStatus = MST_WAIT_SELFCHECK;
				break;
			}
			case MST_WAIT_SELFCHECK:
			{
				if(Cmd.wWaitTime > 0)
				{
					if(Serial.CmdHead_Rx == CMD_HEAD_1000) //Receive data efficacious)
					{
						if(Serial.bValid == TRUE)
						{
							Serial.bValid = FALSE;
							Cmd.bSensorErrCnt[Cmd.bID] = 0;
							if((Serial.byData&0x07) == Cmd.bID)
							{
								Cmd_SaveData();
								Cmd_NextSensorSelf();
							}
							else
							{
								Cmd.wWaitTime = 1;
							}
						}
					}
					Cmd.wWaitTime--;
				}
				else
				{
					Cmd.CmdStatus = MST_DRV_SENSOR_SELFCHECK;
					if((++Cmd.bSensorErrCnt[Cmd.bID]) >= 3)
					{
						Cmd.bSensorErrCnt[Cmd.bID] = 0;
						DiagInfo.bSelfCheckResult[Cmd.bID] = 1;
						Cmd_NextSensorSelf();
					}
				}

				break;
			}
			

			case MST_SEND_SELF_TO_BCM:
			{
#if(TEST_CODE_FIXED_ID == TRUE)
	{
		DiagInfo.bSelfCheckResult[ID_RL] = 0;
		DiagInfo.bSelfCheckResult[ID_RML] = 0;
		DiagInfo.bSelfCheckResult[ID_RR] = 0;
		DiagInfo.bSelfCheckResult[ID_FL] = 0;
		DiagInfo.bSelfCheckResult[ID_FR] = 0;
	}
#endif
				self_temp = 0;
				cnt = 0;

				for(i=1;i<(Cmd.bUsedSnrNUM+1);i++)
				{
					self_temp |= DiagInfo.bSelfCheckResult[i] << Self_Straighten[i];
				}
				MsgRadar_Self._Byte[1] = self_temp;
				
				CommunicaitonMode(COM_Internal);
				for(i=0;i<5;i++)
				{
					Cmd_SendSelfChecktoBCM();
					Delay1ms(20);
				}
				c5times ++;
				Cmd.CmdStatus = MST_DRV_SENSOR;
				Cmd.bID = ID_RL;
				//CommunicaitonMode(COM_Internal);
				break;
			}

			case MST_DRV_SENSOR:
			{
				Delay1ms(5);    //It is must,for after Communit with MP5,the inter communit wrong rate too high
				Srl_SendData(HEAD_800, Cmd.bID, ID_LEN_BIT, 0, 0);
				Cmd.wWaitTime = MASTER_WORK_TIME_MAX;
				Cmd.CmdStatus = MST_WAIT_SENSOR_DATA;
				break;
			}

			case MST_WAIT_SENSOR_DATA:
			{
				if(Cmd.wWaitTime > 0)
				{
					if(Serial.CmdHead_Rx == CMD_HEAD_1000) //Receive data efficacious)
					{
						if(Serial.bValid == TRUE)
						{
							Serial.bValid = FALSE;
							if((Serial.byData&0x07) == Cmd.bID)
							{
								Cmd.bSensorErrCnt[Cmd.bID] = 0;
								Cmd_SaveData();
						    	Cmd.Debug_OKCnt[Cmd.bID]++;
								Cmd_NextSensorDet();
							}
						}
					}
					Cmd.wWaitTime--;
				}
				else
				{
			    	 Cmd.Debug_ErrCnt[Cmd.bID]++;
					if(++Cmd.bSensorErrCnt[Cmd.bID] >= SENSOR_EER_THRE_SENSOR)
					{
						Cmd.bSensorErrCnt[Cmd.bID] = SENSOR_EER_THRE_SENSOR;
						DiagInfo.bSelfCheckResult[Cmd.bID] = 1;
						NearestData.bEachNearestDistance[Cmd.bID] = NO_OBJ;
					}
					Cmd_NextSensorDet();
				}
				break;
			}

			case MST_SEND_DISTANCE_TO_BCM:
			{
				if(Cmd.wWaitTime == 100)
				{
					Cmd_UpdataDistance();
					CommunicaitonMode(COM_Internal);
					InitUart0ReceiveVar();
					Cmd_SendDistancetoBCM();
					Cmd.wWaitTime = 15;

				}
				if(Cmd.wWaitTime > 0)
				{

#if(TEST_CODE_VEHICLE == TRUE)
{
	Uart0RX_OK_Flag = Rec_VechiInfo;
	MsgBCM_Info._bit.ReversGearSts = TRUE;
}
#endif
					Cmd.wWaitTime--;
				}
				else if(Cmd.wWaitTime == 0)
				{
					Cmd_NextSensorDet();
				}

				break;
			}
			case MST_WAIT_VECHILE_CFG:
			{
#if(TEST_CODE_VEHICLE == TRUE)
{
	Uart0RX_OK_Flag = Rec_VechiInfo;
	MsgBCM_Info._bit.ReversGearSts = TRUE;
}
#endif
				if(Cmd.wWaitTime > 0)
				{
					/*
					if(Cmd_RenewVechileCfg() == TRUE)
					{
						InitUart0ReceiveVar();
						Cmd_NextSensorDet(TRUE);
					}
					*/
					Cmd.wWaitTime--;
				}
				else
				{
					InitUart0ReceiveVar();
					if(++Cmd.bVechInfoErrCnt > SENSOR_EER_THRE_CAR)
					{
						Cmd.bVechInfoErrCnt = SENSOR_EER_THRE_CAR;
//						Cmd_ClearBuffer();
						DetObj_InitAllChannelDistance();
						Cmd_UpdataDistance();
						Cmd.bID = ID_RL;
						Cmd.CmdStatus = MST_SEND_DISTANCE_TO_BCM;
					}
				}
				break;
			}

			case CMD_STOP:
			{
#if(TEST_CODE_VEHICLE == TRUE)
{
	Uart0RX_OK_Flag = Rec_VechiInfo;
	MsgBCM_Info._bit.ReversGearSts = TRUE;
}
#endif
				if(Cmd.wWaitTime > 0)
				{
					/*
					if(Cmd_RenewVechileCfg() == TRUE)
					{
						Cmd_NextSensorDet(TRUE);
					}
					*/
					Cmd.wWaitTime--;
				}
				else
				{
					DetObj_InitAllChannelDistance();
					Cmd_UpdataDistance();
					CommunicaitonMode(COM_Internal);
					Cmd_SendDistancetoBCM();
					Cmd.wWaitTime = SENSOR_STOP_SCAN_TIME;
				}
				break;
			}

			case SLV_WAIT_CMD:
			{
				if(Serial.CmdHead_Rx == CMD_HEAD_800)	//work command head
				{
					if(Serial.bValid == TRUE)
					{
						Serial.bValid = FALSE;

						if((Serial.byData&0x07) == DetInfo.bID)
						{
							Cmd.wWaitTime = SINGLE_SENSOR_TIME_OVER_MAX;
							DetInfo.bDetTriggerFlag = TRUE;
							CommunicaitonMode(NO_Com);
							Cmd.CmdStatus = SLV_DISTANCE_BACK;
						}
					}
				}

				if(Serial.CmdHead_Rx == CMD_HEAD_3000)	//Work frock command head
				{
					if(Serial.bValid == TRUE)
					{
						Serial.bValid = FALSE;
						CommunicaitonMode(COM_Internal);
						Cmd.CmdStatus = SLV_CALIBRATION;
					}
				}

				if(Serial.CmdHead_Rx == CMD_HEAD_2500)    //Self check command head
				{
					if(Serial.bValid == TRUE)
					{
						Serial.bValid = FALSE;
						if((Serial.byData&0x07) == DetInfo.bID)
						{
							Cmd.wWaitTime = SINGLE_SENSOR_TIME_OVER_MAX;
							DetInfo.bDetTriggerFlag = FALSE;
							CommunicaitonMode(COM_Internal);
							Cmd.CmdStatus = SLV_DISTANCE_BACK;
						}
					}
				}

				break;
			}

			case SLV_DISTANCE_BACK:
			{
#if(TEST_CODE_DETDATA == TRUE)
{
	NearestData.bNearestDistance          = 0x20;
	DiagInfo.bSelfCheckResult[DetInfo.bID] = 0;		//0-OK,1-NG
	DiagInfo.bKeepFlag                     = 0;
}
#endif

//				CommunicaitonMode(COM_Internal);
				Cmd.wBackData =(((WORD)NearestData.bNearestDistance)|
						        ((WORD)DiagInfo.bSelfCheckResult[DetInfo.bID]<<8)|
								((WORD)DiagInfo.bKeepFlag<<9));
				Srl_SendData(HEAD_1000, DetInfo.bID, ID_LEN_BIT, Cmd.wBackData, 10);
				CommunicaitonMode(COM_Internal);
				Cmd.CmdStatus = SLV_WAIT_CMD;
				break;
			}

			case SLV_CALIBRATION:
			{
				if(Serial.CmdHead_Rx == CMD_HEAD_3000)
				{
					if(Serial.bValid == TRUE)
					{
						Serial.bValid = FALSE;
						Cmd.CmdCalibration = (Serial.byData&0xF);
						DetInfo.bDetTriggerFlag = TRUE;
						Stop_PCA0_Capture(0);		//DATA
						Cmd.CmdStatus = SLV_CALIBRATION_BACK;
					}
				}
				break;
			}

			case SLV_CALIBRATION_BACK:
			{
				switch(Cmd.CmdCalibration)
				{
					case CMD_REQUEST_DISTANCE : 	Cmd.wBackData = ClrtData.bDistance;		break;
					case CMD_REQUEST_RINGTIME :		Cmd.wBackData = (ClrtData.wRingTime/10);		break;
					case CMD_REQUEST_ECHO_WIDTH	:	Cmd.wBackData = ClrtData.wTimeWidth;	break;
					case CMD_REQUEST_ECHO_VOL :		Cmd.wBackData = ClrtData.bVolHeight;	break;
					case CMD_REQUEST_SOFTWARE_VERSION :		Cmd.wBackData = 0x01;			break;
					case CMD_REQUEST_HARDWARE_VERSION :		Cmd.wBackData = 0x01;			break;
					case CMD_REQUEST_ID		   :
						Cmd.wBackData = DetInfo.bID;
						break;
					default:
						Cmd.wBackData = 0x00;
				}
				Srl_SendData(HEAD_3000, Cmd.CmdCalibration, TEST_CMD_LEN_BIT, Cmd.wBackData, 8);
				Start_PCA0_Capture(0);		//DATA
				Cmd.CmdStatus = SLV_CALIBRATION;
				break;
			}

			case DEBUG_MODE:
			{
#if(TEST_CODE_VEHICLE == TRUE)
{
	Uart0RX_OK_Flag = Rec_VechiInfo;
	MsgBCM_Info._bit.ReversGearSts = TRUE;
}
#endif
				if(Cmd.wWaitTime > 0)
				{
				    /*
					if(Cmd_RenewVechileCfg() == TRUE)
					{
						Cmd.CmdStatus = DEBUG_MODE;
						Cmd.wWaitTime = 1;
					}
					*/
					Cmd.wWaitTime--;
				}
				else
				{
					DetObj_InitAllChannelDistance();
					Cmd_UpdataDistance();
					CommunicaitonMode(COM_Internal);
					Cmd_SendDistancetoBCM();
					Cmd.wWaitTime = SENSOR_STOP_SCAN_TIME;
				}
				break;
			}


			default:
			;
		}




}

void Cmd_SaveData(void)
{
	if(Cmd.bID != ID_MASTER)
	{
		if(((Serial.byData >> 11) & 0x01) == 0x00) //The self-test data are normal; 1 is error
		{
			//if(((Serial.byData >> 12) & 0x01) == 0x00) //Determine whether the sensor keep or not; 1 is keep
			DiagInfo.bSelfCheckResult[Cmd.bID] = 0;
			NearestData.bEachNearestDistance[Cmd.bID] = (BYTE)(Serial.byData >> 3);
		}
		else
		{
			DiagInfo.bSelfCheckResult[Cmd.bID] = 0;
			NearestData.bEachNearestDistance[Cmd.bID] = 0xFF;
		}
	}
	Serial.byData = (WORD)0x0000;
}

void Cmd_UpdataDistance(void)
{
	Cmd_DistancetoComInfo();

	MsgRadar_Distance._bit.RPADetectionRL = 0x0F;
	MsgRadar_Distance._bit.RPADetectionRML = 0x0F;
	MsgRadar_Distance._bit.RPADetectionRMR = 0x0F;
	MsgRadar_Distance._bit.RPADetectionRR = 0x0F;
	MsgRadar_Distance._bit.RPADetectionFL = 0x00;
	MsgRadar_Distance._bit.RPADetectionFR = 0x00;

	MsgRadar_Distance._bit.ObsorientRL = 0x0F;
	MsgRadar_Distance._bit.ObsorientRML = 0x0F;
	MsgRadar_Distance._bit.ObsorientRMR = 0x0F;
	MsgRadar_Distance._bit.ObsorientRR = 0x0F;
	MsgRadar_Distance._bit.ObsorientFL = 0x00;
	MsgRadar_Distance._bit.ObsorientFR = 0x00;
	MsgRadar_Distance._bit.Reserve = 0x00;

	MsgRadar_Distance._bit.RPASpeakerInfo = Cmd.bBuzzerInfo;
}

void Cmd_DistancetoComInfo(void)
{
	BYTE i;
	BYTE NearestDis;
	NearestDis = 0xFF;

//	for(i=0;i<ID_SUM;i++)					//distance value cale to command data
//	{
//		if((NearestData.bEachNearestDistance[i] < 30) &&
//		   (NearestData.bEachNearestDistance[i] > 0) )
//		{
//			Cmd.bDistance[i] = 1;
//		}
//		else if(NearestData.bEachNearestDistance[i]<MAX_DETECT_DISTANCE)
//		{
//			Cmd.bDistance[i] = (NearestData.bEachNearestDistance[i]-20)/10;
//		}
//		else if(NearestData.bEachNearestDistance[i] == MAX_DETECT_DISTANCE)
//		{
//			Cmd.bDistance[i] = 0x0C;
//		}
//		else
//		{
//			Cmd.bDistance[i] = 0;
//		}
//
//		if(Cmd.bDistance[i] == 0)		//set the orient
//		{
//			Cmd.bObsorient[i] = 0;
//		}
//		else
//		{
//			Cmd.bObsorient[i] = 1;
//		}
//	}

//	if(Cmd.bUsedSnrNUM  == 5)
//	{
//		if(Cmd.bDistance[ID_RL] > 5)
//		{
//			Cmd.bDistance[ID_RL] = 0;
//		}
//		if(Cmd.bDistance[ID_RML] > 9)
//		{
//			Cmd.bDistance[ID_RML] = 0;
//		}
//		/*
//		if(Cmd.bDistance[ID_RR] > 5)
//		{
//			Cmd.bDistance[ID_RR] = 0;
//		}
//		if(Cmd.bDistance[ID_FL] > 5)
//		{
//			Cmd.bDistance[ID_FL] = 0;
//		}
//		if(Cmd.bDistance[ID_FR] > 5)
//		{
//			Cmd.bDistance[ID_FR] = 0;
//		}
//		*/
//		for(i=0;i<ID_SUM;i++)
//		{
//			if(NearestData.bEachNearestDistance[i] > EachSNMaxDistance_5Snr[i])
//			{
//				NearestData.bEachNearestDistance[i] = 0xFF;
//			}
//			if(NearestDis > NearestData.bEachNearestDistance[i])
//			{
//				NearestDis = NearestData.bEachNearestDistance[i];
//			}
//		}
//		if(NearestDis > MAX_DETECT_DISTANCE)			//renew the buzzer information
//		{
//			Cmd.bBuzzerInfo = 0;
//		}
//		else if(NearestDis >= MID_DETECT_DISTANCE)
//		{
//			Cmd.bBuzzerInfo = 2;
//		}
//		else if(NearestDis >= CONTINUOUS_CHIME_DISTANCE)
//		{
//			Cmd.bBuzzerInfo = 4;
//		}
//		else if(NearestDis >= 0)
//		{
//			Cmd.bBuzzerInfo = 7;
//		}
//	}
	if(Cmd.bUsedSnrNUM  == 2)
	{
		//if(Cmd.bDistance[ID_RL] > 0x0C)
		//{
		//	Cmd.bDistance[ID_RL] = 0;
		//}
		//if(Cmd.bDistance[ID_RML] > 0x0C)
		//{
		//	Cmd.bDistance[ID_RML] = 0;
		//}
		/*
		if(Cmd.bDistance[ID_RR] > 7)
		{
			Cmd.bDistance[ID_RR] = 0;
		}
		Cmd.bDistance[ID_FL] = 0;
		Cmd.bDistance[ID_FR] = 0;
		*/
		for(i=0;i<ID_SUM;i++)
		{
			if(NearestData.bEachNearestDistance[i] > EachSNMaxDistance_2Snr[i])
			{
				NearestData.bEachNearestDistance[i] = 0xFF;
			}

			if(NearestDis > NearestData.bEachNearestDistance[i])
			{
				NearestDis = NearestData.bEachNearestDistance[i];
			}
		}
		if(NearestDis > SN_DISTANCE_2Snr_RML)			//renew the buzzer information
		{
			Cmd.bBuzzerInfo = 3;
		}
		else if(NearestDis >= MID_DETECT_DISTANCE)
		{
			Cmd.bBuzzerInfo = 2;
		}
		else if(NearestDis >= CONTINUOUS_CHIME_DISTANCE)
		{
			Cmd.bBuzzerInfo = 1;
		}
		else if(NearestDis >= 0)
		{
			Cmd.bBuzzerInfo = 0;
		}
	}
}

void Cmd_NextSensorSelf(void)
{
    Cmd.CmdStatus = MST_DRV_SENSOR_SELFCHECK;
    Cmd.bID++;
    if(Cmd.bID == ID_MASTER)Cmd.bID++;
    if(Cmd.bID == ID_SUM)
    {
    	Cmd.bID = ID_RL;
    	Cmd.CmdStatus = MST_SEND_SELF_TO_BCM;
    }
}

void Cmd_NextSensorDet(void)
{

	Cmd.bID = (Cmd.bID+1)%ID_SUM;
	if(Cmd.bID == ID_MASTER)
	{
		DetInfo.bDetTriggerFlag = TRUE;
	} else {
		CommunicaitonMode(COM_Internal);
		Cmd.CmdStatus = MST_DRV_SENSOR;
	}
}

void Cmd_ClearBuffer(void)
{
    BYTE i;
    for(i=0; i<SN_NUM; i++)
    {
          NearestData.bEachNearestDistance[i] = 0xFF;
    }
}

void Cmd_SendReqVechiModetoBCM(void)
{
	BYTE i;

	UART0_ModeDataL();
	UART0_INTRX_Dis();
	Delay1us(START_UART_DELAY);

	for(i=0;i<5;i++)
	{
		UART0_WriteByte(MsgRadar_ReqMode._Word[i]);
	}
	Delay1us(END_UART_DELAY);
	UART0_INTRX_En();
	UART0_ModeDataH();
}

void Cmd_SendSelfChecktoBCM(void)
{
	BYTE i,eve_temp;
    WORD checksum_temp;

    checksum_temp = CalcFrameChecksum(UART0_FRM_SENDSELF);
    MsgRadar_Self._Byte[3] = checksum_temp;
	for(i=0;i<4;i++)
	{
		eve_temp = CalcByteEVEN(MsgRadar_Self._Byte[i]);
		MsgRadar_Self._Byte[i] |= (WORD)((eve_temp & 0x01) << 8);
	}

	//UART0_ModeDataL();
	//UART0_INTRX_Dis();
	//Delay1us(START_UART_DELAY);

    //for(i=0;i<5;i++)
    //{
    //	UART0_WriteByte(MsgRadar_Self._Byte[i]);
    //}

    //Delay1us(END_UART_DELAY);
//	UART0_INTRX_En();    //自检信息不接受MP5的反馈信息
	//UART0_ModeDataH();
	SendData_SelfCheck();
}

void Cmd_SendDistancetoBCM(void)
{
/*
	BYTE i,eve_temp;
    WORD checksum_temp;
//TEST
//	MsgRadar_Distance._bit.RPADetectionRL = 1;
//	MsgRadar_Distance._bit.RPADetectionRML = 3;
//	MsgRadar_Distance._bit.RPADetectionRR = 9;
//	MsgRadar_Distance._bit.RPADetectionFL = 1;
//	MsgRadar_Distance._bit.RPADetectionFR = 3;
//
//	MsgRadar_Distance._bit.ObsorientRL = 1;
//	MsgRadar_Distance._bit.ObsorientRML = 1;
//	MsgRadar_Distance._bit.ObsorientRR = 1;
//	MsgRadar_Distance._bit.ObsorientFL = 1;
//	MsgRadar_Distance._bit.ObsorientFR = 1;

//	MsgRadar_Distance._bit.RPASpeakerInfo = Cmd.bBuzzerInfo;

    checksum_temp = CalcFrameChecksum(UART0_FRM_DISTANCE);
    MsgRadar_Distance._Byte[7] = checksum_temp;
	for(i=0;i<8;i++)
	{
		MsgRadar_Distance._Byte[i] &= 0x00FF;
		eve_temp = CalcByteEVEN(MsgRadar_Distance._Byte[i]);
		MsgRadar_Distance._Byte[i] |= (WORD)((eve_temp & 0x01) << 8);
	}

	Uart0TX_ALL_Flag = FALSE;
	UART0_ModeDataL();
	UART0_INTRX_Dis();
	//The delay time need to adjust
	Delay1us(START_UART_DELAY);
    for(i=0;i<9;i++)
    {
    	UART0_WriteByte(MsgRadar_Distance._Byte[i]);
    }
*/
	if(b50msFlag == TRUE)
		{
		SendData_Work();
		b50msFlag = FALSE;
		}

	
	//SendData_SelfCheck();
    //Delay1us(END_UART_DELAY);
	//UART0_INTRX_En();
	//UART0_ModeDataH();


}

BYTE Cmd_VechileCfgCheck(void)
{


	return FALSE;
}

#if 0
BYTE Cmd_RenewVechileCfg(void)
{
	if(Uart0RX_OK_Flag == Rec_VechiInfo)
	{
		Cmd.bVechInfoErrCnt = 0;
		Uart0RX_OK_Flag = Rec_ERR;

		if(Cmd.bUsedSnrNUM  == 5)
		{
			if(MsgBCM_Info._bit.ReversGearSts == TRUE)
			{
				Cmd.bSensorRun[ID_RL]   = TRUE;
				Cmd.bSensorRun[ID_RML]  = TRUE;
				Cmd.bSensorRun[ID_RR]   = TRUE;
				Cmd.bSensorRun[ID_FL]   = TRUE;
				Cmd.bSensorRun[ID_FR]   = TRUE;
				return TRUE;
			}
			else if(MsgBCM_Info._bit.ParkingGearSts == TRUE)
			{
				Cmd.bSensorRun[ID_RL]   = FALSE;
				Cmd.bSensorRun[ID_RML]  = FALSE;
				Cmd.bSensorRun[ID_RR]   = FALSE;
				Cmd.bSensorRun[ID_FL]   = FALSE;
				Cmd.bSensorRun[ID_FR]   = FALSE;

				NearestData.bEachNearestDistance[ID_RL]   = 0XFF;
				NearestData.bEachNearestDistance[ID_RML]  = 0XFF;
				NearestData.bEachNearestDistance[ID_RR]   = 0XFF;
				NearestData.bEachNearestDistance[ID_FL]   = 0XFF;
				NearestData.bEachNearestDistance[ID_FR]   = 0XFF;

			}
			else if((MsgBCM_Info._bit.FrontSwitchSts == TRUE) &&
					(MsgBCM_Info._bit.VehicleSpeed   == FALSE))
			{
				Cmd.bSensorRun[ID_RL]   = TRUE;
				Cmd.bSensorRun[ID_RML]  = TRUE;
				Cmd.bSensorRun[ID_RR]   = TRUE;
				Cmd.bSensorRun[ID_FL]   = TRUE;
				Cmd.bSensorRun[ID_FR]   = TRUE;
				return TRUE;

			}
			else
			{
				Cmd.bSensorRun[ID_RL]   = FALSE;
				Cmd.bSensorRun[ID_RML]  = FALSE;
				Cmd.bSensorRun[ID_RR]   = FALSE;
				Cmd.bSensorRun[ID_FL]   = FALSE;
				Cmd.bSensorRun[ID_FR]   = FALSE;

				NearestData.bEachNearestDistance[ID_RL]   = 0XFF;
				NearestData.bEachNearestDistance[ID_RML]  = 0XFF;
				NearestData.bEachNearestDistance[ID_RR]   = 0XFF;
				NearestData.bEachNearestDistance[ID_FL]   = 0XFF;
				NearestData.bEachNearestDistance[ID_FR]   = 0XFF;
			}
		}
		else if(Cmd.bUsedSnrNUM  == 3)
		{
			if(MsgBCM_Info._bit.ReversGearSts == TRUE)
			{
				Cmd.bSensorRun[ID_RL]   = TRUE;
				Cmd.bSensorRun[ID_RML]  = TRUE;
				Cmd.bSensorRun[ID_RR]   = TRUE;
				Cmd.bSensorRun[ID_FL]   = FALSE;
				Cmd.bSensorRun[ID_FR]   = FALSE;
				NearestData.bEachNearestDistance[ID_FL]   = 0XFF;
				NearestData.bEachNearestDistance[ID_FR]   = 0XFF;
				return TRUE;
			}
			else
			{
				Cmd.bSensorRun[ID_RL]   = FALSE;
				Cmd.bSensorRun[ID_RML]  = FALSE;
				Cmd.bSensorRun[ID_RR]   = FALSE;
				Cmd.bSensorRun[ID_FL]   = FALSE;
				Cmd.bSensorRun[ID_FR]   = FALSE;

				NearestData.bEachNearestDistance[ID_RL]   = 0XFF;
				NearestData.bEachNearestDistance[ID_RML]  = 0XFF;
				NearestData.bEachNearestDistance[ID_RR]   = 0XFF;
				NearestData.bEachNearestDistance[ID_FL]   = 0XFF;
				NearestData.bEachNearestDistance[ID_FR]   = 0XFF;
			}
		}
	}
	return FALSE;



	//test
//	if(Uart0RX_OK_Flag == Rec_VechiInfo)
//		{
//			Cmd.bVechInfoErrCnt = 0;
//			Uart0RX_OK_Flag = Rec_ERR;
//			//if(MsgBCM_Info._bit.ReversGearSts == TRUE)
//			{
//				Cmd.bSensorRun[ID_RL]   = TRUE;
//				Cmd.bSensorRun[ID_RML]  = TRUE;
//				Cmd.bSensorRun[ID_RR]   = TRUE;
//				Cmd.bSensorRun[ID_FL]   = TRUE;
//				Cmd.bSensorRun[ID_FR]   = TRUE;
//				return TRUE;
//			}
//		}
//	return FALSE;
}

#endif

















