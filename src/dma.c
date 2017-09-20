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
/*!< ͨ��DMA��ʹ��λ���� */
#define      GPDMA_EN_Set                    ((uint32_t)0x00000001)      /*BIT0 set*/
#define      GPDMA_EN_Reset                  ((uint32_t)0xFFFFFFFE)      /*BIT0 reset*/

/*!< DMAͨ��0~7�ļĴ���λ���� */
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
    //ADCʹ��DMAͨ������
    DMA_InitStruct.DMA_SrcAddr = (uint32_t) &(LPC_ADC->ADDR0);  //Դ��ַΪADDR0
    DMA_InitStruct.DMA_DestAddr = (uint32_t )adcSampVal;        //Ŀ���ַ
    DMA_InitStruct.DMA_LLI = 0;                     //��������ѭ�����
    DMA_InitStruct.DMA_TranSize = 256;              //�������ݵ��ܴ�СΪ256��
    DMA_InitStruct.DMA_SBSize = DMA_SBSize_1;       //һ�δ���һ��
    DMA_InitStruct.DMA_DBSize = DMA_DBSize_1;
    DMA_InitStruct.DMA_SBWidth = DMA_SWidth_Word;   //���ݿ��Ϊ32bits
    DMA_InitStruct.DMA_DBWidth = DMA_DWidth_Word;
    DMA_InitStruct.DMA_SrcInc = DMA_SrcInc_Disable;   //Դ��ַ����
    DMA_InitStruct.DMA_DestInc = DMA_DestInc_Enable;//Ŀ���ַ������
    DMA_InitStruct.DMA_SrcPerip = Perip_ADC;    //ע���������ȷ����
    DMA_InitStruct.DMA_DestPerip = 0;           //none
    DMA_InitStruct.DMA_TranType = DMA_TranType_P2M;  //�ڴ浽����
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
  Description: ʹ�� �� ���� DMA ����ͨ��
  Input      : 
   @param  NewState: �µ�״̬����
     @arg ENABLE: ʹ��DMA
     @arg DISABLE: ����DMA
  Output     : ʹ�� �� ���� DMA ����ͨ��
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
  Description: ʹ�� �� ���� DMA ���е�ĳһͨ��
  Input      : 
   @param  LPC_GPDMACHx: ͨ������
     @arg LPC_GPDMACH0~7 :ͨ��0~7
   @param  NewState: �µ�״̬����
     @arg ENABLE: ʹ�ܸ�ͨ��
     @arg DISABLE: ���ܸ�ͨ��
  Output     : ʹ�� �� ���� DMA ĳһͨ��
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
        LPC_GPDMACHx->DMACCConfig |= CCONFIG_HALT_Set;       //��λHALT�����Ժ����DMA����
        while(LPC_GPDMACHx->DMACCConfig&CCONFIG_ACTIVE_Set); //�ȴ�ACTIVE==0
        LPC_GPDMACHx->DMACCConfig &= CCONFIG_EN_Reset;
    }
}

/************************************************************
  Function   : DMA_ITTCCmd()
  Description: ʹ�� �� ���� DMA��ĳһͨ�����ն˼����ж�
  Input      : 
   @param  LPC_GPDMACHx: ͨ������
     @arg LPC_GPDMACH0~7 :ͨ��0~7
   @param  NewState: �µ�״̬����
     @arg ENABLE: ʹ�ܸ�ͨ���ն˼����ж�
     @arg DISABLE: ���ܸ�ͨ���ն˼����ж�
  Output     : ʹ�� �� ���� DMAͨ���ն˼����ж�
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
  Description: �жϴ�������
  Input      : 
   @param  LPC_GPDMACHx: ͨ������
     @arg LPC_GPDMACH0~7 :ͨ��0~7
   @param  NewState: �µ�״̬����
     @arg ENABLE: ʹ���жϴ�������
     @arg DISABLE: ���ܸ�ͨ���жϴ�������
  Output     : ʹ�� �� ���� DMAͨ���жϴ�������
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
  Description: �ն˼����ж�����
  Input      : 
   @param  LPC_GPDMACHx: ͨ������
     @arg LPC_GPDMACH0~7 :ͨ��0~7
   @param  NewState: �µ�״̬����
     @arg ENABLE: ʹ���ն˼����ж�����
     @arg DISABLE: ���ܸ�ͨ���ն˼����ж�����
  Output     : ʹ�� �� ���� DMAͨ���ն˼����ж�����
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
  Description: DMA�������ͬ������
  Input      : 
   @param Perip_x:
      @arg Perip_SSP0TX
      @arg ...��λ��ͷ�ļ�
   @param  NewState: �µĿ�������
     @arg ENABLE: ʹ�ܸ������źŵ�ͬ��
     @arg DISABLE: ���ܸø������źŵ�ͬ��
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
    @param DMA_SELx_x: ѡ��ͨ����������Դ
      @arg DMA_SEL08_TX0 : ͨ��8������Դ��Ϊ UART0 TX
      @arg DMA_SEL08_MAT00 : ͨ��8������Դ��Ϊ MAT0.1
      @arg ... �붨λ��ͷ�ļ�
  Output     : None
  Return     : None
  Others     : �üĴ���λ��SCϵͳ������
************************************************************/
void DMA_ReqSelConfig(uint32_t DMA_SELx_x)
{
    if(DMA_SELx_x&BIT31)
    {
        LPC_SC->DMAREQSEL &= DMA_SELx_x; // �����λ��ͨ������Դ����ΪUART..
    }
    else
    {
        LPC_SC->DMAREQSEL |= DMA_SELx_x; // ��λ��ͨ������Դ����ΪMAT..
    }
}

/************************************************************
  Function   : DMA_GetITStatus()
  Description: �õ��ж�״̬
  Input      : 
    @param DMA_IT_x :�ж�����
      @arg DMA_IT_ST0~7 : ���κ�DMAͨ���жϵ�״̬
      @arg DMA_IT_TC0~7 : DMAͨ���ն˼����ж������״̬
      @arg DMA_IT_ERR0~7: DMAͨ�����жϴ���״̬
  Output     : None
  Return     : SET (��λ���ж�) or RESET(��λ���ж�)
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
  Description: ����жϵȴ�λ
  Input      : 
    @param DMA_IT_x :�ж�����
      @arg DMA_IT_TC0~7 : DMAͨ���ն˼����ж������״̬
      @arg DMA_IT_ERR0~7: DMAͨ�����жϴ���״̬
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
        LPC_GPDMA->DMACIntTCClear = tmp;    //д��0��Ч���ʲ��ð�λ��
    }
    else if( tmp & BIT29)   // bit29==1 ; DMA_IT_ERRx
    {
        tmp &= 0XDFFFFFFF;  // clear bit29
        LPC_GPDMA->DMACIntErrClr = tmp;     //д��0��Ч���ʲ��ð�λ��
    }
    else ;
}

/************************************************************
End of filename
************************************************************/
