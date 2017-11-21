/************************************************************
    Copyright (C), 2013, Hnist FS_GCZX.
    
    FileName: 
    Author: huyong    Version :       Date:
    Description:
    Function List: 
    1.
    History:
    <author>    <time>      <version >      <desc>
    David           1.0         build this moudle
************************************************************/

/**********Includes*********************/
#include "../inc/rit.h"

/**********Macros***********************/
#define     RIT_INT_Set             ((uint32_t)0x00000001)

#define     RIT_ENCLR_Set           ((uint32_t)0x00000002)
#define     RIT_ENCLR_Reset         ((uint32_t)0xFFFFFFFC)

#define     RIT_ENDR_Set            ((uint32_t)0x00000004)
#define     RIT_ENDR_Reset          ((uint32_t)0xFFFFFFFB)

#define     RIT_ENABLE_Set          ((uint32_t)0x00000008)
#define     RIT_ENABLE_Reset        ((uint32_t)0xFFFFFFF7)
/**********Variables********************/
volatile RITCNT_DataDef RITCnt = {0};

/**********Function Prototypes**********/


/**********MAIN Routine*****************/

void RIT_InitConfig(void )
{
    Peripherals_PowerControl(PCRIT, ENABLE);
    PeripheralsClock_Divider(PCLK_RIT, CCLK_DIV8); //FPCLK/8 = 12.5MHZ, 0.08us计数
    
    LPC_RIT->RICTRL |= RIT_INT_Set; //清中断标志
    LPC_RIT->RICOMPVAL = 125000; //计时10ms
    LPC_RIT->RIMASK = 0X00;
    LPC_RIT->RICTRL = RIT_ENCLR_Set|RIT_ENABLE_Set;
}

void RIT_IRQHandler(void )
{
    LPC_RIT->RICTRL |= RIT_INT_Set; //清中断标志
    ++RITCnt.ppmCheck;
}
/************************************************************
  Function   : ()
  Description: 
  Input      : None
  Output     : None
  Return     : None
  Others     : 
************************************************************/
void RIT_CompValConfig(uint32_t compVal)
{
    LPC_RIT->RICOMPVAL = compVal;
}

void RIT_MaskBitConfig(uint32_t maskBit)
{
    LPC_RIT->RIMASK = maskBit;
}

ITStatus RIT_GetINTStatus(void )
{
    ITStatus bitstatus = RESET;
    if(LPC_RIT->RICTRL&RIT_INT_Set)
    {
        bitstatus = SET;
    }

    return(bitstatus);
}

void RIT_CntValConfig(uint32_t cntVal)
{
    LPC_RIT->RICOUNTER = cntVal;
}

uint32_t RIT_ReadCntVal(void )
{
    return (LPC_RIT->RICOUNTER);
}

/************************************************************
End of filename
************************************************************/
