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
#ifndef __RIT_H
#define __RIT_H

#ifdef __cplusplus
    extern "C" {
#endif 
/**********Includes*********************/
#include "../inc/LPC1768GCZX.h"
#include "../inc/sysClock.h"
#include "../inc/lpc17xx_gpio.h"
// #include ".h"

/**********Macros***********************/

/**********Variables********************/
typedef struct
{
    uint32_t ppmCheck;
}RITCNT_DataDef;
extern volatile RITCNT_DataDef RITCnt;

/**********Function Prototypes**********/
void RIT_InitConfig(void );
    
#ifdef __cplusplus
}
#endif

#endif
/************************************************************
End of filename
************************************************************/
