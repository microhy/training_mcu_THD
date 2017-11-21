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
  Description: ��ʱ������ʱ��Ԥ��Ƶ
  Input      : 
     @param  LPC_TIMx : ��ʱ��0,1,2,3
       @arg LPC_TIM0,1,2,3
     @param  PRValue :Ԥ��Ƶֵ
       @arg 0 ~~ 2^32-1
  Output     : None
  Return     : 
  Others     : ��ֵLPC_TIMx->PR
************************************************************/       
void TIM_SetClockPRDiv(LPC_TIM_TypeDef* LPC_TIMx, uint32_t PRValue)
{
    LPC_TIMx->PR = PRValue;
}

/************************************************************
  Function   : TIM_GetITStatus()
  Description: ��ʱ���õ��ж�״̬
  Input      : 
     @param  LPC_TIMx : ��ʱ��0,1,2,3
       @arg LPC_TIM0,1,2,3
     @param  TIM_IT :�ж�Դ
       @arg TIM_IT_MR0,1,2,3: MR0��1��2��3��ƥ���ж�
       @arg TIM_IT_CR0,1 : CR0,1�Ĳ����ж�
  Output     : None
  Return     : bitstatus �ж�״̬
               RESET ���ж� SET���ж�
  Others     : ��ȡLPC_TIMx->IR
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
  Description: ��ʱ���ж����
  Input      : 
     @param  LPC_TIMx : ��ʱ��0,1,2,3
       @arg LPC_TIM0,1,2,3
     @param  TIM_IT :�ж�Դ
       @arg TIM_IT_MR0,1,2,3: MR0��1��2��3��ƥ���ж�
       @arg TIM_IT_CR0,1 : CR0,1�Ĳ����ж�
  Output     : None
  Return     : None
  Others     : �޸�LPC_TIMx->IR
************************************************************/
void TIM_ClearITPendingBit(LPC_TIM_TypeDef* LPC_TIMx, uint16_t TIM_IT)
{
    LPC_TIMx->IR |= (TIM_IT);
}

/************************************************************
  Function   : TIM_CountCmd()
  Description: ��ʱ������ʹ������
  Input      : 
   @param  LPC_TIMx : ��ʱ��0,1,2,3
     @arg LPC_TIM0,1,2,3
   @param  NewState: �µ�״̬����
     @arg ENABLE: ����ʹ��
     @arg DISABLE: �������ܣ�ֹͣ����
  Output     : None
  Return     : None
  Others     : ������LPC_TIMx->TCR
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
  Description: ��ʱ��������λ����
  Input      : 
   @param  LPC_TIMx : ��ʱ��0,1,2,3
     @arg LPC_TIM0,1,2,3
   @param  NewState: �µ�״̬����
     @arg ENABLE: ������λ
     @arg DISABLE: ���������λ����
  Output     : None
  Return     : None
  Others     : ������LPC_TIMx->TCR
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
  Description: TIM�������ƼĴ�������
  Input      : 
    @param capn ����ͨ��ѡ�� 
      @arg CAPn_0,CAPn_1
    @param cntMode ����ģʽ
      @arg TIMMode_Timer��ʱ��ģʽ
      @arg TIMMode_Rising�����ؼ���
      @arg TIMMode_Falling�½����ؼ���
      @arg TIMMode_Both˫���ؼ���
  Output     : None
  Return     : None
  Others     : �����ļĴ���: LPC_TIMx->CTCR
************************************************************/
void TIM_CntTimControl(LPC_TIM_TypeDef* LPC_TIMx, uint16_t capn, uint16_t cntMode)
{
    uint32_t tmpreg ;
    tmpreg = LPC_TIMx->CTCR;
    
    tmpreg &= 0xFFFFFFF0;   //clear bit0,1,2,3
    
    tmpreg |= cntMode;      //���ü�����ʽ

    if (capn != CAPn_0)
    {
        tmpreg |= CAPn_1;
    }
    LPC_TIMx->CTCR = tmpreg;
}

/************************************************************
  Function   : TIM_MatchControlCmd()
  Description: ��ʱ��ƥ���������
  Input      : 
   @param  LPC_TIMx : ��ʱ��0,1,2,3
     @arg LPC_TIM0,1,2,3
   @param  TIMMRx   : ƥ�䷢���¼�
     @arg TIM_MR0I,R,S: MR0�жϣ���λ��ֹͣ
     @arg TIM_MR1I,R,S: MR1�жϣ���λ��ֹͣ
     @arg TIM_MR2I,R,S: MR2�жϣ���λ��ֹͣ
     @arg TIM_MR3I,R,S: MR3�жϣ���λ��ֹͣ
   @param  NewState: �µ�״̬����
     @arg ENABLE: ʹ���ж�
     @arg DISABLE: �����ж�
  Output     : None
  Return     : None
  Others     : ������LPC_TIMx->MCR
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
  Description: ��ʱ���������
  Input      : 
   @param  LPC_TIMx : ��ʱ��0,1,2,3
     @arg LPC_TIM0,1,2,3
   @param  capn     : ���벶��Դ
     @arg CAPn_0,CAPn_1
   @param  capMode: ����ģʽ
     @arg TIMMode_Timer ����ʱ��ʹ��
     @arg TIMMode_Rising ����������
     @arg TIMMode_Falling ����������
     @arg TIMMode_Both ����˫����
  Output     : None
  Return     : None
  Others     : ������LPC_TIMx->CCR
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
        tmpreg |= ((uint32_t)capMode<<3);//�����λ
    }

    LPC_TIMx->CCR = tmpreg;
}

/************************************************************
  Function   : TIM_CaptureIntCmd()
  Description: ��ʱ�������ж�����
  Input      : 
   @param  LPC_TIMx : ��ʱ��0,1,2,3
     @arg LPC_TIM0,1,2,3
   @param  capn     : ���벶��Դ
     @arg CAPn_0,CAPn_1
   @param  NewState: �µ�״̬����
     @arg ENABLE: ʹ���ж�
     @arg DISABLE: �����ж�
  Output     : None
  Return     : None
  Others     : ������LPC_TIMx->CCR
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
  Description: �ⲿƥ����ƣ�������ƥ��ʱ��MATn.0123�����ϵ��¼�
  Input      : 
    LPC_TIMx : ��ʱ��0,1,2,3 
        LPC_TIM0,1,2,3
   TIM_EMCn_x: ƥ����� 0,1,2,3 ����Ӧ���¼� 
        TIM_EMC0_NO ��TC��MR0ƥ�䲻��ȡ�κβ���
        TIM_EMC0_L ��TC��MR0ƥ��ʱ����Ӧ���ⲿƥ��λ/�������Ϊ0
        TIM_EMC0_H ��TC��MR0ƥ��ʱ����Ӧ���ⲿƥ��λ/�������Ϊ1
        TIM_EMC0_TOG ��TC��MR0ƥ��ʱ����Ӧ���ⲿƥ��λ/�����ת
        TIM_EMC1,2,3��ͷ�ļ�����
  Output     : None
  Return     : None
  Others     : ������LPC_TIMx->EMR
************************************************************/
void TIM_ExtMatchControl(LPC_TIM_TypeDef* LPC_TIMx, uint32_t TIM_EMCn_x)
{
    uint16_t tmpreg;
    tmpreg = LPC_TIMx->EMR;
    
    tmpreg &= (uint16_t)(TIM_EMCn_x>>16);   //����
    tmpreg |= (uint16_t)TIM_EMCn_x;
    LPC_TIMx->EMR = tmpreg;
}

/************************************************************
End of filename
************************************************************/
