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
#ifndef __DMA_H
#define __DMA_H

#ifdef __cplusplus
    extern "C" {
#endif 
/**********Includes*********************/
#include "../inc/LPC1768GCZX.h"
// #include ".h"

typedef struct
{
    uint32_t DMA_SrcAddr;
    uint32_t DMA_DestAddr;
    uint32_t DMA_LLI;
    uint32_t DMA_TranSize;
    uint32_t DMA_SBSize;
    uint32_t DMA_DBSize;
    uint32_t DMA_SBWidth;
    uint32_t DMA_DBWidth;
    uint32_t DMA_SrcInc;
    uint32_t DMA_DestInc;
//     uint32_t DMA_PortBits;
    uint32_t DMA_SrcPerip;
    uint32_t DMA_DestPerip;
    uint32_t DMA_TranType;
}DMA_InitTypeDef;

typedef struct 
{
    uint32_t  source;     // start of source area
    uint32_t  destination;// start of destination area
    uint32_t  next;       // address of next strLLI in chain
    uint32_t  control;    // DMACCxControl register
}LLI_StructDef;

/**********DMA 初始类型定义初始化配置定义***********************/
#define      DMA_SBSize_1                ((uint32_t)0x00000000)
#define      DMA_SBSize_4                ((uint32_t)0x00001000)
#define      DMA_SBSize_8                ((uint32_t)0x00002000)
#define      DMA_SBSize_16               ((uint32_t)0x00003000)
#define      DMA_SBSize_32               ((uint32_t)0x00004000)
#define      DMA_SBSize_64               ((uint32_t)0x00005000)
#define      DMA_SBSize_128              ((uint32_t)0x00006000)
#define      DMA_SBSize_256              ((uint32_t)0x00007000)
                                     
#define      DMA_DBSize_1                ((uint32_t)0x00000000)
#define      DMA_DBSize_4                ((uint32_t)0x00008000)
#define      DMA_DBSize_8                ((uint32_t)0x00010000)
#define      DMA_DBSize_16               ((uint32_t)0x00018000)
#define      DMA_DBSize_32               ((uint32_t)0x00020000)
#define      DMA_DBSize_64               ((uint32_t)0x00028000)
#define      DMA_DBSize_128              ((uint32_t)0x00030000)
#define      DMA_DBSize_256              ((uint32_t)0x00038000)
             
#define      DMA_SWidth_Byte             ((uint32_t)0x00000000)
#define      DMA_SWidth_HalfWord         ((uint32_t)0x00040000)
#define      DMA_SWidth_Word             ((uint32_t)0x00080000)
             
#define      DMA_DWidth_Byte             ((uint32_t)0x00000000)
#define      DMA_DWidth_HalfWord         ((uint32_t)0x00200000)
#define      DMA_DWidth_Word             ((uint32_t)0x00400000)
             
#define      DMA_SrcInc_Enable           ((uint32_t)0x04000000)
#define      DMA_SrcInc_Disable          ((uint32_t)0x00000000)
             
#define      DMA_DestInc_Enable          ((uint32_t)0x08000000)
#define      DMA_DestInc_Disable         ((uint32_t)0x00000000)
             
#define      DMA_PortMode_Privileged     ((uint32_t)0x10000000)
#define      DMA_PortMode_User           ((uint32_t)0x00000000)
             
#define      DMA_PortBuffer_Enable       ((uint32_t)0x20000000)
#define      DMA_PortBuffer_Disable      ((uint32_t)0x00000000)
             
#define      DMA_PortCache_Enable        ((uint32_t)0x40000000)
#define      DMA_PortCache_Disable       ((uint32_t)0x00000000)

#define      Perip_SSP0TX                ((uint32_t)0x00000000)
#define      Perip_SSP0RX                ((uint32_t)0x00000002)
#define      Perip_SSP1TX                ((uint32_t)0x00000004)
#define      Perip_SSP1RX                ((uint32_t)0x00000006)
#define      Perip_ADC                   ((uint32_t)0x00000008)
#define      Perip_I2S0                  ((uint32_t)0x0000000A)
#define      Perip_I2S1                  ((uint32_t)0x0000000C)
#define      Perip_DAC                   ((uint32_t)0x0000000E)
#define      Perip_UART0TX               ((uint32_t)0x00000010)
#define      Perip_MAT00                 ((uint32_t)0x00000010)
#define      Perip_UART0RX               ((uint32_t)0x00000012)
#define      Perip_MAT01                 ((uint32_t)0x00000012)
#define      Perip_UART1TX               ((uint32_t)0x00000014)
#define      Perip_MAT10                 ((uint32_t)0x00000014)
#define      Perip_UART1RX               ((uint32_t)0x00000016)
#define      Perip_MAT11                 ((uint32_t)0x00000016)
#define      Perip_UART2TX               ((uint32_t)0x00000018)
#define      Perip_MAT20                 ((uint32_t)0x00000018)
#define      Perip_UART2RX               ((uint32_t)0x0000001A)
#define      Perip_MAT21                 ((uint32_t)0x0000001A)
#define      Perip_UART3TX               ((uint32_t)0x0000001C)
#define      Perip_MAT30                 ((uint32_t)0x0000001C)
#define      Perip_UART3RX               ((uint32_t)0x0000001E)
#define      Perip_MAT31                 ((uint32_t)0x0000001E)

//     uint32_t DMA_DestPerip;
#define      DMA_TranType_M2M            ((uint32_t)0x00000000)
#define      DMA_TranType_M2P            ((uint32_t)0x00000800)
#define      DMA_TranType_P2M            ((uint32_t)0x00001000)
#define      DMA_TranType_P2P            ((uint32_t)0x00001800)

/*中断类型定义*/
#define      DMA_IT_ST0                  ((uint32_t )0X00000001)     /*!< bit28~31==0 */
#define      DMA_IT_TC0                  ((uint32_t )0X10000001)     /*!< bit28==1 */
#define      DMA_IT_ERR0                 ((uint32_t )0X20000001)     /*!< bit29==1 */
                                         
#define      DMA_IT_ST1                  ((uint32_t )0X00000002)
#define      DMA_IT_TC1                  ((uint32_t )0X10000002)
#define      DMA_IT_ERR1                 ((uint32_t )0X20000002)
                                         
#define      DMA_IT_ST2                  ((uint32_t )0X00000004)
#define      DMA_IT_TC2                  ((uint32_t )0X10000004)
#define      DMA_IT_ERR2                 ((uint32_t )0X20000004)
                                         
#define      DMA_IT_ST3                  ((uint32_t )0X00000008)
#define      DMA_IT_TC3                  ((uint32_t )0X10000008)
#define      DMA_IT_ERR3                 ((uint32_t )0X20000008)
                                         
#define      DMA_IT_ST4                  ((uint32_t )0X00000010)
#define      DMA_IT_TC4                  ((uint32_t )0X10000010)
#define      DMA_IT_ERR4                 ((uint32_t )0X20000010)
                                         
#define      DMA_IT_ST5                  ((uint32_t )0X00000020)
#define      DMA_IT_TC5                  ((uint32_t )0X10000020)
#define      DMA_IT_ERR5                 ((uint32_t )0X20000020)
                                         
#define      DMA_IT_ST6                  ((uint32_t )0X00000040)
#define      DMA_IT_TC6                  ((uint32_t )0X10000040)
#define      DMA_IT_ERR6                 ((uint32_t )0X20000040)
                                         
#define      DMA_IT_ST7                  ((uint32_t )0X00000080)
#define      DMA_IT_TC7                  ((uint32_t )0X10000080)
#define      DMA_IT_ERR7                 ((uint32_t )0X20000080)

/* 通道定义  */
// #define     DMA_CHANNEL0        ((uint32_t )0X00000001)
// #define     DMA_CHANNEL1        ((uint32_t )0X00000002)
// #define     DMA_CHANNEL2        ((uint32_t )0X00000004)
// #define     DMA_CHANNEL3        ((uint32_t )0X00000008)
// #define     DMA_CHANNEL4        ((uint32_t )0X00000010)
// #define     DMA_CHANNEL5        ((uint32_t )0X00000020)
// #define     DMA_CHANNEL6        ((uint32_t )0X00000040)
// #define     DMA_CHANNEL7        ((uint32_t )0X00000080)

/*通道8~15的UART和定时器DMA请求可通过寄存器DMAREQSEL来选择*/
#define      DMA_SEL08_TX0               ((uint32_t )0X00000001) /**/
#define      DMA_SEL08_MAT00             ((uint32_t )0XFFFFFFFE)
                                         
#define      DMA_SEL09_RX0               ((uint32_t )0X00000002)
#define      DMA_SEL09_MAT01             ((uint32_t )0XFFFFFFFD)
                                         
#define      DMA_SEL10_TX1               ((uint32_t )0X00000004)
#define      DMA_SEL10_MAT10             ((uint32_t )0XFFFFFFFB)
                                         
#define      DMA_SEL11_RX1               ((uint32_t )0X00000008)
#define      DMA_SEL11_MAT11             ((uint32_t )0XFFFFFFF7)
                                         
#define      DMA_SEL12_TX2               ((uint32_t )0X00000010)
#define      DMA_SEL12_MAT20             ((uint32_t )0XFFFFFFEF)
                                         
#define      DMA_SEL13_RX2               ((uint32_t )0X00000020)
#define      DMA_SEL13_MAT21             ((uint32_t )0XFFFFFFDF)
                                         
#define      DMA_SEL14_TX3               ((uint32_t )0X00000040)
#define      DMA_SEL14_MAT30             ((uint32_t )0XFFFFFFBF)
                                         
#define      DMA_SEL15_RX3               ((uint32_t )0X00000080)
#define      DMA_SEL15_MAT31             ((uint32_t )0XFFFFFF7F)

       
/**********Function Prototypes**********/
void DMA_ChanConfigInit(LPC_GPDMACH_TypeDef* LPC_GPDMACHx, DMA_InitTypeDef* DMA_InitStruct);
void DMA_ConfigCmd(FunctionalState NewState);
void DMA_ChanCmd(LPC_GPDMACH_TypeDef* LPC_GPDMACHx, FunctionalState NewState);
void DMA_ITTCCmd(LPC_GPDMACH_TypeDef* LPC_GPDMACHx, FunctionalState NewState);
void DMA_ITERRMaskCmd(LPC_GPDMACH_TypeDef* LPC_GPDMACHx, FunctionalState NewState);
void DMA_ITTCMaskCmd(LPC_GPDMACH_TypeDef* LPC_GPDMACHx, FunctionalState NewState);
void DMA_ReqSyncCmd(uint32_t Perip_x, FunctionalState NewState);
void DMA_ReqSelConfig(uint32_t DMA_SELx_x);
ITStatus DMA_GetITStatus(uint32_t DMA_IT_x);
void DMA_ClearITPendingBit(uint32_t DMA_IT_x);

#ifdef __cplusplus
}
#endif

#endif
/************************************************************
End of filename
************************************************************/
