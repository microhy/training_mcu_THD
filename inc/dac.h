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
#ifndef __DAC_H
#define __DAC_H

#ifdef __cplusplus
    extern "C" {
#endif 
/**********Includes*********************/
#include "../inc/LPC1768GCZX.h"       
// #include ".h"


/**********InitTypeDef***********************/
typedef struct
{
    uint32_t DAC_AoutValue;
    uint32_t DAC_MaxUpRate;
    uint32_t DAC_Doublebuf;
    uint32_t DAC_TimOutCnt;
    uint32_t DAC_TimloadVal;
    uint32_t DAC_DmaAccess;
}DAC_InitTypeDef;

/**********Macros***********************/
#define      DAC_MaxUpRate_1MHZ              ((uint32_t)0x00000000)
#define      DAC_MaxUpRate_400KHZ            ((uint32_t)0x00010000)

#define      DAC_Doublebuf_Enable            ((uint32_t)0x00000002)
#define      DAC_Doublebuf_Disable           ((uint32_t)0x00000000)

#define      DAC_TimOutCnt_Enable            ((uint32_t)0x00000004)
#define      DAC_TimOutCnt_Disable           ((uint32_t)0x00000000)

#define      DAC_DmaAccess_Enable            ((uint32_t)0x00000008)
#define      DAC_DmaAccess_Disable           ((uint32_t)0x00000000)

/**********Variables********************/


/**********Function Prototypes**********/
void PeripInit_DAC(void );
void DAC_Init(DAC_InitTypeDef* DAC_InitStruct);

#ifdef __cplusplus
}
#endif

#endif
/************************************************************
End of filename
************************************************************/
