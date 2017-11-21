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
#include "../inc/fftAnalyse.h"

/**********Macros***********************/

/**********Variables********************/
volatile fftAnalyse_DataDef fftVal = {0};

volatile short psi_Input[NPOINTS * 2] = {0};
volatile short psi_Output[NPOINTS * 2] ={0};
volatile uint32_t adcSampVal[NPOINTS] = {0};


/**********Function Prototypes**********/
void vTest_PerformFFT(void)
{
	#ifdef NPOINTS
	{
		#if (NPOINTS == NPOINTS_64)
		{
			vF_dspl_fftR4b16N64(psi_Output, psi_Input);
		}
		#elif (NPOINTS == NPOINTS_256)
		{
			vF_dspl_fftR4b16N256(psi_Output, psi_Input);
		}
		#elif (NPOINTS == NPOINTS_1024)
		{
			vF_dspl_fftR4b16N1024(psi_Output, psi_Input);
		}
		#elif (NPOINTS == NPOINTS_4096)
		{
			vF_dspl_fftR4b16N4096(psi_Output, psi_Input);
		}
		#else
		{
			#error "NPOINTS Not Valid!"
		}
		#endif
	}
	#else
	{
		#error "NPOINTS Not Defined!"
	}
	#endif
}

void Measure_SignalPeriod(void )
{
    uint32_t i = 0,tmpPeriod=0; //tmp = 0x00,
    //fftVal.Signalperiod = 0;
    fftVal.Flag_Cap0Done = RESET;
    while(i<10)   //
    {
        if(fftVal.Flag_Cap0Done != RESET)
        {
            if(fftVal.cap0Val[1] > fftVal.cap0Val[0])
            {
                tmpPeriod = tmpPeriod + fftVal.cap0Val[1]-fftVal.cap0Val[0];
            }
//             else //
//             {
//                 tmpPeriod = tmpPeriod + 0XFFFFFFFF - fftVal.cap0Val[0]+ fftVal.cap0Val[1];
//             }
            ++i;
            fftVal.Flag_Cap0Done = RESET;   //������ϣ���λ�������λ
        }
    }
    fftVal.Signalperiod = tmpPeriod; // 100MHz��10������
    //fftVal.Signalperiod = (float)(tmpPeriod/1000); // 100Mhz����ʱ��ת��1us
//    fftVal.Flag_Cap0Done = SET;
}

void TIMER2_IRQHandler()
{
//    if(LPC_TIM2->IR&TIM_IT_CR0)  //CAP2.0���ж��Ƿ�����
    if(fftVal.Flag_Cap0 != SET)
    {
        fftVal.cap0Val[0] = LPC_TIM2->CR0;
        fftVal.Flag_Cap0 = SET;
    }
    else
    {
        fftVal.cap0Val[1] = LPC_TIM2->CR0;
        fftVal.Flag_Cap0 = RESET;
        fftVal.Flag_Cap0Done = SET;
    }
    LPC_TIM2->IR |= TIM_IT_CR0; //CAP2.0���ж�
    ++fftVal.CntEdge;
}

void PeripInit_TIM2_CAP0(void )
{
    TIM_CountResetCmd(LPC_TIM2, ENABLE);
    TIM_CountResetCmd(LPC_TIM2, DISABLE);
    TIM_CntTimControl(LPC_TIM2, CAPn_0, TIMMode_Timer);
    TIM_CaputreControl(LPC_TIM2, CAPn_0, TIMMode_Falling);//cap0�����½���
    TIM_CaptureIntCmd(LPC_TIM2, CAPn_0, ENABLE);  //�����ж�����
    TIM_CountCmd(LPC_TIM2, ENABLE);  //����TIM2
}

void PeripInit_TIM0_MAT1(void )
{
    TIM_CountResetCmd(LPC_TIM0, ENABLE);
    TIM_CountResetCmd(LPC_TIM0, DISABLE);
    TIM_ExtMatchControl(LPC_TIM0, TIM_EMC1_TOG);        //MAT0.1��ת������ADC����ʱ��
    TIM_MatchControlCmd(LPC_TIM0, TIM_MR1R, ENABLE);
}
/************************************************************
  Function   : ()
  Description: 
  Input      : None
  Output     : None
  Return     : None
  Others     : 
************************************************************/
void PeripInit_ADC(void )
{
    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_SamplePins = ADC_SamplePins_1;
    ADC_InitStruct.ADC_ClockFreq = 13000000;
    ADC_InitStruct.ADC_BurstConv = ADC_BurstConv_Disable;
    ADC_InitStruct.ADC_Mode = ADC_Mode_Opeartional;
    ADC_InitStruct.ADC_StartSignal = ADC_StartSignal_MAT01;
    ADC_InitStruct.ADC_StartEdge = ADC_StarteEdge_Rising;
    ADC_Init(&ADC_InitStruct);
    ADC_ITConfig(ADC_IT_GLOBDONE, DISABLE);
    ADC_ITConfig(ADC_IT_CHANNEL1, ENABLE);
}

void PeripInit_DMAChan2(void )
{
    DMA_InitTypeDef DMA_InitStruct;
    LLI_StructDef  LLI_adc;
    LLI_adc.source = (uint32_t) &(LPC_ADC->ADDR1);
    LLI_adc.destination = (uint32_t )adcSampVal;
    LLI_adc.next = (uint32_t)&LLI_adc;
    LLI_adc.control = DMA_SrcInc_Disable|DMA_DestInc_Enable|DMA_SWidth_Word|DMA_DWidth_Word|(uint32_t )NPOINTS;
    
    DMA_InitStruct.DMA_SrcAddr = (uint32_t) &(LPC_ADC->ADDR1);  //Դ��ַΪADDR1
    DMA_InitStruct.DMA_DestAddr = (uint32_t )adcSampVal;        //Ŀ���ַ
    DMA_InitStruct.DMA_LLI = (uint32_t )0;                     //��������ѭ�����
    DMA_InitStruct.DMA_TranSize = NPOINTS;              //�������ݵ��ܴ�С
    DMA_InitStruct.DMA_SBSize = DMA_SBSize_1;       //һ�δ���һ��
    DMA_InitStruct.DMA_DBSize = DMA_DBSize_1;
    DMA_InitStruct.DMA_SBWidth = DMA_SWidth_Word;   //���ݿ��Ϊ32bits
    DMA_InitStruct.DMA_DBWidth = DMA_DWidth_Word;
    DMA_InitStruct.DMA_SrcInc = DMA_SrcInc_Disable;   //Դ��ַ������
    DMA_InitStruct.DMA_DestInc = DMA_DestInc_Enable; //Ŀ���ַ����
    DMA_InitStruct.DMA_SrcPerip = Perip_ADC;    //ע���������ȷ����
    DMA_InitStruct.DMA_DestPerip = 0;           //none
    DMA_InitStruct.DMA_TranType = DMA_TranType_P2M;  //���赽�ڴ�
    DMA_ChanConfigInit(LPC_GPDMACH2, &DMA_InitStruct);
    DMA_ChanCmd(LPC_GPDMACH2, ENABLE);  //ʹ��ͨ��2
    
    DMA_ITTCMaskCmd(LPC_GPDMACH2, ENABLE);
    DMA_ITTCCmd(LPC_GPDMACH2, ENABLE);
}
/************************************************************
End of filename
************************************************************/
