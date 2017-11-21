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

    //DACʹ��DMAͨ������
    LLI_dac.source = (uint32_t )Xk;
    LLI_dac.destination = (uint32_t) &(LPC_DAC->DACR);
    LLI_dac.next = (uint32_t)&LLI_dac;
    LLI_dac.control = DMA_SrcInc_Enable|DMA_DestInc_Disable|DMA_SWidth_Word|DMA_DWidth_Word|512;
    
    DMA_InitStruct.DMA_SrcAddr = (uint32_t )Xk;             //Դ��ַ
    DMA_InitStruct.DMA_DestAddr = (uint32_t) &(LPC_DAC->DACR);  //Ŀ���ַΪDAC��ת���Ĵ���
    DMA_InitStruct.DMA_LLI = (uint32_t)&LLI_dac;                   //��������ѭ�����
    DMA_InitStruct.DMA_TranSize = 512;              //�������ݵ��ܴ���
    DMA_InitStruct.DMA_SBSize = DMA_SBSize_1;       //һ�δ���һ��
    DMA_InitStruct.DMA_DBSize = DMA_DBSize_1;
    DMA_InitStruct.DMA_SBWidth = DMA_SWidth_Word;   //���ݿ��Ϊ32bits
    DMA_InitStruct.DMA_DBWidth = DMA_DWidth_Word;
    DMA_InitStruct.DMA_SrcInc = DMA_SrcInc_Enable;   //Դ��ַ����
    DMA_InitStruct.DMA_DestInc = DMA_DestInc_Disable;//Ŀ���ַ������
    DMA_InitStruct.DMA_SrcPerip = 0;    //none
    DMA_InitStruct.DMA_DestPerip = Perip_DAC;        //ע���������ȷ����
    DMA_InitStruct.DMA_TranType = DMA_TranType_M2P;  //�ڴ浽����
    DMA_ChanConfigInit(LPC_GPDMACH3, &DMA_InitStruct);
    DMA_ChanCmd(LPC_GPDMACH3, ENABLE);  //ʹ��ͨ��1 
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
    //DACת���Ĵ�������
    LPC_DAC->DACR = ((DAC_InitStruct->DAC_AoutValue<<6) | DAC_InitStruct->DAC_MaxUpRate);
    //DACת������ֵ�Ĵ�������
    LPC_DAC->DACCNTVAL = DAC_InitStruct->DAC_TimloadVal;
    //DACת�����ƼĴ�������
    tmpreg = LPC_DAC->DACCTRL;
    tmpreg &= CTRL_CLR_Mask;
    tmpreg |= DAC_InitStruct->DAC_Doublebuf | DAC_InitStruct->DAC_TimOutCnt | 
              DAC_InitStruct->DAC_DmaAccess;
    LPC_DAC->DACCTRL = tmpreg;
}



/************************************************************
End of filename
************************************************************/
