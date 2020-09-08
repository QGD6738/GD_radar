
/*********************************************************************************************************************
** FILE NAME: Command.c
**
** PROJECT:  DFSK_F508_RADAR
** CPU:	     EFM8BB21F16G
** COMPILER: SILICON
** -------------------------------------------------------------------------------------------------------------------
** VERSION:  V0.1
** DATE:     2020.08.06
*********************************************************************************************************************/
#include "cpu.h"
#include "Timer.h"
#include "Serial.h"
#include "Uart.h"
#include "Command.h"
#include "DetectObj.h"
#include "Adc.h"
#include "EFM8BB2_FlashPrimitives.h"

stCmd  xdata Cmd;
unVhcCfgSta xdata SnrCfgSta;


void Cmd_ReleaseVechileCfgInfo(BYTE VhCcfg)
{
	switch(VhCcfg)
	{
		case 2:
			SnrCfgSta._Byte  = 0x06;
			EachSNMaxDistance[ID_RL]  = SN_DISTANCE_2Snr_RL;
			EachSNMaxDistance[ID_RML] = SN_DISTANCE_2Snr_RML;
			EachSNMaxDistance[ID_RMR] = SN_DISTANCE_2Snr_RMR;
			EachSNMaxDistance[ID_RR]  = SN_DISTANCE_2Snr_RR;
			EachSNMaxDistance[ID_FL]  = SN_DISTANCE_2Snr_FL;
			EachSNMaxDistance[ID_FR]  = SN_DISTANCE_2Snr_FR;
		break;
		case 3:
			SnrCfgSta._Byte  = 0x0B;
			EachSNMaxDistance[ID_RL]  = SN_DISTANCE_3Snr_RL;
			EachSNMaxDistance[ID_RML] = SN_DISTANCE_3Snr_RML;
			EachSNMaxDistance[ID_RMR] = SN_DISTANCE_3Snr_RMR;
			EachSNMaxDistance[ID_RR]  = SN_DISTANCE_3Snr_RR;
			EachSNMaxDistance[ID_FL]  = SN_DISTANCE_3Snr_FL;
			EachSNMaxDistance[ID_FR]  = SN_DISTANCE_3Snr_FR;
		break;
		case 4:
			SnrCfgSta._Byte  = 0x0F;
			EachSNMaxDistance[ID_RL]  = SN_DISTANCE_4Snr_RL;
			EachSNMaxDistance[ID_RML] = SN_DISTANCE_4Snr_RML;
			EachSNMaxDistance[ID_RMR] = SN_DISTANCE_4Snr_RMR;
			EachSNMaxDistance[ID_RR]  = SN_DISTANCE_4Snr_RR;
			EachSNMaxDistance[ID_FL]  = SN_DISTANCE_4Snr_FL;
			EachSNMaxDistance[ID_FR]  = SN_DISTANCE_4Snr_FR;
		break;
		case 5:
			SnrCfgSta._Byte  = 0x3B;
			EachSNMaxDistance[ID_RL]  = SN_DISTANCE_5Snr_RL;
			EachSNMaxDistance[ID_RML] = SN_DISTANCE_5Snr_RML;
			EachSNMaxDistance[ID_RMR] = SN_DISTANCE_5Snr_RMR;
			EachSNMaxDistance[ID_RR]  = SN_DISTANCE_5Snr_RR;
			EachSNMaxDistance[ID_FL]  = SN_DISTANCE_5Snr_FL;
			EachSNMaxDistance[ID_FR]  = SN_DISTANCE_5Snr_FR;
		break;
		case 6:
			SnrCfgSta._Byte  = 0x3F;
			EachSNMaxDistance[ID_RL]  = SN_DISTANCE_6Snr_RL;
			EachSNMaxDistance[ID_RML] = SN_DISTANCE_6Snr_RML;
			EachSNMaxDistance[ID_RMR] = SN_DISTANCE_6Snr_RMR;
			EachSNMaxDistance[ID_RR]  = SN_DISTANCE_6Snr_RR;
			EachSNMaxDistance[ID_FL]  = SN_DISTANCE_6Snr_FL;
			EachSNMaxDistance[ID_FR]  = SN_DISTANCE_6Snr_FR;
		break;
		default:
			;
	}

}

void Cmd_GetVechileMode(void)
{
	BYTE mode_temp;
	mode_temp = 0;
	Cmd.wWaitTime = 190;

	CommunicationMode(COM_Uart);
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
				if((MsgBCM_Mode._bit.VechiMode&0xF0) == 0x20)
				{
					Cmd.bUsedSnrNUM = 2;
				}
				else if((MsgBCM_Mode._bit.VechiMode&0xF0) == 0x30)
				{
					Cmd.bUsedSnrNUM = 3;
				}
				else if((MsgBCM_Mode._bit.VechiMode&0xF0) == 0x40)
				{
					Cmd.bUsedSnrNUM = 4;
				}
				else if((MsgBCM_Mode._bit.VechiMode&0xF0) == 0x50)
				{
					Cmd.bUsedSnrNUM = 5;
				}
				else if((MsgBCM_Mode._bit.VechiMode&0xF0) == 0x60)
				{
					Cmd.bUsedSnrNUM = 6;
				}
				mode_temp = MsgBCM_Mode._bit.VechiMode;
				Flash_WriteVechileMode(mode_temp);
				Cmd.wWaitTime = 0;
				break;
			}
		}
		CLR_WDT;
	}

	if(Cmd.wWaitTime == 0)
	{
		mode_temp = Flash_ReadVechileMode();
		Cmd.bUsedSnrNUM = (mode_temp >> 4);
		if(Cmd.bUsedSnrNUM < 2 || Cmd.bUsedSnrNUM > 6)
		{
			Cmd.bUsedSnrNUM = 6;
		}
		Cmd_ReleaseVechileCfgInfo(Cmd.bUsedSnrNUM);
	}
	CommunicationMode(NO_Com);
}



void Cmd_Process(void)
{
	BYTE i,self_temp,cnt;

    if(bSensorCheckFlag == TRUE)
    {
        DetInfo.bDetTriggerFlag = TRUE;
        return;
    }

	switch(Cmd.CmdStatus)
		{
			case MST_DRV_SENSOR_SELFCHECK:
			{
				Srl_SendData(HEAD_2000, Cmd.bID, ID_LEN_BIT, 0, 0);
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
				self_temp = 0;
				for(i=0; i<ID_SUM; i++)
				{
					if(((SnrCfgSta._Byte>>i)&0x01) == TRUE)//sensor is config
					{
						if(i < ID_FL)
						{
							if(DiagInfo.bSelfCheckResult[i] == TRUE)
							{
								self_temp |= (0x01<<(3-i));
							}
						} else {

							if(DiagInfo.bSelfCheckResult[i] == TRUE)
							{
								self_temp |= (0x01<<(9-i));
							}
						}
					}
				}


				MsgRadar_Self._Byte[1] = self_temp;

				cnt = 5;
				if( (SnrCfgSta._bit.SN_RL  == TRUE && DiagInfo.bSelfCheckResult[ID_RL]  == TRUE) ||
					(SnrCfgSta._bit.SN_RML == TRUE && DiagInfo.bSelfCheckResult[ID_RML] == TRUE) ||
					(SnrCfgSta._bit.SN_RMR == TRUE && DiagInfo.bSelfCheckResult[ID_RMR] == TRUE) ||
					(SnrCfgSta._bit.SN_RR  == TRUE && DiagInfo.bSelfCheckResult[ID_RR]  == TRUE) ||
					(SnrCfgSta._bit.SN_FL  == TRUE && DiagInfo.bSelfCheckResult[ID_FL]  == TRUE) ||
					(SnrCfgSta._bit.SN_FR  == TRUE && DiagInfo.bSelfCheckResult[ID_FR]  == TRUE)   )
				{
					cnt = 25;
				}
				CommunicationMode(COM_Uart);
				for(i=0;i<cnt;i++)
				{
					Cmd_SendSelfChecktoBCM();
					Delay1ms(10);
				}
				Cmd.CmdStatus = MST_DRV_SENSOR;
				Cmd.bID = ID_RL;
				CommunicationMode(COM_Internal);
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
						    	Cmd.CmdStatus = MST_CUT_SNR;
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
					Cmd.CmdStatus = MST_CUT_SNR;
				}
				break;
			}

			case MST_SEND_DISTANCE_TO_BCM:
			{
				if(Cmd.wWaitTime > 100)
				{
					Cmd_UpdataDistance();
					CommunicationMode(COM_Uart);
					InitUart0ReceiveVar();
					Cmd_SendDistancetoBCM();
					Cmd.wWaitTime = 92;
				}
				else if(Cmd.wWaitTime > 0)
				{

#if(TEST_CODE_VEHICLE == TRUE)
{
	Uart0RX_OK_Flag = Rec_VechiInfo;
	MsgBCM_Info._bit.ReversGearSts = TRUE;
}
#endif
					if(Cmd_RenewVechileCfg() == TRUE)
					{
						Cmd.CmdStatus = MST_CUT_SNR;
						Cmd.wWaitTime = 110;
					}
					Cmd.wWaitTime--;
				}
				else
				{
					if(++Cmd.bVechInfoErrCnt > 5)
					{
						Cmd.bVechInfoErrCnt = SENSOR_EER_THRE_CAR;
						DetObj_InitAllChannelDistance();
						Cmd_UpdataDistance();
						Cmd.bID = ID_RL;
					}
					Cmd.CmdStatus = MST_SEND_DISTANCE_TO_BCM;
					Cmd.wWaitTime = 110;
				}
				break;
			}

			case MST_WAIT_VECHILE_CFG:  //no use
			{
#if(TEST_CODE_VEHICLE == TRUE)
{
	Uart0RX_OK_Flag = Rec_VechiInfo;
	MsgBCM_Info._bit.ReversGearSts = TRUE;
}
#endif
				if(Cmd.wWaitTime > 0)
				{
					if(Cmd_RenewVechileCfg() == TRUE)
					{
						InitUart0ReceiveVar();
						Cmd.CmdStatus = MST_CUT_SNR;
					}
					Cmd.wWaitTime--;
				}
				else
				{
					InitUart0ReceiveVar();
					if(++Cmd.bVechInfoErrCnt > SENSOR_EER_THRE_CAR)
					{
						Cmd.bVechInfoErrCnt = SENSOR_EER_THRE_CAR;
						DetObj_InitAllChannelDistance();
						Cmd_UpdataDistance();
						Cmd.bID = ID_RL;
						Cmd.CmdStatus = MST_SEND_DISTANCE_TO_BCM;
					}
				}
				break;
			}

			case MST_CUT_SNR:
			{
#if(TEST_CODE_VEHICLE == TRUE)
{
	Uart0RX_OK_Flag = Rec_VechiInfo;
	MsgBCM_Info._bit.ReversGearSts = TRUE;
}
#endif
				Cmd_NextSensorDet();

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
							CommunicationMode(NO_Com);
							Cmd.CmdStatus = SLV_DISTANCE_BACK;
						}
					}
				}

				if(Serial.CmdHead_Rx == CMD_HEAD_1500)	//Work frock command head
				{
					if(Serial.bValid == TRUE)
					{
						Serial.bValid = FALSE;
						CommunicationMode(COM_Internal);
						Cmd.CmdStatus = SLV_CALIBRATION;
					}
				}

				if(Serial.CmdHead_Rx == CMD_HEAD_2000)    //Self check command head
				{
					if(Serial.bValid == TRUE)
					{
						Serial.bValid = FALSE;
						if((Serial.byData&0x07) == DetInfo.bID)
						{
							Cmd.wWaitTime = SINGLE_SENSOR_TIME_OVER_MAX;
							DetInfo.bDetTriggerFlag = FALSE;
							CommunicationMode(COM_Internal);
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

				Cmd.wBackData =(((WORD)NearestData.bNearestDistance)|
						        ((WORD)DiagInfo.bSelfCheckResult[DetInfo.bID]<<8)|
								((WORD)DiagInfo.bKeepFlag<<9));
				Srl_SendData(HEAD_1000, DetInfo.bID, ID_LEN_BIT, Cmd.wBackData, 10);
				CommunicationMode(COM_Internal);
				Cmd.CmdStatus = SLV_WAIT_CMD;
				break;
			}

			case SLV_CALIBRATION:
			{
				if(Serial.CmdHead_Rx == CMD_HEAD_1500)
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
				Srl_SendData(HEAD_1500, Cmd.CmdCalibration, TEST_CMD_LEN_BIT, Cmd.wBackData, 8);
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
					if(Cmd_RenewVechileCfg() == TRUE)
					{
						Cmd.CmdStatus = DEBUG_MODE;
						Cmd.wWaitTime = 1;
					}
					Cmd.wWaitTime--;
				}
				else
				{
					DetObj_InitAllChannelDistance();
					Cmd_UpdataDistance();
					CommunicationMode(COM_Uart);
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

	MsgRadar_Distance._bit.RPADetectionRL  = Cmd.bDistance[ID_RL];
	MsgRadar_Distance._bit.RPADetectionRML = Cmd.bDistance[ID_RML];
	MsgRadar_Distance._bit.RPADetectionRMR = Cmd.bDistance[ID_RMR];
	MsgRadar_Distance._bit.RPADetectionRR  = Cmd.bDistance[ID_RR];
	MsgRadar_Distance._bit.RPADetectionFL  = Cmd.bDistance[ID_FL];
	MsgRadar_Distance._bit.RPADetectionFR  = Cmd.bDistance[ID_FR];

	MsgRadar_Distance._bit.ObsorientRL  = Cmd.bObsorient[ID_RL];
	MsgRadar_Distance._bit.ObsorientRML = Cmd.bObsorient[ID_RML];
	MsgRadar_Distance._bit.ObsorientRMR = Cmd.bObsorient[ID_RMR];
	MsgRadar_Distance._bit.ObsorientRR  = Cmd.bObsorient[ID_RR];
	MsgRadar_Distance._bit.ObsorientFL  = Cmd.bObsorient[ID_FL];
	MsgRadar_Distance._bit.ObsorientFR  = Cmd.bObsorient[ID_FR];
	MsgRadar_Distance._bit.Reserve = 0x00;

	MsgRadar_Distance._bit.RPASpeakerInfo = Cmd.bBuzzerInfo;
}

void Cmd_DistancetoComInfo(void)
{
	BYTE i;
	BYTE NearestDis;
	NearestDis = 0xFF;

	for(i=0;i<ID_SUM;i++)					//distance value cale to command data
	{
		if(NearestData.bEachNearestDistance[i] > EachSNMaxDistance[i])
		{
			NearestData.bEachNearestDistance[i] = 0xFF;
		}
		if(NearestDis > NearestData.bEachNearestDistance[i])
		{
			NearestDis = NearestData.bEachNearestDistance[i];
		}

		if((NearestData.bEachNearestDistance[i] < 30) &&
		   (NearestData.bEachNearestDistance[i] > 0) )
		{
			Cmd.bDistance[i] = 1;
		}
		else if(NearestData.bEachNearestDistance[i]<MAX_DETECT_DISTANCE)
		{
			Cmd.bDistance[i] = (NearestData.bEachNearestDistance[i]-20)/10;
		}
		else if(NearestData.bEachNearestDistance[i] == MAX_DETECT_DISTANCE)
		{
			Cmd.bDistance[i] = 0x0C;
		}
		else
		{
			Cmd.bDistance[i] = 0;
		}

		if(Cmd.bDistance[i] == 0)		//set the orient
		{
			Cmd.bObsorient[i] = 0;
		}
		else
		{
			Cmd.bObsorient[i] = 1;
		}

	}

	if(NearestDis > MAX_DETECT_DISTANCE)			//renew the buzzer information
	{
		Cmd.bBuzzerInfo = 0;
	}
	else if(NearestDis >= MID_DETECT_DISTANCE)
	{
		Cmd.bBuzzerInfo = 2;
	}
	else if(NearestDis >= CONTINUOUS_CHIME_DISTANCE)
	{
		Cmd.bBuzzerInfo = 4;
	}
	else if(NearestDis >= 0)
	{
		Cmd.bBuzzerInfo = 7;
	}
}

void Cmd_NextSensorSelf(void)
{
    Cmd.CmdStatus = MST_DRV_SENSOR_SELFCHECK;

	for( ; Cmd.bID<ID_SUM; )
	{
		Cmd.bID++;
		if(Cmd.bID == ID_MASTER)Cmd.bID++;

		if(((SnrCfgSta._Byte>>Cmd.bID) & 0x01) == TRUE)
		{
			break;
		}
	}

    if(Cmd.bID >= ID_SUM)
    {
        Cmd.bID = ID_RL;
        Cmd.CmdStatus = MST_SEND_SELF_TO_BCM;
    }
}

void Cmd_NextSensorDet(void)
{
	static BYTE SendToBcmFlag = FALSE;

	//TEST
/*	Cmd.bSensorRun[0] = TRUE;
	Cmd.bSensorRun[1] = TRUE;
	Cmd.bSensorRun[2] = TRUE;
	Cmd.bSensorRun[3] = TRUE;
	Cmd.bSensorRun[4] = TRUE;
	Cmd.bSensorRun[5] = TRUE;*/


	if((SendToBcmFlag == FALSE )&&
	   ((Cmd.bID == ID_RR) || (Cmd.bID == ID_FR)))
	//if(0)
	{
		SendToBcmFlag = TRUE;
		Cmd.wWaitTime = 110;
		CommunicationMode(COM_Uart);
		Cmd.CmdStatus = MST_SEND_DISTANCE_TO_BCM;
	}
	else
	{
		SendToBcmFlag = FALSE;
		Cmd.bID = (Cmd.bID+1)%ID_SUM;
		if(Cmd.bSensorRun[Cmd.bID] == TRUE)
		{
			if(Cmd.bID == ID_MASTER)//RML
			{
				DetInfo.bDetTriggerFlag = TRUE;
			}
			else{
				CommunicationMode(COM_Internal);
				Cmd.CmdStatus = MST_DRV_SENSOR;
			}
		}
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

	UART0_ModeDataL();
	UART0_INTRX_Dis();
	Delay1us(START_UART_DELAY);

    for(i=0;i<5;i++)
    {
    	UART0_WriteByte(MsgRadar_Self._Byte[i]);
    }

    Delay1us(END_UART_DELAY);
	UART0_ModeDataH();
}

void Cmd_SendDistancetoBCM(void)
{
	BYTE i,eve_temp;
    WORD checksum_temp;

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
    Delay1us(END_UART_DELAY);
	UART0_INTRX_En();
	UART0_ModeDataH();


}

BYTE Cmd_VechileCfgCheck(void)
{


	return FALSE;
}


BYTE Cmd_RenewVechileCfg(void)
{
	/* test */
	//Uart0RX_OK_Flag = Rec_VechiInfo;
	//MsgBCM_Info._bit.ReversGearSts = TRUE;

	if(Uart0RX_OK_Flag == Rec_VechiInfo)
	{
		Cmd.bVechInfoErrCnt = 0;
		Uart0RX_OK_Flag = Rec_ERR;

		if(MsgBCM_Info._bit.ReversGearSts == TRUE)
		{
			if(SnrCfgSta._bit.SN_RL  == TRUE)	Cmd.bSensorRun[ID_RL]   = TRUE;
			if(SnrCfgSta._bit.SN_RML == TRUE)	Cmd.bSensorRun[ID_RML]  = TRUE;
			if(SnrCfgSta._bit.SN_RMR == TRUE)	Cmd.bSensorRun[ID_RMR]  = TRUE;
			if(SnrCfgSta._bit.SN_RR  == TRUE)	Cmd.bSensorRun[ID_RR]   = TRUE;
			if(SnrCfgSta._bit.SN_FL  == TRUE)	Cmd.bSensorRun[ID_FL]   = TRUE;
			if(SnrCfgSta._bit.SN_FR  == TRUE)	Cmd.bSensorRun[ID_FR]   = TRUE;
			return TRUE;
		}
		else if(MsgBCM_Info._bit.ParkingGearSts == TRUE)
		{
			Cmd.bSensorRun[ID_RL]   = FALSE;
			Cmd.bSensorRun[ID_RML]  = FALSE;
			Cmd.bSensorRun[ID_RMR]  = FALSE;
			Cmd.bSensorRun[ID_RR]   = FALSE;
			Cmd.bSensorRun[ID_FL]   = FALSE;
			Cmd.bSensorRun[ID_FR]   = FALSE;

			NearestData.bEachNearestDistance[ID_RL]   = 0XFF;
			NearestData.bEachNearestDistance[ID_RML]  = 0XFF;
			NearestData.bEachNearestDistance[ID_RMR]  = 0XFF;
			NearestData.bEachNearestDistance[ID_RR]   = 0XFF;
			NearestData.bEachNearestDistance[ID_FL]   = 0XFF;
			NearestData.bEachNearestDistance[ID_FR]   = 0XFF;

		}
		else if((MsgBCM_Info._bit.FrontSwitchSts == TRUE) &&
				(MsgBCM_Info._bit.VehicleSpeed   == FALSE) &&
				(Cmd.bUsedSnrNUM == 5 || Cmd.bUsedSnrNUM == 6))
		{
			if(SnrCfgSta._bit.SN_RL  == TRUE)	Cmd.bSensorRun[ID_RL]   = TRUE;
			if(SnrCfgSta._bit.SN_RML == TRUE)	Cmd.bSensorRun[ID_RML]  = TRUE;
			if(SnrCfgSta._bit.SN_RMR == TRUE)	Cmd.bSensorRun[ID_RMR]  = TRUE;
			if(SnrCfgSta._bit.SN_RR  == TRUE)	Cmd.bSensorRun[ID_RR]   = TRUE;
			if(SnrCfgSta._bit.SN_FL  == TRUE)	Cmd.bSensorRun[ID_FL]   = TRUE;
			if(SnrCfgSta._bit.SN_FR  == TRUE)	Cmd.bSensorRun[ID_FR]   = TRUE;
			return TRUE;

		}
		else
		{
			Cmd.bSensorRun[ID_RL]   = FALSE;
			Cmd.bSensorRun[ID_RML]  = FALSE;
			Cmd.bSensorRun[ID_RMR]  = FALSE;
			Cmd.bSensorRun[ID_RR]   = FALSE;
			Cmd.bSensorRun[ID_FL]   = FALSE;
			Cmd.bSensorRun[ID_FR]   = FALSE;

			NearestData.bEachNearestDistance[ID_RL]   = 0XFF;
			NearestData.bEachNearestDistance[ID_RML]  = 0XFF;
			NearestData.bEachNearestDistance[ID_RMR]  = 0XFF;
			NearestData.bEachNearestDistance[ID_RR]   = 0XFF;
			NearestData.bEachNearestDistance[ID_FL]   = 0XFF;
			NearestData.bEachNearestDistance[ID_FR]   = 0XFF;
		}
	}
	return FALSE;

}



















