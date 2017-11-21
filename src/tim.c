/************************************************************
    Copyright (C), 2013, Hnist FS_GCZX.
    
    FileName: .c
    Author:  huyong   Version :       Date:
    Description:
    Function List: 
    1.
    History:
    <author>    <time>      <version >      <desc>
    David       96/10/12        1.0         build this moudle
************************************************************/

/**********Includes*********************/
#include "../inc/tim.h"

/**********Macros***********************/
#define     TCR_CE_Set             ((uint16_t) 0x0001)
#define     TCR_CE_Reset           ((uint16_t) 0xFFFE)

#define     TCR_CR_Set             ((uint16_t) 0x0002)
#define     TCR_CR_Reset           ((uint16_t) 0xFFFD)

#define     CCR_CAP0I_Set          ((uint16_t) 0x0004)
#define     CCR_CAP0I_Reset        ((uint16_t) 0xFFFB)

#define     CCR_CAP1I_Set          ((uint16_t) 0x0020)
#define     CCR_CAP1I_Reset        ((uint16_t) 0xFFDF)
/**********Variables********************/

/**********Function Prototypes**********/

/**********MAIN Routine*****************/

void TIM_ClearTC_PC(LPC_TIM_TypeDef* LPC_TIMx)
{
    LPC_TIMx->PC = (uint32_t)0;
    LPC_TIMx->TC = (uint32_t)0;
}

/************************************************************
  Function   : TIM_SetClockPRDiv()
  Description: 定时器设置时钟预分频
  Input      : 
     @param  LPC_TIMx : 定时器0,1,2,3
       @arg LPC_TIM0,1,2,3
     @param  PRValue :预分频值
       @arg 0 ~~ 2^32-1
  Output     : None
  Return     : 
  Others     : 赋值LPC_TIMx->PR
************************************************************/       
void TIM_SetClockPRDiv(LPC_TIM_TypeDef* LPC_TIMx, uint32_t PRValue)
{
    LPC_TIMx->PR = PRValue;
}

/************************************************************
  Function   : TIM_GetITStatus()
  Description: 定时器得到中断状态
  Input      : 
     @param  LPC_TIMx : 定时器0,1,2,3
       @arg LPC_TIM0,1,2,3
     @param  TIM_IT :中断源
       @arg TIM_IT_MR0,1,2,3: MR0，1，2，3的匹配中断
       @arg TIM_IT_CR0,1 : CR0,1的捕获中断
  Output     : None
  Return     : bitstatus 中断状态
               RESET 无中断 SET有中断
  Others     : 读取LPC_TIMx->IR
************************************************************/
ITStatus TIM_GetITStatus(LPC_TIM_TypeDef* LPC_TIMx, uint16_t TIM_IT)
{
    ITStatus bitstatus = RESET;
    if(LPC_TIMx->IR&TIM_IT)
    {
        bitstatus = SET;
    }
    
    return(bitstatus);
}

/************************************************************
  Function   : TIM_ClearITPendingBit()
  Description: 定时器中断清除
  Input      : 
     @param  LPC_TIMx : 定时器0,1,2,3
       @arg LPC_TIM0,1,2,3
     @param  TIM_IT :中断源
       @arg TIM_IT_MR0,1,2,3: MR0，1，2，3的匹配中断
       @arg TIM_IT_CR0,1 : CR0,1的捕获中断
  Output     : None
  Return     : None
  Others     : 修改LPC_TIMx->IR
************************************************************/
void TIM_ClearITPendingBit(LPC_TIM_TypeDef* LPC_TIMx, uint16_t TIM_IT)
{
    LPC_TIMx->IR |= (TIM_IT);
}

/************************************************************
  Function   : TIM_CountCmd()
  Description: 定时器计数使能命令
  Input      : 
   @param  LPC_TIMx : 定时器0,1,2,3
     @arg LPC_TIM0,1,2,3
   @param  NewState: 新的状态命令
     @arg ENABLE: 计数使能
     @arg DISABLE: 计数禁能，停止计数
  Output     : None
  Return     : None
  Others     : 操作：LPC_TIMx->TCR
************************************************************/
void TIM_CountCmd(LPC_TIM_TypeDef* LPC_TIMx, FunctionalState NewState)
{
    if(NewState != DISABLE)
    {
        LPC_TIMx->TCR |= TCR_CE_Set;
    }
    else
    {
        LPC_TIMx->TCR &= TCR_CE_Reset;
    }
}

/************************************************************
  Function   : TIM_CountResetCmd()
  Description: 定时器计数复位命令
  Input      : 
   @param  LPC_TIMx : 定时器0,1,2,3
     @arg LPC_TIM0,1,2,3
   @param  NewState: 新的状态命令
     @arg ENABLE: 计数复位
     @arg DISABLE: 清除计数复位命令
  Output     : None
  Return     : None
  Others     : 操作：LPC_TIMx->TCR
************************************************************/
void TIM_CountResetCmd(LPC_TIM_TypeDef* LPC_TIMx, FunctionalState NewState)
{
    if(NewState != DISABLE)
    {
        LPC_TIMx->TCR |= TCR_CR_Set;
    }
    else
    {
        LPC_TIMx->TCR &= TCR_CR_Reset;
    }
}

/************************************************************
  Function   : TIM_CntTimControl()
  Description: TIM计数控制寄存器配置
  Input      : 
    @param capn 计数通道选择 
      @arg CAPn_0,CAPn_1
    @param cntMode 计数模式
      @arg TIMMode_Timer定时器模式
      @arg TIMMode_Rising上升沿计数
      @arg TIMMode_Falling下降沿沿计数
      @arg TIMMode_Both双边沿计数
  Output     : None
  Return     : None
  Others     : 操作的寄存器: LPC_TIMx->CTCR
************************************************************/
void TIM_CntTimControl(LPC_TIM_TypeDef* LPC_TIMx, uint16_t capn, uint16_t cntMode)
{
    uint32_t tmpreg ;
    tmpreg = LPC_TIMx->CTCR;
    
    tmpreg &= 0xFFFFFFF0;   //clear bit0,1,2,3
    
    tmpreg |= cntMode;      //配置计数方式

    if (capn != CAPn_0)
    {
        tmpreg |= CAPn_1;
    }
    LPC_TIMx->CTCR = tmpreg;
}

/************************************************************
  Function   : TIM_MatchControlCmd()
  Description: 定时器匹配控制命令
  Input      : 
   @param  LPC_TIMx : 定时器0,1,2,3
     @arg LPC_TIM0,1,2,3
   @param  TIMMRx   : 匹配发生事件
     @arg TIM_MR0I,R,S: MR0中断，复位，停止
     @arg TIM_MR1I,R,S: MR1中断，复位，停止
     @arg TIM_MR2I,R,S: MR2中断，复位，停止
     @arg TIM_MR3I,R,S: MR3中断，复位，停止
   @param  NewState: 新的状态命令
     @arg ENABLE: 使能中断
     @arg DISABLE: 禁能中断
  Output     : None
  Return     : None
  Others     : 操作：LPC_TIMx->MCR
************************************************************/
void TIM_MatchControlCmd(LPC_TIM_TypeDef* LPC_TIMx, uint16_t TIMMRx, FunctionalState NewState)
{
    if(NewState != DISABLE)
    {
        LPC_TIMx->MCR |= TIMMRx;
    }
    else
    {
        LPC_TIMx->MCR &=~TIMMRx;
    }
}

/************************************************************
  Function   : TIM_CaputreControl()
  Description: 定时器捕获控制
  Input      : 
   @param  LPC_TIMx : 定时器0,1,2,3
     @arg LPC_TIM0,1,2,3
   @param  capn     : 输入捕获源
     @arg CAPn_0,CAPn_1
   @param  capMode: 捕获模式
     @arg TIMMode_Timer 做定时器使用
     @arg TIMMode_Rising 捕获上升沿
     @arg TIMMode_Falling 捕获下升沿
     @arg TIMMode_Both 捕获双边沿
  Output     : None
  Return     : None
  Others     : 操作：LPC_TIMx->CCR
************************************************************/
void TIM_CaputreControl(LPC_TIM_TypeDef* LPC_TIMx, uint16_t capn, uint16_t capMode)
{
    uint32_t tmpreg ;
    tmpreg = LPC_TIMx->CCR;
    
    if(capn==CAPn_0)
    {
        tmpreg &= 0xFFFFFFFC;    //clear bit0,1
        tmpreg |= capMode;
    }
    else
    {
        tmpreg &= 0xFFFFFFE7;    //clear bit3,4
        tmpreg |= ((uint32_t)capMode<<3);//移向高位
    }

    LPC_TIMx->CCR = tmpreg;
}

/************************************************************
  Function   : TIM_CaptureIntCmd()
  Description: 定时器捕获中断命令
  Input      : 
   @param  LPC_TIMx : 定时器0,1,2,3
     @arg LPC_TIM0,1,2,3
   @param  capn     : 输入捕获源
     @arg CAPn_0,CAPn_1
   @param  NewState: 新的状态命令
     @arg ENABLE: 使能中断
     @arg DISABLE: 禁能中断
  Output     : None
  Return     : None
  Others     : 操作：LPC_TIMx->CCR
************************************************************/
void TIM_CaptureIntCmd(LPC_TIM_TypeDef* LPC_TIMx, uint16_t capn, FunctionalState NewState)
{
    if( NewState != DISABLE )
    {
        if(capn == CAPn_0)
        {
            LPC_TIMx->CCR |= CCR_CAP0I_Set;
        }
        else
        {
            LPC_TIMx->CCR |= CCR_CAP1I_Set;
        }
    }
    else
    {
        if(capn == CAPn_0)
        {
            LPC_TIMx->CCR &= CCR_CAP0I_Reset;
        }
        else
        {
            LPC_TIMx->CCR &= CCR_CAP1I_Reset;
        }
    }
}

/************************************************************
  Function   : TIM_ExtMatchControl()
  Description: 外部匹配控制，当产生匹配时，MATn.0123引脚上的事件
  Input      : 
    LPC_TIMx : 定时器0,1,2,3 
        LPC_TIM0,1,2,3
   TIM_EMCn_x: 匹配控制 0,1,2,3 及对应的事件 
        TIM_EMC0_NO 当TC和MR0匹配不采取任何操作
        TIM_EMC0_L 当TC和MR0匹配时将对应的外部匹配位/输出设置为0
        TIM_EMC0_H 当TC和MR0匹配时将对应的外部匹配位/输出设置为1
        TIM_EMC0_TOG 当TC和MR0匹配时将对应的外部匹配位/输出翻转
        TIM_EMC1,2,3见头文件定义
  Output     : None
  Return     : None
  Others     : 操作：LPC_TIMx->EMR
************************************************************/
void TIM_ExtMatchControl(LPC_TIM_TypeDef* LPC_TIMx, uint32_t TIM_EMCn_x)
{
    uint16_t tmpreg;
    tmpreg = LPC_TIMx->EMR;
    
    tmpreg &= (uint16_t)(TIM_EMCn_x>>16);   //清零
    tmpreg |= (uint16_t)TIM_EMCn_x;
    LPC_TIMx->EMR = tmpreg;
}

/************************************************************
End of filename
************************************************************/
