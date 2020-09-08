#include "cpu.h"
/*********************************************************************************************************************
** FILE NAME: Adc.c
**
** PROJECT:  长城H2倒车雷达
** CPU:	     EFM8BB21F16G
** COMPILER: 芯科
** -------------------------------------------------------------------------------------------------------------------
** VERSION:  V1.0
** DATE:     2018.12.24
*********************************************************************************************************************/

#include "ADC.h"
#include "Command.h"
#include "Timer.h"
#include "DetectObj.h"
#include "Serial.h"
#include "Timer.h"

BYTE 	xdata   bySampleTimes;  //Remainning sample times
WORD 	xdata   wSampleValue;   //AD value
WORD 	xdata   wPowerValue;    //Power value
BOOL    xdata	bRadar_PoweFlag;
BOOL 	xdata   bWorkFlag;      //Work flag
BOOL 	xdata   bDetectFlag;    //Detect flag
stADC   xdata   ADC;

WORD RL_MIN_VALUE;      // GND
WORD RL_MAX_VALUE;

WORD RML_MIN_VALUE;	// +12V
WORD RML_MAX_VALUE;

WORD RR_MIN_VALUE;	// NC
WORD RR_MAX_VALUE;

WORD FL_MIN_VALUE;	// GND
WORD FL_MAX_VALUE;

WORD FR_MIN_VALUE;	// NC
WORD FR_MAX_VALUE;

void ADC_InitVariable(void)
{
	bySampleTimes = 0;
	wSampleValue  = 0;
	wPowerValue   = 0;
	bWorkFlag     = TRUE;
	bDetectFlag	  = 0;
}
/*********************************************************************************************************************
** Init ADC.
*********************************************************************************************************************/
void ADC_PowerSamplingInit(void)
{
	ADC0CN0_ADBMEN 	= 0; 	//Disable Burst Mode
	ADC0CN0_ADCM0	= 0;	//ADC0 conversion initiated is software triggerr
	ADC0CN0_ADCM1	= 0;
	ADC0CN1 		= 0x01;	//Common Mode Buffer Enable
	ADC0CF			= 0x19;	/*bit7/6/5/4/3 = 00011 is set clock,49/(3+1) = 12.25mhz
							  bit2 = 0 is set 10 bit mode
							  bit1 = 0 Selects between Normal Tracking Modes
							  bit1 = 1 Gain Control set to 1 or 0.5
							*/
	ADC0AC			= 0x42; /*bit7 = 0 Disable 12-bit mode
							  bit6 = 1 ADC0H:ADC0L contain the accumulated conversion results when Burst Mode is disabled. Firmware must write 0x0000 to ADC0H:ADC0L to clear the accumulated result.
							  bit5/4/3 = 000 Right justified. No shifting applied
							  bit2/1/0 = 010 Perform and Accumulate 8 conversions (2 conversions in 12-bit mode).
							*/
	ADC0PWR			= 0x40; /*bit7/6 = 01 Select bias current mode 1 (SARCLK <= 16 MHz).
							  bit5 = 0 Low power mode disabled
							  bit4 = 0 Disable low power mode
							  bit2/1/0 = 000 This field sets the time delay allowed for the ADC to power up from a low power state. When ADTM is set, an additional 4SARCLKs are added to this time
							*/
	ADC0TK			= 0x00; //no use
	ADC0GTH			= 0xFF;
	ADC0GTL			= 0xFF;
	ADC0LTH			= 0x00;
	ADC0LTL			= 0x00;
	ADC0MX			= 0x03; //Selects the channel P0.0-P1.6 == 00000-01110
	REF0CN			= 0x08; /*bit7 = 0 The internal reference operates at 1.65 V nominal; 1 is 2.4v
							  bit6 = 0 Reserved
							  bit5 = 0 The ADC0 ground reference is the GND pin
							  bit4/3 = 01 The ADC0 voltage reference is the VDD pin
							  bit2 = 0 Disable the Temperature Sensor
							  bit1/0 = 00 Reserved
							*/
	ADC0 			= 0x0000;	//Clear the accumulated conversion results
	ADC0CN0_ADEN 	= 1; 		//ADC Enable
	EIE1		   &= (~0x08); 	//Disable interrupt

}

void ADC_IDSamplingInit (void)
{
	ADC0CN0_ADBMEN 	= 0; 	//Disable Burst Mode
	ADC0CN0_ADCM0	= 0;	//ADC0 conversion initiated is software triggerr
	ADC0CN0_ADCM1	= 0;
	ADC0CN1 		= 0x01;	//Common Mode Buffer Enable
	ADC0CF			= 0x19;	/*bit7/6/5/4/3 = 00011 is set clock,49/(3+1) = 12.25mhz
							  bit2 = 0 is set 10 bit mode
							  bit1 = 0 Selects between Normal Tracking Modes
							  bit1 = 1 Gain Control set to 1 or 0.5
							*/
	ADC0AC			= 0x42; /*bit7 = 0 Disable 12-bit mode
							  bit6 = 1 ADC0H:ADC0L contain the accumulated conversion results when Burst Mode is disabled. Firmware must write 0x0000 to ADC0H:ADC0L to clear the accumulated result.
							  bit5/4/3 = 000 Right justified. No shifting applied
							  bit2/1/0 = 010 Perform and Accumulate 8 conversions (2 conversions in 12-bit mode).
							*/
	ADC0PWR			= 0x40; /*bit7/6 = 01 Select bias current mode 1 (SARCLK <= 16 MHz).
							  bit5 = 0 Low power mode disabled
							  bit4 = 0 Disable low power mode
							  bit2/1/0 = 000 This field sets the time delay allowed for the ADC to power up from a low power state. When ADTM is set, an additional 4SARCLKs are added to this time
							*/
	ADC0TK			= 0x00; //no use
	ADC0GTH			= 0xFF;
	ADC0GTL			= 0xFF;
	ADC0LTH			= 0x00;
	ADC0LTL			= 0x00;
	ADC0MX			= 0x05; //Selects the channel P0.0-P1.6 == 00000-01110
	REF0CN			= 0x08; /*bit7 = 0 The internal reference operates at 1.65 V nominal; 1 is 2.4v
							  bit6 = 0 Reserved
							  bit5 = 0 The ADC0 ground reference is the GND pin
							  bit4/3 = 01 The ADC0 voltage reference is the VDD pin
							  bit2 = 0 Disable the Temperature Sensor
							  bit1/0 = 00 Reserved
							*/
	ADC0 			= 0x0000;	//Clear the accumulated conversion results
	ADC0CN0_ADEN 	= 1; 		//ADC Enable
	EIE1		   &= (~0x08); 	//Disable interrupt

}

void ADC_CALUE(void)
{
    ADC0 = 0;//Clear the accumulated conversion results
    for(bySampleTimes=0; bySampleTimes<8; bySampleTimes++)
    {
    	ADC0CN0_ADINT = 0;	//Clear interrupt flag;
    	ADC0CN0_ADBUSY = 1;	//Start adc convert
		Delay1us(5);
    }
    wSampleValue = ADC0;
    ADC_Stop();
    wSampleValue = wSampleValue>>3;
}

void ADC_PowerSample(void)
{
	ADC_PowerSamplingInit();
    ADC_CALUE();
    ADC.Power_SampleValue = wSampleValue;
    ADC_IDSamplingInit();
    ADC_CALUE();
    ADC.ID_SampleValue = wSampleValue;

    if((ADC.Power_SampleValue <= MAX_POWER_VOLTAGE && ADC.Power_SampleValue >= MIN_POWER_VOLTAGE) ||
       (ADC.ID_SampleValue <= MAX_POWER_VOLTAGE    && ADC.ID_SampleValue >= MIN_POWER_VOLTAGE)	)
    {
    	ADC.Power_OK_Flag = TRUE;
    }
    else
    {
    	ADC.Power_OK_Flag = FALSE;
    }

//    wPowerValue = MAX_COMPARE(ADC.Power_SampleValue,ADC.ID_SampleValue);



}
void ADC_IDSample(void)
{
	ADC_IDSamplingInit();
    ADC_CALUE();
    ADC.ID_SampleValue = wSampleValue;
}

void ADC_ReadID(void)
{
	WORD temp;
//	DATA_HIGH;
//	wTimeMs = 10;
//	Delay1ms();
//	while(!ADC.Power_OK_Flag)
//	{
//		ADC_PowerSample();CLR_WDT;
//	}
	ADC_PowerSamplingInit();
	ADC_CALUE();
	ADC.Power_SampleValue = wSampleValue;
    ADC_IDSamplingInit();
    ADC_CALUE();
    ADC.ID_SampleValue = wSampleValue;

	//Judge ID code...
//	test code
//	ADC.PowerValueTest     = (WORD)((((DWORD)ADC.Power_SampleValue*3300)/1024)*6);
//	ADC.PowerValueSiteTest = (WORD)(((DWORD)ADC.Power_SampleValue*3300)/1024);
//	ADC.IDValueTest        = (WORD)((((DWORD)ADC.ID_SampleValue*3300)/1024)*6);
//	ADC.IDValueSiteTest    = (WORD)(((DWORD)ADC.ID_SampleValue*3300)/1024);

    RL_MIN_VALUE = 0;      // GND
	RL_MAX_VALUE = 200;//150;//100;

//	RML_MIN_VALUE = (WORD)(((DWORD)wPowerValue*83 )/100);//(WORD)(wPowerValue-20); // +12V
//	RML_MAX_VALUE = (WORD)(((DWORD)wPowerValue*117)/100);

	temp = (WORD)(((DWORD)ADC.Power_SampleValue*(P_R1VALUE+P_R2VALUE)/P_R1VALUE*R1VALUE/(R1VALUE+R2VALUE+R3VALUE)*117 )/100); //117 NC+17%
	if(temp > 1000)
	{
		temp = 1000;
	}

	RML_MIN_VALUE = temp;  // +12V
	RML_MAX_VALUE = 1024;//(WORD)(((DWORD)wPowerValue*33/1024*R1VALUE/(R1VALUE+R2VALUE)*117)/100);

	//RMR_LOW_MIN_VALUE = 0;	// DATA_LOW
	//RMR_LOW_MAX_VALUE = 300;

	RR_MIN_VALUE = RL_MAX_VALUE;//100; // NC
	RR_MAX_VALUE = (WORD)(((DWORD)ADC.Power_SampleValue*(P_R1VALUE+P_R2VALUE)/P_R1VALUE*R1VALUE/(R1VALUE+R2VALUE+R3VALUE)*83 )/100);
					//(WORD)(((DWORD)ADC.Power_SampleValue*33/1024*R1VALUE/(R1VALUE+R2VALUE+R3VALUE)*83 )/100);

//    FL_MIN_VALUE = 0;	// GND
//    FL_MAX_VALUE = 100;
//
//    FR_MIN_VALUE = 100; // NC
//    FR_MAX_VALUE = (WORD)(((DWORD)ADC.Power_SampleValue*83 )/100);//WORD)((wPowerValue*6)/8-24+100);

	if(ADC.ID_SampleValue >= RL_MIN_VALUE && ADC.ID_SampleValue < RL_MAX_VALUE)
	{
		DetInfo.bID  = ID_RL;	//GND
	}
	/*
	if(ADC.ID_SampleValue > RR_MIN_VALUE && ADC.ID_SampleValue < RR_MAX_VALUE)
	{
		DetInfo.bID  = ID_RR;	//NC
	}
	*/
	if(ADC.ID_SampleValue > RML_MIN_VALUE && ADC.ID_SampleValue < RML_MAX_VALUE)
	{
		wTimeMs = 10;
		Delay1ms(1);
		DATA_LOW;
		wTimeMs = 10;
		Delay1ms(1);
		//ADC_IDSample();
		/*
		if(ADC.ID_SampleValue > RMR_LOW_MIN_VALUE && ADC.ID_SampleValue < RMR_LOW_MAX_VALUE)
		{
			DetInfo.bID  = ID_RMR;
		}
		*/
		if(ADC.ID_SampleValue > RML_MIN_VALUE && ADC.ID_SampleValue < RML_MAX_VALUE)
		{
			DetInfo.bID  = ID_RML;
		}
	}
	DATA_HIGH;
}



void ADC_Start(void)
{
	ADC0CN0_ADINT = 0;	//Clear interrupt flag;
	ADC0CN0_ADBUSY = 1;	//Start adc convert
	ADC0CN0_ADEN = 1; 	//ADC Enable
	bDetectFlag = TRUE;
}

void ADC_Stop(void)
{
	ADC0CN0_ADBUSY = 0;	//Stop adc convert
	ADC0CN0_ADEN = 0; 	//ADC Disable
	bDetectFlag = FALSE;
}


void ADC_InitDetect(void)
{
	ADC0CN0_ADBMEN 	= 0; 	//Disable Burst Mode
	ADC0CN0_ADCM0	= 0;	//ADC0 conversion initiated is software triggerr
	ADC0CN0_ADCM1	= 0;
	ADC0CN1 		= 0x01;	//Common Mode Buffer Enable
	ADC0CF			= 0x1D;	/*bit7/6/5/4/3 = 00011 is set clock,49/(3+1) = 12.25mhz
							  bit2 = 1 is set 8 bit mode
							  bit1 = 0 Selects between Normal Tracking Modes
							  bit1 = 1 Gain Control set to 1 or 0.5
							*/
	ADC0AC			= 0x10; /*bit7 = 0 Disable 12-bit mode
							  bit6 = 0 ADC0H:ADC0L contain the result of the latest conversion
							  bit5/4/3 = 010 Right justified. No shifting applied;(*this is 10 bit set way)
							  bit2/1/0 = 000 Perform and Accumulate 1 conversion (not used in 12-bit mode).
							*/
	ADC0PWR			= 0x40; /*bit7/6 = 01 Select bias current mode 1 (SARCLK <= 16 MHz).
							  bit5 = 0 Low power mode disabled
							  bit4 = 0 Disable low power mode
							  bit2/1/0 = 000 This field sets the time delay allowed for the ADC to power up from a low power state. When ADTM is set, an additional 4SARCLKs are added to this time
							*/
	ADC0TK			= 0x00; //no use
	ADC0GTH			= 0xFF;
	ADC0GTL			= 0xFF;
	ADC0LTH			= 0x00;
	ADC0LTL			= 0x00;
	ADC0MX			= 0x06; //Selects the channel P0.0-P1.6 == 00000-01110
	REF0CN			= 0x08; /*bit7 = 0 The internal reference operates at 1.65 V nominal; 1 is 2.4v
							  bit6 = 0 Reserved
							  bit5 = 0 The ADC0 ground reference is the GND pin
							  bit4/3 = 01 The ADC0 voltage reference is the VDD pin
							  bit2 = 0 Disable the Temperature Sensor
							  bit1/0 = 00 Reserved
							*/
	EIE1		   |= ( 0x08); //10; Set Interrupt Priority; 11 is highest; 00 is lowest;
	EIP1		   &= (~0x08);
	EIP1H		   |= 0x08; //Enable interrupt
	ADC0CN0_ADEN 	= 1; 	//ADC Enable
    ADC_Start();
}

/*********************************************************************************************************************
** AD interrupt.
*********************************************************************************************************************/
SI_INTERRUPT (ADC_ISR, ADC0EOC_IRQn)
{
    if(RawData.bAdcBfIndex < 7)
    {
    	RawData.bAdcBuffer[RawData.bAdcBfIndex] = ADC0L;
    	RawData.bAdcBfIndex++;
    }
    else
    {
        BYTE i;
        BYTE Max = 0x00;
        BYTE Min = 0xFF;
        RawData.bAdcBuffer[RawData.bAdcBfIndex] = ADC0L;
    	RawData.bAdcBfIndex = 0;
		for(i=0; i<8;i++)	//Calculate  8 buffer = 6.5us, and samling use 24us; total is 32us
		{
			if(Max < RawData.bAdcBuffer[i])
				Max = RawData.bAdcBuffer[i];
			if(Min > RawData.bAdcBuffer[i])
				Min = RawData.bAdcBuffer[i];
		}
		RawData.bRawDataBuffer[RawData.wRawDataIndex] = Max - Min;
    }
    ADC0CN0_ADINT 	= 0;	//Clear interrupt flag;
    ADC0CN0_ADBUSY 	= 1;	//Start adc convert
}

/*********************************************************************************************************************
** AD window interrupt.
*********************************************************************************************************************/

SI_INTERRUPT (ADC_WINDOW_ISR, ADC0WC_IRQn)
{
	ADC0CN0_ADWINT = 0;//Clear interrupt flag;
}


