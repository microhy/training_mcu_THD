/************************************************************
    Copyright (C), 2013, Hnist FS_GCZX.
    
    FileName: 
    Author:     Version :       Date:
    Description:
    Function List: 
    1.
    History:
    <author>    <time>      <version >      <desc>
    David           1.0         build this moudle
************************************************************/

/**********Includes*********************/
#include "../inc/carctr.h"

/**********Macros***********************/

/**********Variables********************/
volatile ppmDecode_DataDef ppmVal = {0x00};
volatile CarCtr_DataDef carVal = {0x00};

/**********Function Prototypes**********/

/**********MAIN Routine*****************/
void TIMER3_IRQHandler()
{
    if(LPC_TIM3->IR&TIM_IT_CR0)
    {
        LPC_TIM3->IR |= TIM_IT_CR0; //���ж�
        if(LPC_GPIO0->FIOPIN & GPIO_Pin_23)     //����0��ǰΪ��
        {
            ppmVal.cap0Val[0] = LPC_TIM3->CR0;
            ppmVal.Flag_Cap0H = SET;
        }
        else if( ppmVal.Flag_Cap0H != RESET)    //����0Ϊ�����Ѿ����񵽸ߵ�ƽ
        {
            ppmVal.cap0Val[1] = LPC_TIM3->CR0;
            ppmVal.Flag_Dedone0 = SET;  //CAP0�������
            ppmVal.Flag_Cap0H = RESET;
        }
        else ;
    }
    if(LPC_TIM3->IR&TIM_IT_CR1)
    {
        LPC_TIM3->IR |= TIM_IT_CR1; //���ж�
        if(LPC_GPIO0->FIOPIN & GPIO_Pin_24)     //����1��ǰΪ��
        {
            ppmVal.cap1Val[0] = LPC_TIM3->CR1;
            ppmVal.Flag_Cap1H = SET;
        }
        else if( ppmVal.Flag_Cap1H != RESET)    //����1Ϊ�����Ѿ����񵽸ߵ�ƽ
        {
            ppmVal.cap1Val[1] = LPC_TIM3->CR1;
            ppmVal.Flag_Dedone1 = SET;    //CAP1�������
            ppmVal.Flag_Cap1H = RESET;
        }
        else ;
    }
//     else 
//     {
//         printf("LPC_TIM3->IR = %x\n", LPC_TIM3->IR);
//     }
}

void MyTimer_InitConfig(void )
{
    //��ʱ��Ӳ����ʼ��
    Peripherals_PowerControl(PCTIM3, ENABLE);
    PeripheralsClock_Divider(PCLK_TIMER3, CCLK_DIV4);
    //P0.23,P0.24��PPM��������
    GPIO0_PinFuncSel(GPIO_Pin_23, GPIOFunc_AF3);
    GPIO0_PinFuncSel(GPIO_Pin_24, GPIOFunc_AF3);
    //��ʱ����ʼ��
    TIM_CountResetCmd(LPC_TIM3, ENABLE);
    TIM_CountResetCmd(LPC_TIM3, DISABLE);
    TIM_SetClockPRDiv(LPC_TIM3, 24);
    TIM_CaputreControl(LPC_TIM3, CAPn_0, TIMMode_Both);
    TIM_CaptureIntCmd(LPC_TIM3, CAPn_0, ENABLE);
    TIM_CaputreControl(LPC_TIM3, CAPn_1, TIMMode_Both);
    TIM_CaptureIntCmd(LPC_TIM3, CAPn_1, ENABLE);
}

/************************************************************
  Function   : ()
  Description: 
  Input      : None
  Output     : None
  Return     : None
  Others     : 
************************************************************/
void PWM_CarChannelConfig(uint32_t pwmchanx, uint16_t duty)
{
    switch (pwmchanx)
    {
        case PWMChan1: LPC_PWM1->MR1 = duty; break;
        case PWMChan2: LPC_PWM1->MR2 = duty; break;
        case PWMChan3: LPC_PWM1->MR3 = duty; break;
        case PWMChan4: LPC_PWM1->MR4 = duty; break;
        case PWMChan5: LPC_PWM1->MR5 = duty; break;
        case PWMChan6: LPC_PWM1->MR6 = duty; break;
        default: while(1);
    }
}

void PWM_CarCtrInit(void )
{
    LPC_SC->PCONP |= PCPWM1;    //��PWM1�ĵ�Դ���ƣ�Ĭ���Ǵ򿪵ģ�
    PeripheralsClock_Divider(PCLK_PWM1, CCLK_DIV4); //Ĭ����/4
    //PWM1.2~1.5�������� P2.1~2.4
    GPIO2_PinFuncSel(GPIO_Pin_4, GPIOFunc_AF1);
    GPIO2_PinFuncSel(GPIO_Pin_1, GPIOFunc_AF1);
    GPIO2_PinFuncSel(GPIO_Pin_2, GPIOFunc_AF1);
    GPIO2_PinFuncSel(GPIO_Pin_3, GPIOFunc_AF1);
    
    LPC_PWM1->TCR = TCR_CntRst; //��λ��ʱ����������Ԥ��Ƶ��  
    LPC_PWM1->TC  = 0;           //���㶨ʱ��������
    LPC_PWM1->PR = 24;             //����Ԥ��Ƶ
    LPC_PWM1->MR0 = PWM_Frequency; //����PWM��Ƶ��
    LPC_PWM1->MCR |= BIT1;         //PWMTC����PWMMR0ƥ��ʱ��λ�����㣩
    LPC_PWM1->LER |= BIT0;         //����MR0��ֵ
    PWM_CarChannelConfig(PWMChan5, 0);
    PWM_CarChannelConfig(PWMChan2, 0);
    PWM_CarChannelConfig(PWMChan3, 0);
    PWM_CarChannelConfig(PWMChan4, 0);
    PWM_LogicEnable(PWMChan5|PWMChan2|PWMChan3|PWMChan4);
    PWM_OutputCmd(PWMChan5|PWMChan2|PWMChan3|PWMChan4, ENABLE);
    PWM_CountCmd(ENABLE);
    //����������Ź�������
    GPIO_DirectionConfig(LPC_GPIOIN, GPIO_Pin_INALL, GPIO_Output);   //IN1~8����Ϊ���
    GPIO_ResetBits(LPC_GPIOIN, GPIO_Pin_INALL);
}

void Car_GoForward(void )
{
    GPIO_ResetBits(LPC_GPIOIN, GPIO_Pin_INEVEN); //IN_ż�����Ÿ�λ
    GPIO_SetBits(LPC_GPIOIN, GPIO_Pin_INODD);    //IN_����������λ
}

void Car_GoBack(void )
{
    GPIO_ResetBits(LPC_GPIOIN, GPIO_Pin_INODD); //IN_�������Ÿ�λ
    GPIO_SetBits(LPC_GPIOIN, GPIO_Pin_INEVEN);  //IN_ż��������λ
}

void Car_Stop(void )
{
    GPIO_ResetBits(LPC_GPIOIN, GPIO_Pin_INALL);
    LPC_PWM1->MR1 = 0;      //PWM��ռ�ձȱ�Ϊ0
    LPC_PWM1->MR2 = 0;  
    LPC_PWM1->MR3 = 0;
    LPC_PWM1->MR4 = 0;
    LPC_PWM1->LER |= BIT5|BIT2|BIT3|BIT4;//��������
}
/************************************************************
End of filename
************************************************************/
