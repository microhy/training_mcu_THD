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
#ifndef __LPC17XX_ADC_H
#define __LPC17XX_ADC_H

#ifdef __cplusplus
    extern "C" {
#endif 
/**********Includes*********************/
#include "../inc/LPC1768GCZX.h"

#define     FPCLK_ADC       (25000000)

typedef struct
{
    uint32_t ADC_SamplePins;
    uint32_t ADC_ClockFreq;
    uint32_t ADC_BurstConv;
    uint32_t ADC_Mode;
    uint32_t ADC_StartSignal;
    uint32_t ADC_StartEdge;
}ADC_InitTypeDef;


/**********Macros***********************/
#define          ADC_SamplePins_0               ((uint32_t)0x00000001)
#define          ADC_SamplePins_1               ((uint32_t)0x00000002)
#define          ADC_SamplePins_2               ((uint32_t)0x00000004)
#define          ADC_SamplePins_3               ((uint32_t)0x00000008)
#define          ADC_SamplePins_4               ((uint32_t)0x00000010)
#define          ADC_SamplePins_5               ((uint32_t)0x00000020)
#define          ADC_SamplePins_6               ((uint32_t)0x00000040)
#define          ADC_SamplePins_7               ((uint32_t)0x00000080)

#define          ADC_BurstConv_Enable           ((uint32_t)0x00010000)
#define          ADC_BurstConv_Disable          ((uint32_t)0x00000000) 
  
#define          ADC_Mode_Opeartional           ((uint32_t)0x00200000) 
#define          ADC_Mode_PowerDown             ((uint32_t)0x00000000)

#define          ADC_StartSignal_Stop           ((uint32_t)0x00000000)
#define          ADC_StartSignal_Start          ((uint32_t)0x01000000)
#define          ADC_StartSignal_P210           ((uint32_t)0x02000000)
#define          ADC_StartSignal_P127           ((uint32_t)0x03000000)
#define          ADC_StartSignal_MAT01          ((uint32_t)0x04000000)
#define          ADC_StartSignal_MAT03          ((uint32_t)0x05000000)
#define          ADC_StartSignal_MAT10          ((uint32_t)0x06000000)
#define          ADC_StartSignal_MAT11          ((uint32_t)0x07000000)

#define          ADC_StarteEdge_Falling         ((uint32_t)0x08000000)
#define          ADC_StarteEdge_Rising          ((uint32_t)0x00000000)  

#define          ADC_IT_CHANNEL0                ((uint32_t)0x00000001)
#define          ADC_IT_CHANNEL1                ((uint32_t)0x00000002)
#define          ADC_IT_CHANNEL2                ((uint32_t)0x00000004)
#define          ADC_IT_CHANNEL3                ((uint32_t)0x00000008)
#define          ADC_IT_CHANNEL4                ((uint32_t)0x00000010)
#define          ADC_IT_CHANNEL5                ((uint32_t)0x00000020)
#define          ADC_IT_CHANNEL6                ((uint32_t)0x00000040)
#define          ADC_IT_CHANNEL7                ((uint32_t)0x00000080)
#define          ADC_IT_GLOBDONE                ((uint32_t)0x00000100)   

#define          ADC_STATUS_DONE0               ((uint32_t)0x00000001) 
#define          ADC_STATUS_DONE1               ((uint32_t)0x00000002) 
#define          ADC_STATUS_DONE2               ((uint32_t)0x00000004) 
#define          ADC_STATUS_DONE3               ((uint32_t)0x00000008) 
#define          ADC_STATUS_DONE4               ((uint32_t)0x00000010) 
#define          ADC_STATUS_DONE5               ((uint32_t)0x00000020) 
#define          ADC_STATUS_DONE6               ((uint32_t)0x00000040) 
#define          ADC_STATUS_DONE7               ((uint32_t)0x00000080)  
#define          ADC_STATUS_OVERRUN0            ((uint32_t)0x00000100) 
#define          ADC_STATUS_OVERRUN1            ((uint32_t)0x00000200) 
#define          ADC_STATUS_OVERRUN2            ((uint32_t)0x00000400) 
#define          ADC_STATUS_OVERRUN3            ((uint32_t)0x00000800) 
#define          ADC_STATUS_OVERRUN4            ((uint32_t)0x00001000) 
#define          ADC_STATUS_OVERRUN5            ((uint32_t)0x00002000) 
#define          ADC_STATUS_OVERRUN6            ((uint32_t)0x00004000) 
#define          ADC_STATUS_OVERRUN7            ((uint32_t)0x00008000) 
#define          ADC_STATUS_ADINT               ((uint32_t)0x00010000) 

/**********Function Prototypes**********/
void ADC_Init(ADC_InitTypeDef* ADC_InitStruct);
void ADC_StartConvCmd(FunctionalState NewState);
void ADC_ITConfig(uint32_t ADC_IT, FunctionalState NewState);
FlagStatus ADC_GetFlagStatus(uint32_t ADC_STATUS);
ITStatus ADC_GetITStatus(void );

#ifdef __cplusplus
}
#endif

#endif
/************************************************************
End of filename
************************************************************/
