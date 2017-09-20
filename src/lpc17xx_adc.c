/************************************************************
    Copyright (C), 2013, Hnist FS_GCZX.
    
    FileName: .c
    Author:     Version :       Date:
    Description:
    Function List: 
    1.
    History:
    <author>    <time>      <version >      <desc>
    David       96/10/12        1.0         build this moudle
************************************************************/

/**********Includes*********************/
#include "../inc/lpc17xx_adc.h" 

/**********Macros***********************/
#define         ADC_StartSignal_Mask           ((uint32_t)0xF8FFFFFF)
#define         ADSTAT_ADINT_Set               ((uint32_t)0x00010000)

/**********Variables********************/

/**********Function Prototypes**********/

/************************************************************
  Function   : ADC_Init()
  Description: 
  Input      : None
  Output     : None
  Return     : None
  Others     : 
    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_SamplePins = ADC_SamplePins_0;
    ADC_InitStruct.ADC_ClockFreq = 4000000;
    ADC_InitStruct.ADC_BurstConv = ADC_BurstConv_Disable;
    ADC_InitStruct.ADC_Mode = ADC_Mode_Opeartional;
    ADC_InitStruct.ADC_StartSignal = ADC_StartSignal_Stop;
    ADC_InitStruct.ADC_StartEdge = ADC_StarteEdge_Rising;
    ADC_Init(&ADC_InitStruct);
************************************************************/
void ADC_Init(ADC_InitTypeDef* ADC_InitStruct)
{
    uint32_t tmp = 0X00;
    tmp = (FPCLK_ADC/ADC_InitStruct->ADC_ClockFreq - 1) << 8;

    LPC_ADC->ADCR = ADC_InitStruct->ADC_SamplePins | tmp | 
                    ADC_InitStruct->ADC_BurstConv | ADC_InitStruct->ADC_Mode | 
                    ADC_InitStruct->ADC_StartSignal | ADC_InitStruct->ADC_StartEdge;
}

/************************************************************
  Function   : ADC_StartConvCmd()
  Description: 
  Input      : None
  Output     : None
  Return     : None
  Others     : 
************************************************************/
void ADC_StartConvCmd(FunctionalState NewState)
{
    if(NewState != DISABLE)
    {
        LPC_ADC->ADCR |= ADC_StartSignal_Start;
    }
    else
    {
        LPC_ADC->ADCR &= ADC_StartSignal_Mask;  //Stop Conv
    }
}

/************************************************************
  Function   : ADC_ITConfig()
  Description: ADC中断配置
  Input      :
    @param ADC_IT: 中断名称
      @arg ADC_IT_CHANNEL0 :通道0的中断
      @arg ADC_IT_CHANNEL1 :通道1的中断
      @arg ADC_IT_CHANNEL2 :通道2的中断
      @arg ADC_IT_CHANNEL3 :通道3的中断
      @arg ADC_IT_CHANNEL4 :通道4的中断
      @arg ADC_IT_CHANNEL5 :通道5的中断
      @arg ADC_IT_CHANNEL6 :通道6的中断
      @arg ADC_IT_CHANNEL7 :通道7的中断
      @arg ADC_IT_GLOBDONE :全局数据寄存器全局中断标志
    @param NewState:新的使能位
      @arg ENABLE :使能该中断
      @arg DISABLE :禁能该中断
  Output     : None
  Return     : None
  Others     : 
************************************************************/
void ADC_ITConfig(uint32_t ADC_IT, FunctionalState NewState)
{
    if(NewState != DISABLE)
    {
        LPC_ADC->ADINTEN |= ADC_IT;
    }
    else
    {
        LPC_ADC->ADINTEN &=~ADC_IT;
    }
}

// uint16_t ADC_ReadConvResult(uint32_t ADDRx)
// {

// }
/************************************************************
  Function   : ADC_GetFlagStatus()
  Description: ADC读标志位状态
  Input      : 
    @param ADC_STATUS 状态位 
      @arg ADC_STATUS_DONE0~7 :转换完成标志位
      @arg ADC_STATUS_OVERRUN0~7 ：数据覆盖出错位
      @arg ADC_STATUS_ADINT: 中断标志位
  Output     : None
  Return     : SET 该位置位 ; RESET 该位未置位
  Others     : 
************************************************************/
FlagStatus ADC_GetFlagStatus(uint32_t ADC_STATUS)
{
    if(LPC_ADC->ADSTAT & ADC_STATUS)
    {
        return(SET);
    }
    else
    {
        return(RESET);
    }
}

ITStatus ADC_GetITStatus(void )
{
    if(LPC_ADC->ADSTAT & ADSTAT_ADINT_Set)
    {
        return(SET);
    }
    else
    {
        return(RESET);
    }
}

/******************************************************************************
** Function name:   ADC0Handler
**
** Descriptions:    ADC0 interrupt handler
**
** parameters:  None
** Returned value:  None
** 
******************************************************************************/
// void ADC_IRQHandler (void)  
// {
//     uint32_t regVal;
//     regVal = LPC_ADC -> ADSTAT; /* Read ADC will clear the interrupt */    
// }
/************************************************************
End of filename
************************************************************/
