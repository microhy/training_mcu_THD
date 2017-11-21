/************************************************************
    Copyright (C), 2013, Hnist FS_GCZX.
    
    FileName: .c
    Author: huyong    Version :       Date:
    Description:
    Function List: 
    1.
    History:
    <author>    <time>      <version >      <desc>
    David       96/10/12        1.0         build this moudle
************************************************************/

/**********Includes*********************/
#include "../inc/lpc_PGA2310.h" 

/**********Macros***********************/   //去掉直流量
#define     SIGNALE_AMP_LIMIT    1150      /* 4096*2.8V/3.3 = 3475-2048(1.65V)≈1430*/
#define     AMP_OFFSET_LOW       101       /*  80*3.3/4096 = 100 mV*/
#define     AMP_OFFSET_HIGH      51        /*  50*3.3/4096 = 50 mV*/
/**********Variables********************/
volatile GainAdjust_DataDef GainAdjVal = {0};

/**********Function Prototypes**********/

#define     N       5
/**********MAIN Routine*****************/
void PGA2310_GainAdjust_Func(void )
{   
    //int tmp,tmpL,tmpR;
    //float adjAv,adjdB; //,currentdB
    //GainAdjVal.Gain_ChanL = 192;
    //GainAdjVal.Gain_ChanR = GainAdjVal.Gain_ChanL;
    printf("cur = %d  ",GainAdjVal.ADC0Value_cur);
    if((SIGNALE_AMP_LIMIT>GainAdjVal.ADC0Value_cur)&&(SIGNALE_AMP_LIMIT-GainAdjVal.ADC0Value_cur>AMP_OFFSET_LOW))
    {
        if(GainAdjVal.Gain_ChanL<=253)
        {
            GainAdjVal.Gain_ChanL += 2;
        }
        else if(GainAdjVal.Gain_ChanR<=253)
        {
            GainAdjVal.Gain_ChanR += 2;
        }
        //PGA2310_GainConfig(GainAdjVal.Gain_ChanL, GainAdjVal.Gain_ChanR);
        printf("rising GL = %d, GR = %d\n",GainAdjVal.Gain_ChanL, GainAdjVal.Gain_ChanR);
    }
    else if((GainAdjVal.ADC0Value_cur>SIGNALE_AMP_LIMIT)&&(GainAdjVal.ADC0Value_cur-SIGNALE_AMP_LIMIT>AMP_OFFSET_HIGH))
    {
        if(GainAdjVal.Gain_ChanL>=3)
        {
            GainAdjVal.Gain_ChanL -= 3;
        }
        else if(GainAdjVal.Gain_ChanR>=3)
        {
            GainAdjVal.Gain_ChanR -= 3;
        }
        //PGA2310_GainConfig(GainAdjVal.Gain_ChanL, GainAdjVal.Gain_ChanR);
        printf("falling GL = %d, GR = %d\n",GainAdjVal.Gain_ChanL, GainAdjVal.Gain_ChanR);
    }//增益的更新会在频率测量之后更新
    
}

void PeripInit_TIM_GainAdjust(void )
{   
    TIM_CountResetCmd(LPC_TIM1, ENABLE);
    TIM_CountResetCmd(LPC_TIM1, DISABLE);
    TIM_SetClockPRDiv(LPC_TIM1, 5-1);        // 25/5=5MHZ
    LPC_TIM1->PC = 0;
    LPC_TIM1->TC = 0;  
    LPC_TIM1->MR0 = 250000-1; //5Mhz计时50ms
    TIM_MatchControlCmd(LPC_TIM1, TIM_MR0I|TIM_MR0R, ENABLE);
    //TIM_ExtMatchControl(LPC_TIM1, TIM_EMC0_TOG);
    //TIM_CntTimControl(LPC_TIM0, CAPn_0, TIMMode_Timer);
    //TIM_CaputreControl(LPC_TIM0, CAPn_0, TIMMode_Rising);
    //TIM_CaptureIntCmd(LPC_TIM0, CAPn_0, ENABLE);
    //TIM_CountCmd(LPC_TIM1, ENABLE);
}
/************************************************************
  Function   : ()
  Description: 
  Input      : None
  Output     : None
  Return     : None
  Others     : 
************************************************************/
// void PeripInit_ADC_GainAdjust(void )
// {
//     uint32_t tmp;
//     ADC_InitTypeDef ADC_InitStruct;
//     ADC_InitStruct.ADC_SamplePins = ADC_SamplePins_2;
//     ADC_InitStruct.ADC_ClockFreq = 12000000;  // 12MHz
//     ADC_InitStruct.ADC_BurstConv = ADC_BurstConv_Enable;
//     ADC_InitStruct.ADC_Mode = ADC_Mode_Opeartional;
//     ADC_InitStruct.ADC_StartSignal = ADC_StartSignal_Stop;
//     ADC_InitStruct.ADC_StartEdge = ADC_StartEdge_Rising;
//     ADC_ITConfig(ADC_IT_GLOBDONE, DISABLE);
//     ADC_Init(&ADC_InitStruct);
//     ADC_ITConfig(ADC_IT_CHANNEL2, ENABLE);
// }

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
