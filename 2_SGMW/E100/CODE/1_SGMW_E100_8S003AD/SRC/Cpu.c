/*********************************************************************************************************************
** FILE NAME: cpu.c
**
** PROJECT:  DetTemplate
** CPU:	     STM8S003F3
** COMPILER: ST
** -------------------------------------------------------------------------------------------------------------------
** VERSION:  V1.0
** DATE:     2018.09.10
*********************************************************************************************************************/
#include "cpu.h"
#include "ADC.h"
#include "Timer.h"

  
void Cpu_Clk_Init (void)
{
#ifdef EN_EXTER       //HSE
    uchar i ;
    
    CLK_SWCR |= SWEN ;  //使能自动切换机制
    CLK_SWR   = 0XB4 ;  //选择需切换的时钟源  0XB4=HSE  0XD2=HSI  0XE1=HSI
    do
    {
        for(i=0; i<=100 ; i++)
        {
            Timer.Delay1ms(2);
            if(CLK_SWCR_SWIF)
            {
                break;
            }
        }
        break;
    }
    while(!CLK_SWCR_SWIF);
    
    if(i<100)     //HSE正常启动
    {
        CLK_CSSR   |= CSSDIE + CSSEN ;              //开启CSS
        CLK_PCKENR1 = PCKEN17 + PCKEN15 + PCKEN14 ; //打开TIM1.5.6时钟
        CLK_PCKENR2 = PCKEN23 ;                     //打开ADC时钟
    }
    else          //切换至HIS
    {
        CLK_ECKR    = ~HSEEN ;                      //关闭HSE
        CLK_CKDIVR  = HSIDIV0 ;                     // fmaster/2  =8MHz
        CLK_PCKENR1 = PCKEN17 + PCKEN15 + PCKEN14 ; //打开TIM1、5、6时钟
        CLK_PCKENR2 = PCKEN23 ;                     //打开ADC时钟            
    }
    
#else            //HSI
    
    CLK_ECKR    = ~HSEEN ;                          //关闭HSE
    CLK_CKDIVR  = HSIDIV1 ;                         // fcpu  = 16M 
    CLK_PCKENR1 = PCKEN17 + PCKEN15 + PCKEN14 ;     //打开TIM1、5、6时钟
    CLK_PCKENR2 = PCKEN23 ;                         //打开ADC时钟       
    
#endif
}


void Cpu_WDG_Init()
{
    IWDG_KR  = 0X55;//去除写保护
    IWDG_KR  = 0XCC;//启动硬件看门狗
    IWDG_PR  = 0X05;// 0x06=256分频---1.02s 0x05=128--500ms
    IWDG_RLR = 0xFF;//计数器重载值
    IWDG_KR  = 0xAA;//重新计数，清狗
}

void Cpu_EnableInterrupt (void)
{
    asm("rim");        //开启总中断
}

void Cpu_DisableInterrupt (void)
{
    asm("sim");        //关闭总中断
}

void Cpu_InitHardwarePort(void)
{
    PD_DDR_DDR5 = PORT_OUTPUT;      // DATA Tx
    PD_ODR_ODR5 = V_LOW;            // 输出低
    PD_CR1_C15  = V_HIGH;           // 推免输出
    PD_CR2_C25  = V_HIGH;           // 10MHZ
    
    PD_DDR_DDR6 = PORT_INPUT;       // DATA Rx 
    PD_ODR_ODR6 = V_LOW;            // 输出低
    PD_CR1_C16  = V_LOW;            // 浮空输入
    PD_CR2_C26  = V_LOW;            // 关闭外部中端
    
//    PD_DDR_DDR6 = PORT_INPUT;       // DATA AD--AIN6 
//    PD_ODR_ODR6 = V_LOW;            // 输出低
//    PD_CR1_C16  = V_LOW;            // 浮空输入
//    PD_CR2_C26  = V_LOW;            // 关闭外部中端

    PD_DDR_DDR4 = PORT_OUTPUT;      // ID Tx
    PD_ODR_ODR4 = V_LOW;            // 输出低
    PD_CR1_C14  = V_HIGH;           // 推免输出
    PD_CR2_C24  = V_HIGH;           // 10MHZ
    
    PD_DDR_DDR3 = PORT_INPUT;       // ID Rx 
    PD_ODR_ODR3 = V_LOW;            // 输出低
    PD_CR1_C13  = V_LOW;            // 浮空输入
    PD_CR2_C23  = V_LOW;            // 关闭外部中端
    
//    PD_DDR_DDR3 = PORT_INPUT;       // ID AD--AIN4
//    PD_ODR_ODR3 = V_LOW;            // 输出低
//    PD_CR1_C13  = V_LOW;            // 浮空输入
//    PD_CR2_C23  = V_LOW;            // 关闭外部中端
            
    PC_DDR_DDR4 = PORT_INPUT;       // Sen_Echo--AIN2
    PC_ODR_ODR4 = V_LOW;            // 输出低
    PC_CR1_C14  = V_LOW;            // 浮空输入
    PC_CR2_C24  = V_LOW;            // 关闭外部中端
    
    PD_DDR_DDR2 = PORT_INPUT;       // Power_AD--AIN3
    PD_ODR_ODR2 = V_LOW;            // 输出低
    PD_CR1_C12  = V_LOW;            // 浮空输入
    PD_CR2_C22  = V_LOW;            // 关闭外部中端
    
    PC_DDR_DDR7 = PORT_OUTPUT;      // Drive 1
    PC_ODR_ODR7 = V_LOW;            // 输出低
    PC_CR1_C17  = V_HIGH;           // 推免输出
    PC_CR2_C27  = V_HIGH;           // 10MHZ
    
    PC_DDR_DDR6 = PORT_OUTPUT;      // Drive 2
    PC_ODR_ODR6 = V_LOW;            // 输出低
    PC_CR1_C16  = V_HIGH;           // 推免输出
    PC_CR2_C26  = V_HIGH;           // 10MHZ
    
    EXTI_CR1 = 0xC0;                //PD设置为双边沿触发  PA:bit0/bit1 PB:bit2/bit3 PC:bit4/bit5 PD:bit6/bit7
}

void Cpu_Init_Vect(void)
{
    ITC_SPR1 = 0x3f;   /* EXTI0      CLK        AWU         TLI       */ /* EXTI0 = PA = 0X05*/ /* 优先级从高到低依次为11 00 01 10 */
    ITC_SPR2 = 0x30;   /* EXTI4      EXTI3      EXTI2       EXTI1     */ //PD为通信脚 PC为回波输入引脚
    ITC_SPR3 = 0xc0;   /* TIM1_OVR   SPI        CAN_TX      CAN_RX    */ //TIM1_OVR为距离时间发送中断
    ITC_SPR4 = 0x00;   /* TIM3_OVR   TIM2_CMP   TIM2_OVR    TIM1_CMP  */
    ITC_SPR5 = 0x00;   /* URT2/3_TX  I2C        URT_RX      URT1_TX   */
    ITC_SPR6 = 0x10;   /* FLASH      TM4_OUT    ADC         URT2/3_RX */
//    ITC_SPR7 = 0xff;   /* reserve    reserve    reserve     reserve   */
//    ITC_SPR8 = 0xff;   /* reserve    reserve    reserve     reserve   */	
}

