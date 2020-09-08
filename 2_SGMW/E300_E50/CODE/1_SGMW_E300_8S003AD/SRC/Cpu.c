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
    
    CLK_SWCR |= SWEN ;  //ʹ���Զ��л�����
    CLK_SWR   = 0XB4 ;  //ѡ�����л���ʱ��Դ  0XB4=HSE  0XD2=HSI  0XE1=HSI
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
    
    if(i<100)     //HSE��������
    {
        CLK_CSSR   |= CSSDIE + CSSEN ;              //����CSS
        CLK_PCKENR1 = PCKEN17 + PCKEN15 + PCKEN14 ; //��TIM1.5.6ʱ��
        CLK_PCKENR2 = PCKEN23 ;                     //��ADCʱ��
    }
    else          //�л���HIS
    {
        CLK_ECKR    = ~HSEEN ;                      //�ر�HSE
        CLK_CKDIVR  = HSIDIV0 ;                     // fmaster/2  =8MHz
        CLK_PCKENR1 = PCKEN17 + PCKEN15 + PCKEN14 ; //��TIM1��5��6ʱ��
        CLK_PCKENR2 = PCKEN23 ;                     //��ADCʱ��            
    }
    
#else            //HSI
    
    CLK_ECKR    = ~HSEEN ;                          //�ر�HSE
    CLK_CKDIVR  = HSIDIV1 ;                         // fcpu  = 16M 
    CLK_PCKENR1 = PCKEN17 + PCKEN15 + PCKEN14 ;     //��TIM1��5��6ʱ��
    CLK_PCKENR2 = PCKEN23 ;                         //��ADCʱ��       
    
#endif
}


void Cpu_WDG_Init()
{
    IWDG_KR  = 0X55;//ȥ��д����
    IWDG_KR  = 0XCC;//����Ӳ�����Ź�
    IWDG_PR  = 0X05;// 0x06=256��Ƶ---1.02s 0x05=128--500ms
    IWDG_RLR = 0xFF;//����������ֵ
    IWDG_KR  = 0xAA;//���¼������幷
}

void Cpu_EnableInterrupt (void)
{
    asm("rim");        //�������ж�
}

void Cpu_DisableInterrupt (void)
{
    asm("sim");        //�ر����ж�
}

void Cpu_InitHardwarePort(void)
{
    PD_DDR_DDR5 = PORT_OUTPUT;      // DATA Tx
    PD_ODR_ODR5 = V_LOW;            // �����
    PD_CR1_C15  = V_HIGH;           // �������
    PD_CR2_C25  = V_HIGH;           // 10MHZ
    
    PD_DDR_DDR6 = PORT_INPUT;       // DATA Rx 
    PD_ODR_ODR6 = V_LOW;            // �����
    PD_CR1_C16  = V_LOW;            // ��������
    PD_CR2_C26  = V_LOW;            // �ر��ⲿ�ж�
    
//    PD_DDR_DDR6 = PORT_INPUT;       // DATA AD--AIN6 
//    PD_ODR_ODR6 = V_LOW;            // �����
//    PD_CR1_C16  = V_LOW;            // ��������
//    PD_CR2_C26  = V_LOW;            // �ر��ⲿ�ж�

    PD_DDR_DDR4 = PORT_OUTPUT;      // ID Tx
    PD_ODR_ODR4 = V_LOW;            // �����
    PD_CR1_C14  = V_HIGH;           // �������
    PD_CR2_C24  = V_HIGH;           // 10MHZ
    
    PD_DDR_DDR3 = PORT_INPUT;       // ID Rx 
    PD_ODR_ODR3 = V_LOW;            // �����
    PD_CR1_C13  = V_LOW;            // ��������
    PD_CR2_C23  = V_LOW;            // �ر��ⲿ�ж�
    
//    PD_DDR_DDR3 = PORT_INPUT;       // ID AD--AIN4
//    PD_ODR_ODR3 = V_LOW;            // �����
//    PD_CR1_C13  = V_LOW;            // ��������
//    PD_CR2_C23  = V_LOW;            // �ر��ⲿ�ж�
            
    PC_DDR_DDR4 = PORT_INPUT;       // Sen_Echo--AIN2
    PC_ODR_ODR4 = V_LOW;            // �����
    PC_CR1_C14  = V_LOW;            // ��������
    PC_CR2_C24  = V_LOW;            // �ر��ⲿ�ж�
    
    PD_DDR_DDR2 = PORT_INPUT;       // Power_AD--AIN3
    PD_ODR_ODR2 = V_LOW;            // �����
    PD_CR1_C12  = V_LOW;            // ��������
    PD_CR2_C22  = V_LOW;            // �ر��ⲿ�ж�
    
    PC_DDR_DDR7 = PORT_OUTPUT;      // Drive 1
    PC_ODR_ODR7 = V_LOW;            // �����
    PC_CR1_C17  = V_HIGH;           // �������
    PC_CR2_C27  = V_HIGH;           // 10MHZ
    
    PC_DDR_DDR6 = PORT_OUTPUT;      // Drive 2
    PC_ODR_ODR6 = V_LOW;            // �����
    PC_CR1_C16  = V_HIGH;           // �������
    PC_CR2_C26  = V_HIGH;           // 10MHZ
    
    EXTI_CR1 = 0xC0;                //PD����Ϊ˫���ش���  PA:bit0/bit1 PB:bit2/bit3 PC:bit4/bit5 PD:bit6/bit7
}

void Cpu_Init_Vect(void)
{
    ITC_SPR1 = 0x3f;   /* EXTI0      CLK        AWU         TLI       */ /* EXTI0 = PA = 0X05*/ /* ���ȼ��Ӹߵ�������Ϊ11 00 01 10 */
    ITC_SPR2 = 0x30;   /* EXTI4      EXTI3      EXTI2       EXTI1     */ //PDΪͨ�Ž� PCΪ�ز���������
    ITC_SPR3 = 0xc0;   /* TIM1_OVR   SPI        CAN_TX      CAN_RX    */ //TIM1_OVRΪ����ʱ�䷢���ж�
    ITC_SPR4 = 0x00;   /* TIM3_OVR   TIM2_CMP   TIM2_OVR    TIM1_CMP  */
    ITC_SPR5 = 0x00;   /* URT2/3_TX  I2C        URT_RX      URT1_TX   */
    ITC_SPR6 = 0x10;   /* FLASH      TM4_OUT    ADC         URT2/3_RX */
//    ITC_SPR7 = 0xff;   /* reserve    reserve    reserve     reserve   */
//    ITC_SPR8 = 0xff;   /* reserve    reserve    reserve     reserve   */	
}

