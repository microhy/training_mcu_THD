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
#include "../inc/dac.h" 

/**********Macros***********************/
#define     CTRL_CLR_Mask       ((uint32_t)0X00000001)
/**********Variables********************/
extern uint32_t Xk[512];
/**********Function Prototypes**********/

/**********MAIN Routine*****************/

void PeripInit_DAC(void )
{
    DMA_InitTypeDef DMA_InitStruct;
    DAC_InitTypeDef DAC_InitStruct;
    LLI_StructDef  LLI_dac;

    //DAC使用DMA通道配置
    LLI_dac.source = (uint32_t )Xk;
    LLI_dac.destination = (uint32_t) &(LPC_DAC->DACR);
    LLI_dac.next = (uint32_t)&LLI_dac;
    LLI_dac.control = DMA_SrcInc_Enable|DMA_DestInc_Disable|DMA_SWidth_Word|DMA_DWidth_Word|512;
    
    DMA_InitStruct.DMA_SrcAddr = (uint32_t )Xk;             //源地址
    DMA_InitStruct.DMA_DestAddr = (uint32_t) &(LPC_DAC->DACR);  //目标地址为DAC的转换寄存器
    DMA_InitStruct.DMA_LLI = (uint32_t)&LLI_dac;                   //链表用于循环输出
    DMA_InitStruct.DMA_TranSize = 512;              //传送数据的总次数
    DMA_InitStruct.DMA_SBSize = DMA_SBSize_1;       //一次传送一个
    DMA_InitStruct.DMA_DBSize = DMA_DBSize_1;
    DMA_InitStruct.DMA_SBWidth = DMA_SWidth_Word;   //数据宽度为32bits
    DMA_InitStruct.DMA_DBWidth = DMA_DWidth_Word;
    DMA_InitStruct.DMA_SrcInc = DMA_SrcInc_Enable;   //源地址递增
    DMA_InitStruct.DMA_DestInc = DMA_DestInc_Disable;//目标地址不递增
    DMA_InitStruct.DMA_SrcPerip = 0;    //none
    DMA_InitStruct.DMA_DestPerip = Perip_DAC;        //注意外设的正确配置
    DMA_InitStruct.DMA_TranType = DMA_TranType_M2P;  //内存到外设
    DMA_ChanConfigInit(LPC_GPDMACH3, &DMA_InitStruct);
    DMA_ChanCmd(LPC_GPDMACH3, ENABLE);  //使能通道1 
    //DAC startup
    PeripheralsClock_Divider(PCLK_DAC, CCLK_DIV4);

    DAC_InitStruct.DAC_AoutValue = 0;
    DAC_InitStruct.DAC_MaxUpRate = DAC_MaxUpRate_1MHZ;
    DAC_InitStruct.DAC_Doublebuf = DAC_Doublebuf_Enable;
    DAC_InitStruct.DAC_TimOutCnt = DAC_TimOutCnt_Enable;
    DAC_InitStruct.DAC_TimloadVal = 5000;
    DAC_InitStruct.DAC_DmaAccess = DAC_DmaAccess_Enable;
    DAC_Init(&DAC_InitStruct);
}
/************************************************************
  Function   : ()
  Description: 
  Input      : None
  Output     : None
  Return     : None
  Others     : 
    DAC_InitTypeDef DAC_InitStruct;
    DAC_InitStruct.DAC_AoutValue = 0X1FF;
    DAC_InitStruct.DAC_MaxUpRate = DAC_MaxUpRate_1MHZ;
    DAC_InitStruct.DAC_Doublebuf = DAC_Doublebuf_Enable;
    DAC_InitStruct.DAC_TimOutCnt = DAC_TimOutCnt_Enable;
    DAC_InitStruct.DAC_TimloadVal = 100;
    DAC_InitStruct.DAC_DmaAccess = DAC_DmaAccess_Enable;
************************************************************/
void DAC_Init(DAC_InitTypeDef* DAC_InitStruct)
{
    uint32_t tmpreg = 0x00;
    //DAC转换寄存器配置
    LPC_DAC->DACR = ((DAC_InitStruct->DAC_AoutValue<<6) | DAC_InitStruct->DAC_MaxUpRate);
    //DAC转换计数值寄存器配置
    LPC_DAC->DACCNTVAL = DAC_InitStruct->DAC_TimloadVal;
    //DAC转换控制寄存器配置
    tmpreg = LPC_DAC->DACCTRL;
    tmpreg &= CTRL_CLR_Mask;
    tmpreg |= DAC_InitStruct->DAC_Doublebuf | DAC_InitStruct->DAC_TimOutCnt | 
              DAC_InitStruct->DAC_DmaAccess;
    LPC_DAC->DACCTRL = tmpreg;
}



/************************************************************
End of filename
************************************************************/
