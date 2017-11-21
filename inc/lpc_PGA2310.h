/************************************************************
    Copyright (C), 2013, Hnist FS_GCZX.
    
    File name: .h
    Author:         Version:        Date:
    Description: 
    Others: 
    Function List:
    History: 
    1.Date:
      Author:
      Modification:
    2. ...
*************************************************************/
#ifndef __LPC_PGA2310_H
#define __LPC_PGA2310_H

#ifdef __cplusplus
    extern "C" {
#endif 
/**********Includes*********************/
#include "../system_h/lpc17xx.h"
#include "../system_h/stdint.h"
#include "../inc/lpc17xx_gpio.h"

#define     ADN     4
typedef struct{
    //uint32_t ADC0Value[ADN+1];
    uint32_t ADC0Value_buf;
    uint32_t ADC0Value_cur;
    //uint32_t ADC0ValMax;
    //uint32_t Cnt_avoidDither;   //消抖动计数
    FlagStatus flag_Timer10ms;
    FlagStatus flag_AdjDone; //调整完成标志位
    FlagStatus flag_AD0IntDone;
    //uint32_t TimerCnt_Tog;
    //float VinMax;
    //float VinCurrent;
    uint8_t Gain_ChanL;
    uint8_t Gain_ChanR;
}GainAdjust_DataDef;

extern volatile GainAdjust_DataDef GainAdjVal;
/**********Macros***********************/
#define     GPIO_PGA_CS         LPC_GPIO2
#define     GPIO_PGA_SCLK       LPC_GPIO2
#define     GPIO_PGA_SDI        LPC_GPIO2
#define     GPIO_PGA_SDO        LPC_GPIO2
        
#define     Pin_PGA_CS          GPIO_Pin_2
#define     Pin_PGA_SDI         GPIO_Pin_4
#define     Pin_PGA_SCLK        GPIO_Pin_6
#define     Pin_PGA_SDO         GPIO_Pin_8

#define     PGA_CS_EN           GPIO_PGA_CS->FIOCLR = Pin_PGA_CS   /*CS=0*/
#define     PGA_CS_DIS          GPIO_PGA_CS->FIOSET = Pin_PGA_CS

#define     PGA_SCLK_L          GPIO_PGA_SCLK->FIOCLR = Pin_PGA_SCLK        
#define     PGA_SCLK_H          GPIO_PGA_SCLK->FIOSET = Pin_PGA_SCLK

#define     PGA_SDI_0           GPIO_PGA_SDI->FIOCLR = Pin_PGA_SDI
#define     PGA_SDI_1           GPIO_PGA_SDI->FIOSET = Pin_PGA_SDI

#define     PGA_SDO_READ        (GPIO_PGA_SDO->FIOPIN&Pin_PGA_SDO ? 1 : 0)
/**********Variables********************/
       
/**********Function Prototypes**********/
void PGA2310_GainAdjust_Func(void );
void PeripInit_TIM_GainAdjust(void );
void PeripInit_ADC_GainAdjust(void );
void PGA2310_PortInit(void );
uint16_t PGA2310_GainConfig(uint8_t GaLeft, uint8_t GaRight);
    
#ifdef __cplusplus
}
#endif

#endif
/************************************************************
End of filename
************************************************************/
