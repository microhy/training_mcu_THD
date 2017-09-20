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
#include "../inc/lpc_PGA2310.h" 

/**********Macros***********************/
#define     SIGNALE_AMP_LIMIT    3600
/**********Variables********************/
volatile GainAdjust_DataDef GainAdjVal = {0};

/**********Function Prototypes**********/


/**********MAIN Routine*****************/
void GainAdjust_Func(void )
{
    float adjAv,adjdB;
    uint8_t tmp;
    //PeripInit_TIM_GainAdjust();
    //PeripInit_ADC_GainAdjust();
    GainAdjVal.Gain_ChanL = 192;
    GainAdjVal.Gain_ChanR = GainAdjVal.Gain_ChanL;
    while(GainAdjVal.flag_AdjDone == RESET)
    {
        PGA2310_GainConfig(GainAdjVal.Gain_ChanL, GainAdjVal.Gain_ChanR);
        while( GainAdjVal.flag_Timer10ms == RESET);
        TIM_CountCmd(LPC_TIM1, DISABLE); //cnt dis
        ADC_StartConvCmd(DISABLE);  //stop
        GainAdjVal.flag_Timer10ms = RESET; //reset
        if( GainAdjVal.ADC0ValMax>= SIGNALE_AMP_LIMIT ||(GainAdjVal.Gain_ChanL>=250))
        {
            GainAdjVal.flag_AdjDone = SET;
        }
        else
        {
            adjAv = (float)(SIGNALE_AMP_LIMIT/(float)GainAdjVal.ADC0ValMax); //限制的最大幅值除以当前最大值
            adjdB = (float)10*log(adjAv); //本来是20logAv倍，两个通道故除以2
            tmp = (uint8_t)(2*adjdB + 192);   //转换成PGA2310配置要加上的数
            if(tmp<=255)
            {
                GainAdjVal.Gain_ChanL = tmp;
                GainAdjVal.Gain_ChanR = GainAdjVal.Gain_ChanL;
                PGA2310_GainConfig(GainAdjVal.Gain_ChanL, GainAdjVal.Gain_ChanR);
            }
            else
            {
                GainAdjVal.flag_AdjDone = SET;
            }
            TIM_CountCmd(LPC_TIM1, ENABLE);
            TIM_CountCmd(LPC_TIM1, ENABLE);
        }
    }
}

void PeripInit_TIM_GainAdjust(void )
{   
    TIM_CountResetCmd(LPC_TIM1, ENABLE);
    TIM_CountResetCmd(LPC_TIM1, DISABLE);
    TIM_SetClockPRDiv(LPC_TIM1, 25-1);        // 1MHZ
    LPC_TIM1->PC = 0;
    LPC_TIM1->TC = 0;  
    LPC_TIM1->MR0 = 10000-1; //1Mhz计时10ms
    TIM_MatchControlCmd(LPC_TIM1, TIM_MR0I|TIM_MR0R, ENABLE);
    //TIM_CntTimControl(LPC_TIM0, CAPn_0, TIMMode_Timer);
    //TIM_CaputreControl(LPC_TIM0, CAPn_0, TIMMode_Rising);
    //TIM_CaptureIntCmd(LPC_TIM0, CAPn_0, ENABLE);
    TIM_CountCmd(LPC_TIM1, ENABLE);
}
/************************************************************
  Function   : ()
  Description: 
  Input      : None
  Output     : None
  Return     : None
  Others     : 
************************************************************/
void PeripInit_ADC_GainAdjust(void )
{
    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_SamplePins = ADC_SamplePins_0;
    ADC_InitStruct.ADC_ClockFreq = 13000000;
    ADC_InitStruct.ADC_BurstConv = ADC_BurstConv_Disable;
    ADC_InitStruct.ADC_Mode = ADC_Mode_Opeartional;
    ADC_InitStruct.ADC_StartSignal = ADC_StartSignal_Start;
    ADC_InitStruct.ADC_StartEdge = ADC_StarteEdge_Rising;
    ADC_Init(&ADC_InitStruct);
    ADC_ITConfig(ADC_IT_GLOBDONE, DISABLE);
    ADC_ITConfig(ADC_IT_CHANNEL0, ENABLE);
    ADC_StartConvCmd(ENABLE);
}

void PGA2310_PortInit(void )
{
    GPIO_PGA_CS->FIODIR |= Pin_PGA_CS;
    GPIO_PGA_SCLK->FIODIR |= Pin_PGA_SCLK;
    GPIO_PGA_SDI->FIODIR |= Pin_PGA_SDI;
    GPIO_PGA_SDO->FIODIR &=~Pin_PGA_SDO; //input dir
    PGA_CS_DIS;
    PGA_SCLK_L;PGA_SDI_1;
}

/************************************************************
  Function   : ()
  Description: 
  Calls      : 
  Called By  : 
  Input      : none
  Output     : none
  Return     : none
  Others     : 
************************************************************/
uint16_t PGA2310_GainConfig(uint8_t GaLeft, uint8_t GaRight)
{
    uint16_t i,temp;
    PGA_SCLK_L;
    PGA_CS_EN;
    __nop();__nop();__nop();__nop();
    temp = (GaRight<<8) | GaLeft;
    for(i=0; i<16; ++i)
    {   
        if(temp&0x8000)
        {   
            PGA_SDI_1;
        }
        else
        {   
            PGA_SDI_0;
        }
        temp <<= 1;
        temp |= PGA_SDO_READ;
        PGA_SCLK_L;
        __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
        PGA_SCLK_H;
        __nop();__nop();__nop();__nop();
    }
    PGA_SCLK_L;
    __nop();__nop(); __nop();__nop();
    PGA_CS_DIS;
    
    return(temp);
}

/************************************************************
End of filename
************************************************************/
