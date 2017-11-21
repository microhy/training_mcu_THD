/************************************************************ 
        Copyright (C), 2013, Hnist FS_GCZX. 
    FileName: project.c 
    Author:Hnist FS_GCZX   Version: 13.11   Date: 2013.11.13
    Description: LPC1768����Դ����ģ�壬Դ�����Ӧͷ�ļ�Ӧ���н���
                 �Դ�ģ��Ϊ��˵����̸�ʽ������μ���̹淶
    Function List: 

    History:
    <author>  huyong 
    <time>    2014_06_20
    <version > V2014.06.20
    <desc> 200Hz~5KHz ���Ҳ������������ǲ�ʧ��ȷ�����LCDƵ����ʾ
    200Hz <=f <1250Hz : Fs=128f
        if(fftVal.SignalFreq<1250)   //200hz -- 5ms �����ʱ��5ms*2 = 10ms
        {
            TP = 2;  //���ɼ�256�㣬��2�����ڲ��ꡣ�� fs=128f; fsmax=128*1.25k = 160k<200k
        }
        else if(fftVal.SignalFreq<2500) //1.25khz --800us �����ʱ��800us*4=3.2ms
        {
            TP = 4;  //���ɼ�256�㣬��4�����ڲ��ꡣ�� fs=64f; fsmax=64*2.5k = 160k<200k
        }
        else  //2.5khz--400us �����ʱ��400us*8=3.2ms
        {
            TP = 8;  //���ɼ�256�㣬��8�����ڲ��ꡣ�� fs=32f; fsmax=32*5k = 160k<200k
***********************************************************/

// -----Includes----- //
#include "../inc/LPC1768GCZX.h" // �Զ���ͷ�ļ����μ���ͷ�ļ�ע�� //
#include "../inc/ili9320.h"
#include "../inc/ili9320_api.h"
#include "../inc/lpc_PGA2310.h"
// -----Macros------ //


// -----Function Prototypes----- //
void SysTick_Delay100ms(void );
void Hardware_Startup(void );
// -----Variables----- //
uint32_t Xk[NPOINTS/2] = {0};
uint8_t TP = 2;

int main(void)
{
    uint32_t i = 0;
    uint64_t SumUn;
    float derat = 0;
    GainAdjVal.Gain_ChanL = 192;
    GainAdjVal.Gain_ChanR = 192;
    SystemInit(); // ������system-LPC17xx.c�У���Ҫ���ϵͳʱ�ӳ�ʼ���ȹ��� //
/********************PeripInit**************************/
    Hardware_Startup();
    PeripInit_UART0();
    printf("UART0 init OK!\n");
    LCD_Config();
    PeripInit_ADC();
//    PeripInit_TIM0_MAT1();
//    PeripInit_DMAChan2();
    DMA_ConfigCmd(ENABLE);//ʹ��DMAģ��
    
    PGA2310_PortInit();
    PGA2310_GainConfig(192, 192);
/******************NVIC INTERRUPT ENABLE****************/
    NVIC_EnableIRQ(TIMER2_IRQn);
//     NVIC_EnableIRQ(RIT_IRQn);
//     NVIC_EnableIRQ(ADC_IRQn);
    
/*********************************************************/
    while(1)
    {   
        //���Ŵ����ڲ���Ƶ��
        PGA2310_GainConfig(255, 192);   //ֻ�Ŵ�һ��ͨ�������Ŵ�����ͨ�����������źűȽ���Ҳ���������ź�
        delayus((fftVal.Signalperiod/1000));
        delayus((fftVal.Signalperiod/1000));
        delayus((fftVal.Signalperiod/1000));//�ȴ��������ø���
        NVIC_EnableIRQ(TIMER2_IRQn);
        fftVal.CntEdge = 0;
        PeripInit_TIM2_CAP0();   //CAP2.0���ڲ�������
        Measure_SignalPeriod();  //������������
        ////���ڲ�����ϣ���λTIM2
        TIM_CaptureIntCmd(LPC_TIM2, CAPn_0, DISABLE);  //�����жϲ�����
        TIM_CountResetCmd(LPC_TIM2, ENABLE);
        TIM_ClearITPendingBit(LPC_TIM2, TIM_IT_CR0);
        NVIC_DisableIRQ(TIMER2_IRQn);
        fftVal.SignalFreq = 1000000000/fftVal.Signalperiod; //
        if(fftVal.SignalFreq<1250)   //200hz -- 5ms �����ʱ��5ms*2 = 10ms
        {
            TP = 2;  //���ɼ�256�㣬��2�����ڲ��ꡣ�� fs=128f; fsmax=128*1.25k = 160k<200k
        }
        else if(fftVal.SignalFreq<2500) //1.25khz --800us �����ʱ��800us*4=3.2ms
        {
            TP = 4;  //���ɼ�256�㣬��4�����ڲ��ꡣ�� fs=64f; fsmax=64*2.5k = 160k<200k
        }
        else  //2.5khz--400us �����ʱ��400us*8=3.2ms
        {
            TP = 8;  //���ɼ�256�㣬��8�����ڲ��ꡣ�� fs=32f; fsmax=32*5k = 160k<200k
        }
        derat = TP*fftVal.Signalperiod/(10*NPOINTS); //����10����Ϊ�ǲ�����ʱ�ۼ���10��
        printf("period = %f us\n",fftVal.Signalperiod/1000); //
        printf("fre = %f Hz\n\n",fftVal.SignalFreq); //
        //�ָ�ԭ�ź����ڲ���
        PGA2310_GainConfig(GainAdjVal.Gain_ChanL, GainAdjVal.Gain_ChanR);
        delayus((fftVal.Signalperiod/1000)); 
        delayus((fftVal.Signalperiod/1000));
        delayus((fftVal.Signalperiod/1000)); //�ȴ��������ø���
        PeripInit_TIM0_MAT1();
        TIM_CountResetCmd(LPC_TIM0, ENABLE);  //�����λTIM0
        LPC_TIM0->MR1 = derat/2;               //��ʼ��������
        TIM_CountResetCmd(LPC_TIM0, DISABLE);  //�����λTIM0
        TIM_CountCmd(LPC_TIM0, ENABLE);        //����TIM0����
        PeripInit_DMAChan2();
        
        delayms(15);
        //delayms(15);
        //while((LPC_ADC->ADSTAT&ADC_STATUS_DONE1)==0); //�ȴ����һ��ת������
        //ADC_StartConvCmd(DISABLE);  //������ϣ�ֹͣADCת��         
        LPC_TIM0->MR1 = 0;
        TIM_MatchControlCmd(LPC_TIM0, TIM_MR1R, DISABLE);//������ϣ�ʧ��ƥ��MAT0.1
        for( i =0; i<NPOINTS; i++)
        {
            psi_Input[i*2] = (uint16_t)adcSampVal[i]>>4;
            if(psi_Input[i*2] >3700)  //3700*3.3/4096 = 3.0V
            {
                if(i>0&&i<NPOINTS-1)
                {
                    psi_Input[i*2] = (psi_Input[i*2+2]+psi_Input[i*2-2])/2;
                }
                else if(i==0)
                {
                    psi_Input[0] = (psi_Input[2] + psi_Input[2*NPOINTS-2])/2;
                }
                else
                {   psi_Input[2*NPOINTS-2] = (psi_Input[2*NPOINTS-4] + psi_Input[0])/2;
                }
            }
            if(psi_Input[i*2] > fftVal.adcMostMax)//�����ֵ�������ֵ
            {
                fftVal.adcMostMax = psi_Input[i*2];
                fftVal.adcMostMax_num = i*2;
            }
            else if(psi_Input[i*2] > fftVal.adcMoreMax)  //����ֵ���ڴ����ֵ
            {
                fftVal.adcMoreMax = psi_Input[i*2];
            }
        }
        GainAdjVal.ADC0Value_cur = fftVal.adcMostMax - 2048;    //1.65*4096/3.3 = 2048����ȥֱ����;
        printf("adcMoreMax = %d\n", fftVal.adcMoreMax);
        printf("adcMostMax = %d\n\n", fftVal.adcMostMax);
                                 
        for( i=0; i<NPOINTS*2; i = i+2)
        {
            printf("%d\n", psi_Input[i ]);
        }
        vTest_PerformFFT();
        for( i =0; i<NPOINTS/2; i++) // 0~NPOINTS/2-1
        {
            Xk[i] = (uint32_t)(pow(psi_Output[2 * i], 2) + pow(psi_Output[(2 * i) + 1], 2));
//            printf("%d + %d*i\n",psi_Output[2 * i],psi_Output[(2 * i) + 1]);
        }
        SumUn = 0;  //����
        for(i=2*TP; i<11*TP; i+=TP) //ֻ������10��г������
        {
            SumUn += Xk[i];
        }
        fftVal.SignalTHD = 100*sqrt((double)SumUn/Xk[TP]);
        printf("SumUn = %d\n",SumUn);
        printf("thd = %f\n",fftVal.SignalTHD);
        
        PGA2310_GainAdjust_Func();    //����������ٶȽ���
//         for( i=0; i<NPOINTS/2; i++)
//         {
//             //printf("X[%d]=%d\n",i,Xk[i]);
//             printf("%d\n",Xk[i]);
//         }
        LCD_UpdateFre_THD(fftVal.SignalFreq, fftVal.SignalTHD); 
        fftVal.adcMoreMax = 0;
        fftVal.adcMostMax = 0;
        //delayms(100);
        //delayms(500);
    }
}

void SysTick_Delay100ms()
{
    SysTick->LOAD = FAC_MS*100-1;
    SysTick->VAL = 0;
    SysTick->CTRL = (BIT0|BIT1|BIT2);       // �򿪼�������ʼ���������жϣ�ѡ��CPUʱ��
}

/**
  * @brief  Retargets the C library printf function to the UART.
  * @param  None
  * @retval None
  */
int fputc(int ch,FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the UART */
    LPC_UART0->THR = ch; /*����һ���ַ�����*/
    while((LPC_UART0->LSR&0x40) ==0 ); /*�ȴ��������*/
    return ch;
}

/*******************************************************************************
* Function Name : assert_failed
* Description : Reports the name of the source file and the source line number
* where the assert_param error has occurred.
* Input : - file: pointer to the source file name
*         - line: assert_param error line source number
* Output : None
* Return : None */
#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
    while(1)
    {
        printf("Wrong parameters value: file %s on line %d\r\n", file, line);
    }
}
#endif /* USE_FULL_ASSERT */
/*******************************************************************************/


/************************************************************
  Function   : Hardware_Startup()
  Description: Ӳ��������ʼ������
  Input      : None
  Output     : None
  Return     : ����Ӳ������,�������Ź���
  Others     : 
************************************************************/
void Hardware_Startup(void )
{
    //��Դ����
    Peripherals_PowerControl(PCADC, ENABLE);
    Peripherals_PowerControl(PCGPDMA, ENABLE);
    Peripherals_PowerControl(PCTIM2, ENABLE);
    Peripherals_PowerControl(PCTIM0, ENABLE);
    Peripherals_PowerControl(PCUART0, ENABLE);
    //����ʱ�ӷ�Ƶ
    PeripheralsClock_Divider(PCLK_TIMER0, CCLK_DIV1);   //Timer0--100MHZ
    TIM_SetClockPRDiv(LPC_TIM0, 1-1);                   //100MHZ/1 = 100MHZ
    PeripheralsClock_Divider(PCLK_TIMER2, CCLK_DIV1);   //Timer2--100MHZ
    TIM_SetClockPRDiv(LPC_TIM2, 1-1);                   //
    PeripheralsClock_Divider(PCLK_UART0, CCLK_DIV4);    //25MHZ
    
    //�������Ź�������
    GPIO0_PinFuncSel(GPIO_Pin_24, GPIOFunc_AF1);        //AD0.1����P0.24
    //GPIO0_PinFuncSel(GPIO_Pin_26, GPIOFunc_AF2);        //AOUT--P0.26
    GPIO0_PinFuncSel(GPIO_Pin_2,GPIOFunc_AF1);          //TXD0--P0.2
    GPIO0_PinFuncSel(GPIO_Pin_3,GPIOFunc_AF1);          //RXD0--P0.3
    
    GPIO3_PinFuncSel(GPIO_Pin_26, GPIOFunc_AF2);        //MAT0.1--P3.26
    GPIO0_PinFuncSel(GPIO_Pin_4, GPIOFunc_AF3);         //CAP2.0--P0.4
    LPC_PINCON->PINMODE0 |= BIT8|BIT9;                  //P0.4��������
    //GPIO0_PinFuncSel(GPIO_Pin_6, GPIOFunc_AF3);         //MAT2.0--P0.6

    //GPIO2_PinFuncSel(GPIO_Pin_10, GPIOFunc_PORT);     //IO -- P2.10
    //���ŷ�������
    GPIO_DirectionConfig(LPC_GPIO2,GPIO_Pin_LED1,GPIO_Output);
    //GPIO_DirectionConfig(LPC_GPIO2,GPIO_Pin_10,GPIO_Input); //P2.10 -- InputPin
    //GPIO_DirectionConfig(LPC_GPIO0,GPIO_Pin_4,GPIO_Input);    //P0.4 -- InputPin
    //GPIO_INTConfig(LPC_GPIO0,GPIO_Pin_4,GPIOIntEdge_Falling,ENABLE); //P0.4�½����ж�
}


/************************************************* 
    End of project.c     
*************************************************/
