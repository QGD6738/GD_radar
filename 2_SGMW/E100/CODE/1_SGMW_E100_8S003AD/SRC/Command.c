#include "Serial.h"
#include "Command.h"
#include "ReadID.h"
#include "Timer.h"
#include "DetectObj.h"
#include "speaker.h"

stCmd Cmd;
UNION_VECHILEINFO VechInfo;

void Cmd_ClearBuffer(void)
{
    BYTE i;
    
    for(i=0; i<SN_NUM; i++)
    {
          NearestData.bEachNearestDistance[i] = 0xFF;
    }
}


void Cmd_DetNextSensor(BYTE ForceCutFlag)
{
//    BYTE CNT;
//    BYTE STA;
    
    Cmd.Cursnr++;
    Cmd.Cursnr = Cmd.Cursnr%USED_SN_NUM;
    if(Cmd.Cursnr == MASTER_SENSOR)
    {
        DetInfo.bDetTriggerFlag = TRUE;
    }
    Cmd.CurSta = CMD_DRIVE_SENSOR_WORK;
}

BYTE Cmd_WorkDataCheckSum(void)
{
    BYTE i;
    BYTE checksum = 0;
    BYTE ret = FALSE;
    
    for(i=1; i<11;i++)
    {
        if(Srl.dData&((DWORD)0x000000001<<i))
        {
            checksum++;
        }
    }
    if(checksum%2 == (Srl.dData&0x01))
      ret = TRUE;
    
    return ret;
}

BYTE Cmd_CarDataCheckSum(void)
{
    BYTE i;
    BYTE checksum = 0;
    BYTE ret = FALSE;
    
    for(i=0; i<5;i++)
    {
        if(Srl.dData&((DWORD)0x000000001<<i))
        {
            checksum++;
        }
    }
    if(checksum%2 == ((Srl.dData>>5)&0x01))
      ret = TRUE;
    
    return ret;
}

BYTE Cmd_SetCmdCheckSum(void)
{
    BYTE checksum = 0;
    BYTE ret = FALSE;
    
    checksum = (BYTE)(Srl.dData>>4);
    checksum = (((checksum&0x05)+3)&0x0f);
    
    if(checksum == (Srl.dData&0x0f))
      ret = TRUE;
    return ret;
}


void Cmd_NextSensorSelf(void)
{
    Cmd.CurSta = CMD_DRIVE_SENSOR_SELF;
    Cmd.Cursnr++;
    if(Cmd.Cursnr == MASTER_SENSOR)Cmd.Cursnr++;
    if(Cmd.Cursnr >= USED_SN_NUM)
    {
        Cmd.Cursnr = ID_RL;
        Cmd.CurSta = CMD_SEND_SELF_DATA_TO_CAR;
        Cmd.SpkState = Spk_Self_Type;
    }
}

void Cmd_ReturnSelf(void)
{
    BYTE i,j,checksum1;

    checksum1 = 0;
    Cmd.dTxData = 0;
    for(i=0; i<USED_SN_NUM;i++)
    {
       if(Cmd.bSelfCheckResult[i] != SELF_SENSOR_OK)
       {
          Cmd.dTxData |= (DWORD)(0x00000001<<i);
          checksum1++;
       }
    }
    if(checksum1 == USED_SN_NUM)
        Cmd.dTxData |= (DWORD)(0x00000001<<USED_SN_NUM);
    if(checksum1%2)
        Cmd.dTxData |= (DWORD)(0x00000001<<(USED_SN_NUM+1));
    if(checksum1 == 0)
        j = 5;
    else
        j = 5;
    for(i=0;i<j;i++)
    {
        SendData(&Cmd.dTxData, (USED_SN_NUM+2), HEAD_RETURN_SELF);
        Delay1us(30000);
        Delay1us(50000);
    }
}

void Cmd_ReturnDataToCar(void)
{
  
}

void Cmd_ReturnForCmd(BYTE Value, EtReqSta REQ_STA)
{
    BYTE i,CheckSum = 0;
    Cmd.dTxData = 0;
    
    Cmd.dTxData  = ((WORD)Value<<3)|(BYTE)(REQ_STA<<1);
    for(i=1; i<11; i++)
    {
        if((Cmd.dTxData>>i) & (DWORD)0x00000001)
        {
            CheckSum++;
        }
    }
    if(CheckSum%2)
    {
        Cmd.dTxData |= 0x01;
    }
    SendData(&Cmd.dTxData, 11, HEAD_SENSOR_BACK);
    Serial_ISP = &GetHeader;
    EnableInternalCommunicaiton();
}

void Cmd_Send(BYTE cmd)
{
    Srl.bBackFlag = E_HEAD_SENSOR_BACK;
    Cmd.dTxData = (Cmd.Cursnr<<3)|cmd;
    SendData(&Cmd.dTxData, 6, HEAD_SENSOR_DRIVE);
    Serial_ISP = &GetHeader;
    EnableInternalCommunicaiton();
    Cmd.bTimeOutCnt = TIMEOUT_THRE_SENSOR;
}

void Cmd_DetectObj (void)
{
    static BYTE ErrCnt = 0;
    
    if(bSensorCheckFlag == TRUE)
    {
        DetInfo.bDetTriggerFlag = TRUE; 
        return;
    }
    
    switch(Cmd.CurSta)
    {
      case CMD_DRIVE_SENSOR_SELF:
          Cmd_Send(REQ_ID_SELF_DATA);
          Cmd.CurSta = CMD_WAIT_SENSOR_SELF;
      break;
      
      case CMD_WAIT_SENSOR_SELF:
          if(Cmd.bTimeOutCnt > 0)
          {
              Cmd.bTimeOutCnt--;
              if(Srl.bValid == TRUE)
              {
                  Srl.bValid = FALSE;
                  ErrCnt = 0;
                  if((Srl.dData>>8) == Cmd.Cursnr)
                  {
                      Cmd.bSelfCheckResult[Cmd.Cursnr] = (Srl.dData>>3)&0x01F;
                  }
                  Cmd_NextSensorSelf();
              }
          }
          else
          {
              Cmd.CurSta = CMD_DRIVE_SENSOR_SELF;
              if(++ErrCnt>=2)
              {
                  ErrCnt = 0;
                  Cmd.bSelfCheckResult[Cmd.Cursnr] = SELF_SENSOR_NO_CONNECT;
                  Cmd_NextSensorSelf();
              }
          }
      break;
      
      case CMD_DRIVE_SENSOR_WORK:
          //Cmd.Cursnr = ID_FR;
          Cmd_Send(REQ_DISTANCE);
          Cmd.CurSta = CMD_WAIT_SENSOR_DATA;
      break;
          
      case CMD_WAIT_SENSOR_DATA:
          if(Cmd.bTimeOutCnt > 0)
          {
              Cmd.bTimeOutCnt--;
              if(Srl.bValid == TRUE)
              {
                  Srl.bValid = FALSE;
                  if(Cmd_WorkDataCheckSum() == TRUE)
                  {
                      Cmd.bSensorErrCnt[Cmd.Cursnr] = 0;
                      NearestData.bEachNearestDistance[Cmd.Cursnr] = (BYTE)((Srl.dData>>3)&0xFF);
                  }
                  Cmd_DetNextSensor(FALSE);
              }
          }
          else
          {
              if(++Cmd.bSensorErrCnt[Cmd.Cursnr] >= SENSOR_EER_THRE_SENSOR)
              {
                  Cmd.bSensorErrCnt[Cmd.Cursnr] = SENSOR_EER_THRE_SENSOR;
                  /*增加错误探头间隔周期扫描*/
                  NearestData.bEachNearestDistance[Cmd.Cursnr] = 0xFF;
              }
              Cmd_DetNextSensor(FALSE);
          }
      break;
      
      case CMD_SEND_SELF_DATA_TO_CAR:      //自检报警
          if(Cmd.SpkState == Spk_Self_Type)
          {
            Cmd.bSelfCheckResult[MASTER_SENSOR] =  DiagInfo.bSelfCheckResult[MASTER_SENSOR];
            Spk_JudgeThePowerUpChimeType();
            Cmd.SpkState = Spk_Self_Chime;
          }
          if(Cmd.SpkState == Spk_Self_Chime)
          {
              if(DiagInfo.wPowerUpChimeTime > 0)
              {
                 DiagInfo.wPowerUpChimeTime--; 
                 Spk_PowerUpChime();                         /* Chime the speaker. */
              }
              else
              {
                  STOP_THE_SPEAKER;
                  SpkInfo.bSoundDistance = NO_OBJ-1;
                  Cmd.SpkState = Spk_Det;
              }
          }
          if(Cmd.SpkState == Spk_Det)
          {
            Cmd.CurSta = CMD_DRIVE_SENSOR_WORK;
          }
      break;
      
      case CMD_SEND_DISTANCE_TO_CAR:

      break;
      
      case CMD_WAIT_VEHICLE_CONFIG:

      break;
      
      case CMD_STOP:
          if(Cmd.bTimeOutCnt > 0)
          {
              Cmd.bTimeOutCnt--;
              if(Srl.bValid == TRUE)
              {
                  Srl.bValid = FALSE;
                  if(Cmd_CarDataCheckSum() == TRUE)
                  {
                      Cmd.bVechInfoErrCnt = 0;
                      VechInfo.VechicleData = (BYTE)((Srl.dData>>1)&0x1F);
                      Cmd_RenewalVehicleInfo();
                      Cmd_DetNextSensor(FALSE);
                  }
              }
          }
          else
          {
              Cmd_ReturnDataToCar();
              Cmd.bTimeOutCnt = SENSOR_STOP_SCAN_TIME;
          }
      break;
      
      case CMD_WAIT_REQ:
          if(Srl.bValid == TRUE)
          {
              Srl.bValid = FALSE;
              if((BYTE)((Srl.dData>>3)&0x07) == DetInfo.bID)
              {
                  EtCmdSta STA;
                  BYTE temp = 0;
                  STA = (EtCmdSta)(Srl.dData&0x07);
                  switch(STA)
                  {
                    case REQ_DISTANCE:
                      DetInfo.bDetTriggerFlag = TRUE;
                    break;
                    case REQ_RINGTIME:
                      if(ClrtData.wRingTime > 2500)
                         ClrtData.wRingTime = 2500;
                      temp = ClrtData.wRingTime/10;
                      Cmd_ReturnForCmd(temp,REQ_STA_FAULT);
                    break;
                    case REQ_ID_SELF_DATA:
                      temp = DetInfo.bID<<5;
                      temp |= DiagInfo.bSelfCheckResult[DetInfo.bID];
                      Cmd_ReturnForCmd(temp,REQ_STA_FAULT);
                    break;
                    case REQ_ECHO_WIDE:
                      temp = ClrtData.wTimeWidth/10;
                      Cmd_ReturnForCmd(temp,REQ_STA_FAULT);
                    break;
                    case REQ_ECHO_HEIGHT:
                      Cmd_ReturnForCmd(ClrtData.bVolHeight,REQ_STA_FAULT);
                    break;
                    case REQ_EEPROM_SAMP:
                      Cmd_ReturnForCmd(0x00,REQ_STA_FAULT);
                    break;
                    case REQ_HANDSHAKE:
                      Cmd_ReturnForCmd(VEHICLE_TYPE,REQ_STA_FAULT);
                    break;
                    case REQ_RESERVED2:
                    break;
                  }
              }
          }
      break;
      case CMD_SET:
//        if(DetInfo.bID == ID_RMR)
//            Cmd.CurSta = CMD_DRIVE_SENSOR_WORK; 
//        else
//            Cmd.CurSta = CMD_WAIT_REQ;
      break;
      default:;
//        if(DetInfo.bID == ID_RMR)
//            Cmd.CurSta = CMD_DRIVE_SENSOR_WORK; 
//        else
//            Cmd.CurSta = CMD_WAIT_REQ;
    }
}

BYTE Cmd_RenewalVehicleInfo(void)
{
    return FALSE;
}