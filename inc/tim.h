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
#ifndef __TIM_H
#define __TIM_H

#ifdef __cplusplus
    extern "C" {
#endif 
/**********Includes*********************/
#include "../inc/LPC1768GCZX.h"       
// #include ".h"
        
/**********Macros***********************/
/*!< 定时器中断源定义 */
#define     TIM_IT_MR0      ((uint16_t) 0x0001)
#define     TIM_IT_MR1      ((uint16_t) 0x0002)
#define     TIM_IT_MR2      ((uint16_t) 0x0004)
#define     TIM_IT_MR3      ((uint16_t) 0x0008)

#define     TIM_IT_CR0      ((uint16_t) 0x0010)
#define     TIM_IT_CR1      ((uint16_t) 0x0020)
#define     TIM_IT_ALL      ((uint16_t) 0x003F)
/**********Variables********************/
/*!< 计数捕获源 */
#define    CAPn_0       ((uint16_t) 0x0000)
#define    CAPn_1       ((uint16_t) 0x0004)

/*!< 模式定义 */
#define    TIMMode_Timer        ((uint16_t) 0x0000)
#define    TIMMode_Rising       ((uint16_t) 0x0001)
#define    TIMMode_Falling      ((uint16_t) 0x0002)
#define    TIMMode_Both         ((uint16_t) 0x0003)

/*!< Match Control Register  */
#define    TIM_MR0I             ((uint16_t) 0x0001)
#define    TIM_MR0R             ((uint16_t) 0x0002)
#define    TIM_MR0S             ((uint16_t) 0x0004)

#define    TIM_MR1I             ((uint16_t) 0x0008) 
#define    TIM_MR1R             ((uint16_t) 0x0010) 
#define    TIM_MR1S             ((uint16_t) 0x0020) 

#define    TIM_MR2I             ((uint16_t) 0x0040) 
#define    TIM_MR2R             ((uint16_t) 0x0080) 
#define    TIM_MR2S             ((uint16_t) 0x0100) 

#define    TIM_MR3I             ((uint16_t) 0x0200) 
#define    TIM_MR3R             ((uint16_t) 0x0400) 
#define    TIM_MR3S             ((uint16_t) 0x0800)

/*!<  External Match Register */
/*数据如下定义，高16位是为了将对应的控制位与运算清零
 低16位是为了或运算将对应的位置1 来达到相应效果
*/
#define    TIM_EMC0_NO          ((uint32_t) 0xFFCF0000)
#define    TIM_EMC0_L           ((uint32_t) 0xFFCF0010)
#define    TIM_EMC0_H           ((uint32_t) 0xFFCF0020)
#define    TIM_EMC0_TOG         ((uint32_t) 0xFFCF0030)

#define    TIM_EMC1_NO          ((uint32_t) 0xFF3F0000)
#define    TIM_EMC1_L           ((uint32_t) 0xFF3F0040)
#define    TIM_EMC1_H           ((uint32_t) 0xFF3F0080)
#define    TIM_EMC1_TOG         ((uint32_t) 0xFF3F00C0)

#define    TIM_EMC2_NO          ((uint32_t) 0xFCFF0000)
#define    TIM_EMC2_L           ((uint32_t) 0xFCFF0100)
#define    TIM_EMC2_H           ((uint32_t) 0xFCFF0200)
#define    TIM_EMC2_TOG         ((uint32_t) 0xFCFF0300)

#define    TIM_EMC3_NO          ((uint32_t) 0xF3FF0000)
#define    TIM_EMC3_L           ((uint32_t) 0xF3FF0400)
#define    TIM_EMC3_H           ((uint32_t) 0xF3FF0800)
#define    TIM_EMC3_TOG         ((uint32_t) 0xF3FF0C00)

/**********Function Prototypes**********/
void TIM_ClearTC_PC(LPC_TIM_TypeDef* LPC_TIMx);
void TIM_SetClockPRDiv(LPC_TIM_TypeDef* LPC_TIMx, uint32_t PRValue);
ITStatus TIM_GetITStatus(LPC_TIM_TypeDef* LPC_TIMx, uint16_t TIM_IT);
void TIM_ClearITPendingBit(LPC_TIM_TypeDef* LPC_TIMx, uint16_t TIM_IT);
void TIM_CountCmd(LPC_TIM_TypeDef* LPC_TIMx, FunctionalState NewState);
void TIM_CountResetCmd(LPC_TIM_TypeDef* LPC_TIMx, FunctionalState NewState);
void TIM_CntTimControl(LPC_TIM_TypeDef* LPC_TIMx, uint16_t capn, uint16_t cntMode);
void TIM_MatchControlCmd(LPC_TIM_TypeDef* LPC_TIMx, uint16_t TIMMRx, FunctionalState NewState);
void TIM_CaputreControl(LPC_TIM_TypeDef* LPC_TIMx, uint16_t capn, uint16_t capMode);
void TIM_CaptureIntCmd(LPC_TIM_TypeDef* LPC_TIMx, uint16_t capn, FunctionalState NewState);
void TIM_ExtMatchControl(LPC_TIM_TypeDef* LPC_TIMx, uint32_t TIM_EMCn_x);

#ifdef __cplusplus
}
#endif

#endif
/************************************************************
End of filename
************************************************************/
