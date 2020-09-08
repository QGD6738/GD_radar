#include "cpu.h"
#include "Timer.h"
#include "Serial.h"
#include "Uart.h"
#include "Command.h"
#include "DetectObj.h"
#include "Adc.h"
#include "EFM8BB2_FlashPrimitives.h"

stCmd xdata Cmd;

void Cmd_ReadID(void)
{


}


void Cmd_Process(void)
{
	BYTE i;

	if(ADC.Power_OK_Flag == FALSE)
	{
		return;
	}
    if(bSensorCheckFlag == TRUE)
    {
        DetInfo.bDetTriggerFlag = TRUE;
        return;
    }

	switch(Cmd.CmdStatus)
	{
		case MST_SEND_SELF_TO_BCM:
		{
			Cmd_SelfCheckUpdataDistance();
			Cmd_SendDistancetoBCM();
			//Cmd_NextSensorDet(TRUE);
			Cmd.CmdStatus = MST_DRV_SENSOR;
			break;
		}

		case MST_DRV_SENSOR:
		{
//			//TEST
//			DetInfo.bID = ID_RL;
			Cmd_SetChannel();
			DetInfo.bDetCompleteFlag = FALSE;
			DetInfo.bDetTriggerFlag  = TRUE;
			Cmd.wWaitTime = MASTER_WORK_TIME_MAX;
			Cmd.CmdStatus = MST_WAIT_SENSOR_DATA;
			break;
		}

		case MST_WAIT_SENSOR_DATA:
		{
			if(Cmd.wWaitTime > 0)
			{
				if(DetInfo.bDetCompleteFlag)
				{
					Cmd_NextSensorDet(FALSE);
				}
				Cmd.wWaitTime--;
			}
			else
			{
				if(++Cmd.bSensorErrCnt[DetInfo.bID] >= SENSOR_EER_THRE_SENSOR)
				{
					Cmd.bSensorErrCnt[DetInfo.bID] = SENSOR_EER_THRE_SENSOR;
					DiagInfo.bSelfCheckResult[DetInfo.bID] = 1;
					NearestData.bEachNearestDistance[DetInfo.bID] = NO_OBJ;
				}
				Cmd_NextSensorDet(FALSE);
			}
			break;
		}

		case MST_SEND_DISTANCE_TO_BCM:
		{
			Cmd_UpdataDistance();
			Cmd_SendDistancetoBCM();
			Cmd_NextSensorDet(TRUE);
			Cmd.CmdStatus = MST_DRV_SENSOR;
			break;
		}

		case SLV_WAIT_CMD:
		{


			break;
		}

		case SLV_DISTANCE_BACK:
		{

			break;
		}

		case SLV_CALIBRATION:
		{

			break;
		}

		case SLV_CALIBRATION_BACK:
		{

			break;
		}

		default:
		;
	}
}

void Cmd_SaveData(void)
{

}

void Cmd_UpdataDistance(void)
{
	Cmd_DistancetoComInfo();
//	MsgRadar_Distance._bit.RPADetectionRL = Cmd.bDistance[ID_RL];
//	MsgRadar_Distance._bit.RPADetectionRML = Cmd.bDistance[ID_RML];
//	MsgRadar_Distance._bit.RPADetectionRR = Cmd.bDistance[ID_RR];
	if (DiagInfo.bSelfCheckResult[ID_RL] == 1)
	{
		MsgRadar_Distance[1] = 0x27;
	}else{
		MsgRadar_Distance[1] = Cmd.bDistance[ID_RL];
	}
	if (DiagInfo.bSelfCheckResult[ID_RML] == 1)
	{
		MsgRadar_Distance[2] = 0x27;
	}else{
		MsgRadar_Distance[2] = Cmd.bDistance[ID_RML];
	}
	if (DiagInfo.bSelfCheckResult[ID_RR] == 1)
	{
		MsgRadar_Distance[3] = 0x27;
	}else{
		MsgRadar_Distance[3] = Cmd.bDistance[ID_RR];
	}
}
void Cmd_SelfCheckUpdataDistance(void)
{
	//Cmd_DistancetoComInfo();
	if (DiagInfo.bSelfCheckResult[ID_RL] == 1)
	{
		MsgRadar_Distance[1] = 0x27;
	}else{
		MsgRadar_Distance[1] = 0x00;
	}
	if (DiagInfo.bSelfCheckResult[ID_RML] == 1)
	{
		MsgRadar_Distance[2] = 0x27;
	}else{
		MsgRadar_Distance[2] = 0x00;
	}	
	if (DiagInfo.bSelfCheckResult[ID_RR] == 1)
	{
		MsgRadar_Distance[3] = 0x27;
	}else{
		MsgRadar_Distance[3] = 0x00;
	}
}

void Cmd_DistancetoComInfo(void)
{
	BYTE i;
	for(i=0;i<ID_SUM;i++)					//distance value cale to command data
	{
		if(NearestData.bEachNearestDistance[i]<=30)
		{
			Cmd.bDistance[i] = 0x03;
		}
		else if(NearestData.bEachNearestDistance[i] <= MAX_DETECT_DISTANCE)
		{
			Cmd.bDistance[i] = ((NearestData.bEachNearestDistance[i]+5)/10);  //ËÄÉáÎåÈë
		}
		else
		{
			Cmd.bDistance[i] = 0x19;
			//Cmd.bDistance[i] = 0x30;
		}
	}
}

void Cmd_NextSensorSelf(void)
{
//    Cmd.CmdStatus = MST_DRV_SENSOR_SELFCHECK;
//    Cmd.bID++;
//    if(Cmd.bID == ID_MASTER)Cmd.bID++;
//    if(Cmd.bID >= Cmd.bUsedSnrNUM)
//    {
//        Cmd.bID = ID_RL;
//        Cmd.bID = MST_SEND_SELF_TO_BCM;
//    }
}

void Cmd_NextSensorDet(BYTE ForceCutFlag)
{
//	BYTE cnt,sta;
//
	if(ForceCutFlag == TRUE)
	{
		DetInfo.bID++;
		DetInfo.bID = DetInfo.bID % DetInfo.bDetSnNum;
		Cmd.CmdStatus = MST_DRV_SENSOR;
	}
	else
	{
		if(DetInfo.bID == ID_RR)
		{
			Cmd.CmdStatus = MST_SEND_DISTANCE_TO_BCM;
		}
		else
		{
			DetInfo.bID++;
			DetInfo.bID = DetInfo.bID % DetInfo.bDetSnNum;
			Cmd.CmdStatus = MST_DRV_SENSOR;
		}
	}
}

void Cmd_SetChannel(void)
{
//	SW_DIS;
	switch(DetInfo.bID)
	{
		case ID_RL:
		{
			 SW_SET_RL_INPUT;
		}
			break;
		case ID_RML:
		{
			SW_SET_RML_INPUT;
		}
		break;
		case ID_RR:
		{
			SW_SET_RR_INPUT;
		}
		break;
		default:
		{
			SW_SET_RML_INPUT;
			DetInfo.bID = ID_RL;
		}
		break;
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

void Cmd_SendDistancetoBCM(void)
{
	BYTE i,checksum;
    WORD checksum_temp;
    checksum = 0;
    checksum_temp = (WORD)0x0000;

	for(i=1;i<(MsgRadar_Distance_Frame_Len-2);i++)
	{
		checksum_temp += (BYTE)(MsgRadar_Distance[i] & 0xFF);
		if(checksum_temp > 0xFF)
		{
			checksum_temp = checksum_temp-0xFF;
		}
	}
	if(checksum_temp <= 0xFF)
	{
		checksum = 0xFF-checksum_temp;
	}
	MsgRadar_Distance[MsgRadar_Distance_Frame_Len-2] = checksum;
    for(i=0;i<MsgRadar_Distance_Frame_Len;i++)
	{
		UART1_WriteByte(MsgRadar_Distance[i]);
	}
}

BYTE Cmd_VechileCfgCheck(void)
{


	return FALSE;
}


















