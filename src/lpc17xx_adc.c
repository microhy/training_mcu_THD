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
  Description: ADC�ж�����
  Input      :
    @param ADC_IT: �ж�����
      @arg ADC_IT_CHANNEL0 :ͨ��0���ж�
      @arg ADC_IT_CHANNEL1 :ͨ��1���ж�
      @arg ADC_IT_CHANNEL2 :ͨ��2���ж�
      @arg ADC_IT_CHANNEL3 :ͨ��3���ж�
      @arg ADC_IT_CHANNEL4 :ͨ��4���ж�
      @arg ADC_IT_CHANNEL5 :ͨ��5���ж�
      @arg ADC_IT_CHANNEL6 :ͨ��6���ж�
      @arg ADC_IT_CHANNEL7 :ͨ��7���ж�
      @arg ADC_IT_GLOBDONE :ȫ�����ݼĴ���ȫ���жϱ�־
    @param NewState:�µ�ʹ��λ
      @arg ENABLE :ʹ�ܸ��ж�
      @arg DISABLE :���ܸ��ж�
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
  Description: ADC����־λ״̬
  Input      : 
    @param ADC_STATUS ״̬λ 
      @arg ADC_STATUS_DONE0~7 :ת����ɱ�־λ
      @arg ADC_STATUS_OVERRUN0~7 �����ݸ��ǳ���λ
      @arg ADC_STATUS_ADINT: �жϱ�־λ
  Output     : None
  Return     : SET ��λ��λ ; RESET ��λδ��λ
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
