/************************************************************
    Copyright (C), 2013, Hnist FS_GCZX.
    
    FileName: 
    Author:     Version :       Date:
    Description:
    Function List: 
    1.
    History:
    <author>    <time>      <version >      <desc>
    David           1.0         build this moudle
************************************************************/

/**********Includes*********************/
#include "../inc/dma.h" 

/**********Macros***********************/
/*!< 通用DMA总使能位定义 */
#define      GPDMA_EN_Set                    ((uint32_t)0x00000001)      /*BIT0 set*/
#define      GPDMA_EN_Reset                  ((uint32_t)0xFFFFFFFE)      /*BIT0 reset*/

/*!< DMA通道0~7的寄存器位定义 */
/*!< DMA channel control registers (DMACCxControl - 0x5000 41xC)*/
#define      CCONTROL_CLR_Mask               ((uint32_t)0x0CFFFFFF)      /*BIT31:28,25:24 mask*/

#define      CCONTROL_ITTC_Set               ((uint32_t)0x80000000)      /*BIT31 set*/
#define      CCONTROL_ITTC_Reset             ((uint32_t)0x7FFFFFFF)      /*BIT31 reset*/

/*!< DMA Channel Configuration registers (DMACCxConfig - 0x5000 41x0)*/
#define      CCONFIG_CLR_Mask                ((uint32_t)0x0007C001)      /*BIT25:24 mask*/

#define      CCONFIG_EN_Set                  ((uint32_t)0x00000001)      /*BIT0 set*/
#define      CCONFIG_EN_Reset                ((uint32_t)0xFFFFFFFE)      /*BIT0 reset*/

#define      CCONFIG_IEMask_Set              ((uint32_t)0x00004000)      /*BIT14 set*/
#define      CCONFIG_IEMask_Reset            ((uint32_t)0xFFFFBFFF)      /*BIT14 reset*/

#define      CCONFIG_ITCMask_Set             ((uint32_t)0x00008000)      /*BIT15 set*/
#define      CCONFIG_ITCMask_Reset           ((uint32_t)0xFFFF7FFF)      /*BIT15 reset*/

//#define      CCONFIG_LOCK_Set                ((uint32_t)0x00010000)      /*BIT16 set*/
//#define      CCONFIG_LOCK_Reset              ((uint32_t)0xFFFEFFFF)      /*BIT16 reset*/

#define      CCONFIG_ACTIVE_Set              ((uint32_t)0x00020000)      /*BIT17 set*/

#define      CCONFIG_HALT_Set                ((uint32_t)0x00040000)      /*BIT18 set*/
#define      CCONFIG_HALT_Reset              ((uint32_t)0xFFFBFFFE)      /*BIT18 reset*/
/**********Variables********************/

/**********Function Prototypes**********/
/************************************************************
  Function   : ()
  Description: 
  Input      : None
  Output     : None
  Return     : None
  Others     : 
    //ADC使用DMA通道配置
    DMA_InitStruct.DMA_SrcAddr = (uint32_t) &(LPC_ADC->ADDR0);  //源地址为ADDR0
    DMA_InitStruct.DMA_DestAddr = (uint32_t )adcSampVal;        //目标地址
    DMA_InitStruct.DMA_LLI = 0;                     //链表用于循环输出
    DMA_InitStruct.DMA_TranSize = 256;              //传送数据的总大小为256次
    DMA_InitStruct.DMA_SBSize = DMA_SBSize_1;       //一次传送一个
    DMA_InitStruct.DMA_DBSize = DMA_DBSize_1;
    DMA_InitStruct.DMA_SBWidth = DMA_SWidth_Word;   //数据宽度为32bits
    DMA_InitStruct.DMA_DBWidth = DMA_DWidth_Word;
    DMA_InitStruct.DMA_SrcInc = DMA_SrcInc_Disable;   //源地址递增
    DMA_InitStruct.DMA_DestInc = DMA_DestInc_Enable;//目标地址不递增
    DMA_InitStruct.DMA_SrcPerip = Perip_ADC;    //注意外设的正确配置
    DMA_InitStruct.DMA_DestPerip = 0;           //none
    DMA_InitStruct.DMA_TranType = DMA_TranType_P2M;  //内存到外设
    DMA_ChanConfigInit(LPC_GPDMACH2, &DMA_InitStruct);
************************************************************/
void DMA_ChanConfigInit(LPC_GPDMACH_TypeDef* LPC_GPDMACHx, DMA_InitTypeDef* DMA_InitStruct)
{
    uint32_t tmpreg = 0x00;
    
    tmpreg = LPC_GPDMACHx->DMACCControl;
    tmpreg &= CCONTROL_CLR_Mask;
    
    tmpreg |= DMA_InitStruct->DMA_TranSize | DMA_InitStruct->DMA_SBSize | 
             DMA_InitStruct->DMA_DBSize | DMA_InitStruct->DMA_SBWidth | 
             DMA_InitStruct->DMA_DBWidth | DMA_InitStruct->DMA_SrcInc |
             DMA_InitStruct->DMA_DestInc;
    
    LPC_GPDMACHx->DMACCControl = tmpreg;
    
    tmpreg = LPC_GPDMACHx->DMACCConfig;
    tmpreg &= CCONFIG_CLR_Mask;
    
    tmpreg |= DMA_InitStruct->DMA_TranType | DMA_InitStruct->DMA_SrcPerip | 
             (DMA_InitStruct->DMA_DestPerip<<5);
             
    LPC_GPDMACHx->DMACCConfig = tmpreg;
    LPC_GPDMACHx->DMACCSrcAddr = DMA_InitStruct->DMA_SrcAddr;
    LPC_GPDMACHx->DMACCDestAddr = DMA_InitStruct->DMA_DestAddr;
    LPC_GPDMACHx->DMACCLLI = DMA_InitStruct->DMA_LLI;
    
}

/************************************************************
  Function   : DMA_ConfigCmd()
  Description: 使能 或 禁能 DMA 所有通道
  Input      : 
   @param  NewState: 新的状态命令
     @arg ENABLE: 使能DMA
     @arg DISABLE: 禁能DMA
  Output     : 使能 或 禁能 DMA 所有通道
  Return     : None
  Others     : LPC_GPDMA->DMACConfig
************************************************************/
void DMA_ConfigCmd(FunctionalState NewState)
{
    if( NewState!= DISABLE)
    {
        LPC_GPDMA->DMACConfig |= GPDMA_EN_Set;
    }
    else
    {
        LPC_GPDMA->DMACConfig &= GPDMA_EN_Reset;
    }
}
/************************************************************
  Function   : DMA_ChanCmd()
  Description: 使能 或 禁能 DMA 其中的某一通道
  Input      : 
   @param  LPC_GPDMACHx: 通道名称
     @arg LPC_GPDMACH0~7 :通道0~7
   @param  NewState: 新的状态命令
     @arg ENABLE: 使能该通道
     @arg DISABLE: 禁能该通道
  Output     : 使能 或 禁能 DMA 某一通道
  Return     : None
  Others     : LPC_GPDMACHx->DMACCConfig
************************************************************/
void DMA_ChanCmd(LPC_GPDMACH_TypeDef* LPC_GPDMACHx, FunctionalState NewState)
{
    if( NewState!= DISABLE)
    {
        LPC_GPDMACHx->DMACCConfig |= CCONFIG_EN_Set;
    }
    else
    {
        LPC_GPDMACHx->DMACCConfig |= CCONFIG_HALT_Set;       //置位HALT，忽略后面的DMA请求
        while(LPC_GPDMACHx->DMACCConfig&CCONFIG_ACTIVE_Set); //等待ACTIVE==0
        LPC_GPDMACHx->DMACCConfig &= CCONFIG_EN_Reset;
    }
}

/************************************************************
  Function   : DMA_ITTCCmd()
  Description: 使能 或 禁能 DMA中某一通道的终端计数中断
  Input      : 
   @param  LPC_GPDMACHx: 通道名称
     @arg LPC_GPDMACH0~7 :通道0~7
   @param  NewState: 新的状态命令
     @arg ENABLE: 使能该通道终端计数中断
     @arg DISABLE: 禁能该通道终端计数中断
  Output     : 使能 或 禁能 DMA通道终端计数中断
  Return     : None
  Others     : LPC_GPDMACHx->DMACCControl
************************************************************/
void DMA_ITTCCmd(LPC_GPDMACH_TypeDef* LPC_GPDMACHx, FunctionalState NewState)
{
    if( NewState!= DISABLE)
    {
        LPC_GPDMACHx->DMACCControl |= CCONTROL_ITTC_Set;
    }
    else
    {
        LPC_GPDMACHx->DMACCControl &= CCONTROL_ITTC_Reset;
    }
}

/************************************************************
  Function   : DMA_ITERRMaskCmd
  Description: 中断错误屏蔽
  Input      : 
   @param  LPC_GPDMACHx: 通道名称
     @arg LPC_GPDMACH0~7 :通道0~7
   @param  NewState: 新的状态命令
     @arg ENABLE: 使能中断错误屏蔽
     @arg DISABLE: 禁能该通道中断错误屏蔽
  Output     : 使能 或 禁能 DMA通道中断错误屏蔽
  Return     : None
  Others     : LPC_GPDMACHx->DMACCConfig
************************************************************/
void DMA_ITERRMaskCmd(LPC_GPDMACH_TypeDef* LPC_GPDMACHx, FunctionalState NewState)
{
    if( NewState!= DISABLE)
    {
        LPC_GPDMACHx->DMACCConfig |= CCONFIG_IEMask_Set;
    }
    else
    {
        LPC_GPDMACHx->DMACCConfig &= CCONFIG_IEMask_Set;
    }
}

/************************************************************
  Function   : DMA_ITERRMaskCmd
  Description: 终端计数中断屏蔽
  Input      : 
   @param  LPC_GPDMACHx: 通道名称
     @arg LPC_GPDMACH0~7 :通道0~7
   @param  NewState: 新的状态命令
     @arg ENABLE: 使能终端计数中断屏蔽
     @arg DISABLE: 禁能该通道终端计数中断屏蔽
  Output     : 使能 或 禁能 DMA通道终端计数中断屏蔽
  Return     : None
  Others     : LPC_GPDMACHx->DMACCConfig
************************************************************/
void DMA_ITTCMaskCmd(LPC_GPDMACH_TypeDef* LPC_GPDMACHx, FunctionalState NewState)
{
    if( NewState!= DISABLE)
    {
        LPC_GPDMACHx->DMACCConfig |= CCONFIG_ITCMask_Set;
    }
    else
    {
        LPC_GPDMACHx->DMACCConfig &= CCONFIG_ITCMask_Set;
    }
}
// FunctionalState DMA_GetChanEnStatus(uint32_t DMA_CHANNELx)
// {
//     FunctionalState currState = DISABLE;
//     if( LPC_GPDMA->DMACEnbldChns&DMA_CHANNELx)
//     {
//          currState = ENABLE;
//     }
//     return (currState);
// }
/************************************************************
  Function   : DMA_ReqSyncCmd()
  Description: DMA请求控制同步命令
  Input      : 
   @param Perip_x:
      @arg Perip_SSP0TX
      @arg ...定位至头文件
   @param  NewState: 新的控制命令
     @arg ENABLE: 使能该请求信号的同步
     @arg DISABLE: 禁能该该请求信号的同步
  Output     : None
  Return     : None
  Others     : LPC_GPDMA->DMACSync
************************************************************/
void DMA_ReqSyncCmd(uint32_t Perip_x, FunctionalState NewState)
{
    if( NewState!= DISABLE)
    {
        LPC_GPDMA->DMACSync |= (Perip_x>>1);
    }
    else
    {
        LPC_GPDMA->DMACSync &=~(Perip_x>>1);
    }
}
/************************************************************
  Function   : DMA_ReqSelConfig()
  Description: 
  Input      : 
    @param DMA_SELx_x: 选择通道及其请求源
      @arg DMA_SEL08_TX0 : 通道8的请求源设为 UART0 TX
      @arg DMA_SEL08_MAT00 : 通道8的请求源设为 MAT0.1
      @arg ... 请定位到头文件
  Output     : None
  Return     : None
  Others     : 该寄存器位于SC系统控制中
************************************************************/
void DMA_ReqSelConfig(uint32_t DMA_SELx_x)
{
    if(DMA_SELx_x&BIT31)
    {
        LPC_SC->DMAREQSEL &= DMA_SELx_x; // 清零该位，通道请求源设置为UART..
    }
    else
    {
        LPC_SC->DMAREQSEL |= DMA_SELx_x; // 置位，通道请求源设置为MAT..
    }
}

/************************************************************
  Function   : DMA_GetITStatus()
  Description: 得到中断状态
  Input      : 
    @param DMA_IT_x :中断类型
      @arg DMA_IT_ST0~7 : 屏蔽后DMA通道中断的状态
      @arg DMA_IT_TC0~7 : DMA通道终端计数中断请求的状态
      @arg DMA_IT_ERR0~7: DMA通道的中断错误状态
  Output     : None
  Return     : SET (该位有中断) or RESET(该位无中断)
  Others     : 
************************************************************/
ITStatus DMA_GetITStatus(uint32_t DMA_IT_x)
{
    uint32_t tmp = DMA_IT_x;
    
    if( tmp & BIT28)        // bit28 ==1 ; DMA_IT_TCx
    {
        tmp &= 0XEFFFFFFF;  // clear bit28
        tmp &=LPC_GPDMA->DMACIntTCStat;
    }
    else if( tmp & BIT29)   // bit29==1 ; DMA_IT_ERRx
    {
        tmp &= 0XDFFFFFFF;  // clear bit29
        tmp &= LPC_GPDMA->DMACIntErrStat;
    }
    else                    // bit28 == bit29 ==0 ; DMA_IT_STx
    {
        tmp &= LPC_GPDMA->DMACIntStat;
    }
    if( tmp )
    {
        return (SET);
    }
    else
    {
        return (RESET);
    }
}

/************************************************************
  Function   : DMA_ClearITPendingBit()
  Description: 清楚中断等待位
  Input      : 
    @param DMA_IT_x :中断类型
      @arg DMA_IT_TC0~7 : DMA通道终端计数中断请求的状态
      @arg DMA_IT_ERR0~7: DMA通道的中断错误状态
  Output     : None
  Return     : None
  Others     : 
************************************************************/
void DMA_ClearITPendingBit(uint32_t DMA_IT_x)
{
    uint32_t tmp = DMA_IT_x;
    
    if( tmp & BIT28)        // bit28 ==1 ; DMA_IT_TCx
    {
        tmp &= 0XEFFFFFFF;  // clear bit28
        LPC_GPDMA->DMACIntTCClear = tmp;    //写入0无效，故不用按位或
    }
    else if( tmp & BIT29)   // bit29==1 ; DMA_IT_ERRx
    {
        tmp &= 0XDFFFFFFF;  // clear bit29
        LPC_GPDMA->DMACIntErrClr = tmp;     //写入0无效，故不用按位或
    }
    else ;
}

/************************************************************
End of filename
************************************************************/
