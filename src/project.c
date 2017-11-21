/************************************************************ 
        Copyright (C), 2013, Hnist FS_GCZX. 
    FileName: project.c 
    Author:Hnist FS_GCZX   Version: 13.11   Date: 2013.11.13
    Description: LPC1768工程源程序模板，源程序对应头文件应自行建立
                 以此模板为例说明编程格式，详情参见编程规范
    Function List: 

    History:
    <author>  huyong 
    <time>    2014_06_20
    <version > V2014.06.20
    <desc> 200Hz~5KHz 正弦波，方波，三角波失真度分析及LCD频谱显示
    200Hz <=f <1250Hz : Fs=128f
        if(fftVal.SignalFreq<1250)   //200hz -- 5ms 最长采样时间5ms*2 = 10ms
        {
            TP = 2;  //共采集256点，分2个周期采完。故 fs=128f; fsmax=128*1.25k = 160k<200k
        }
        else if(fftVal.SignalFreq<2500) //1.25khz --800us 最长采样时间800us*4=3.2ms
        {
            TP = 4;  //共采集256点，分4个周期采完。故 fs=64f; fsmax=64*2.5k = 160k<200k
        }
        else  //2.5khz--400us 最长采样时间400us*8=3.2ms
        {
            TP = 8;  //共采集256点，分8个周期采完。故 fs=32f; fsmax=32*5k = 160k<200k
***********************************************************/

// -----Includes----- //
#include "../inc/LPC1768GCZX.h" // 自定义头文件，参见该头文件注释 //
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
    SystemInit(); // 定义于system-LPC17xx.c中，主要完成系统时钟初始化等功能 //
/********************PeripInit**************************/
    Hardware_Startup();
    PeripInit_UART0();
    printf("UART0 init OK!\n");
    LCD_Config();
    PeripInit_ADC();
//    PeripInit_TIM0_MAT1();
//    PeripInit_DMAChan2();
    DMA_ConfigCmd(ENABLE);//使能DMA模块
    
    PGA2310_PortInit();
    PGA2310_GainConfig(192, 192);
/******************NVIC INTERRUPT ENABLE****************/
    NVIC_EnableIRQ(TIMER2_IRQn);
//     NVIC_EnableIRQ(RIT_IRQn);
//     NVIC_EnableIRQ(ADC_IRQn);
    
/*********************************************************/
    while(1)
    {   
        //最大放大用于测量频率
        PGA2310_GainConfig(255, 192);   //只放大一个通道，若放大两个通道，不输入信号比较器也会有跳变信号
        delayus((fftVal.Signalperiod/1000));
        delayus((fftVal.Signalperiod/1000));
        delayus((fftVal.Signalperiod/1000));//等待增益配置更新
        NVIC_EnableIRQ(TIMER2_IRQn);
        fftVal.CntEdge = 0;
        PeripInit_TIM2_CAP0();   //CAP2.0用于测量周期
        Measure_SignalPeriod();  //函数测量周期
        ////周期测量完毕，复位TIM2
        TIM_CaptureIntCmd(LPC_TIM2, CAPn_0, DISABLE);  //捕获中断不允许
        TIM_CountResetCmd(LPC_TIM2, ENABLE);
        TIM_ClearITPendingBit(LPC_TIM2, TIM_IT_CR0);
        NVIC_DisableIRQ(TIMER2_IRQn);
        fftVal.SignalFreq = 1000000000/fftVal.Signalperiod; //
        if(fftVal.SignalFreq<1250)   //200hz -- 5ms 最长采样时间5ms*2 = 10ms
        {
            TP = 2;  //共采集256点，分2个周期采完。故 fs=128f; fsmax=128*1.25k = 160k<200k
        }
        else if(fftVal.SignalFreq<2500) //1.25khz --800us 最长采样时间800us*4=3.2ms
        {
            TP = 4;  //共采集256点，分4个周期采完。故 fs=64f; fsmax=64*2.5k = 160k<200k
        }
        else  //2.5khz--400us 最长采样时间400us*8=3.2ms
        {
            TP = 8;  //共采集256点，分8个周期采完。故 fs=32f; fsmax=32*5k = 160k<200k
        }
        derat = TP*fftVal.Signalperiod/(10*NPOINTS); //除以10是因为是测周期时累加了10次
        printf("period = %f us\n",fftVal.Signalperiod/1000); //
        printf("fre = %f Hz\n\n",fftVal.SignalFreq); //
        //恢复原信号用于采样
        PGA2310_GainConfig(GainAdjVal.Gain_ChanL, GainAdjVal.Gain_ChanR);
        delayus((fftVal.Signalperiod/1000)); 
        delayus((fftVal.Signalperiod/1000));
        delayus((fftVal.Signalperiod/1000)); //等待增益配置更新
        PeripInit_TIM0_MAT1();
        TIM_CountResetCmd(LPC_TIM0, ENABLE);  //清除复位TIM0
        LPC_TIM0->MR1 = derat/2;               //开始触发采样
        TIM_CountResetCmd(LPC_TIM0, DISABLE);  //清除复位TIM0
        TIM_CountCmd(LPC_TIM0, ENABLE);        //启动TIM0计数
        PeripInit_DMAChan2();
        
        delayms(15);
        //delayms(15);
        //while((LPC_ADC->ADSTAT&ADC_STATUS_DONE1)==0); //等待最后一次转换结束
        //ADC_StartConvCmd(DISABLE);  //采样完毕，停止ADC转换         
        LPC_TIM0->MR1 = 0;
        TIM_MatchControlCmd(LPC_TIM0, TIM_MR1R, DISABLE);//采样完毕，失能匹配MAT0.1
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
            if(psi_Input[i*2] > fftVal.adcMostMax)//次最大值大于最大值
            {
                fftVal.adcMostMax = psi_Input[i*2];
                fftVal.adcMostMax_num = i*2;
            }
            else if(psi_Input[i*2] > fftVal.adcMoreMax)  //采样值大于次最大值
            {
                fftVal.adcMoreMax = psi_Input[i*2];
            }
        }
        GainAdjVal.ADC0Value_cur = fftVal.adcMostMax - 2048;    //1.65*4096/3.3 = 2048，减去直流量;
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
        SumUn = 0;  //清零
        for(i=2*TP; i<11*TP; i+=TP) //只分析到10次谐波分量
        {
            SumUn += Xk[i];
        }
        fftVal.SignalTHD = 100*sqrt((double)SumUn/Xk[TP]);
        printf("SumUn = %d\n",SumUn);
        printf("thd = %f\n",fftVal.SignalTHD);
        
        PGA2310_GainAdjust_Func();    //增益调整，速度较慢
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
    SysTick->CTRL = (BIT0|BIT1|BIT2);       // 打开计数器开始倒数，开中断，选择CPU时钟
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
    LPC_UART0->THR = ch; /*发送一个字符函数*/
    while((LPC_UART0->LSR&0x40) ==0 ); /*等待发送完毕*/
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
  Description: 硬件启动初始化设置
  Input      : None
  Output     : None
  Return     : 启动硬件外设,配置引脚功能
  Others     : 
************************************************************/
void Hardware_Startup(void )
{
    //电源控制
    Peripherals_PowerControl(PCADC, ENABLE);
    Peripherals_PowerControl(PCGPDMA, ENABLE);
    Peripherals_PowerControl(PCTIM2, ENABLE);
    Peripherals_PowerControl(PCTIM0, ENABLE);
    Peripherals_PowerControl(PCUART0, ENABLE);
    //外设时钟分频
    PeripheralsClock_Divider(PCLK_TIMER0, CCLK_DIV1);   //Timer0--100MHZ
    TIM_SetClockPRDiv(LPC_TIM0, 1-1);                   //100MHZ/1 = 100MHZ
    PeripheralsClock_Divider(PCLK_TIMER2, CCLK_DIV1);   //Timer2--100MHZ
    TIM_SetClockPRDiv(LPC_TIM2, 1-1);                   //
    PeripheralsClock_Divider(PCLK_UART0, CCLK_DIV4);    //25MHZ
    
    //外设引脚功能配置
    GPIO0_PinFuncSel(GPIO_Pin_24, GPIOFunc_AF1);        //AD0.1引脚P0.24
    //GPIO0_PinFuncSel(GPIO_Pin_26, GPIOFunc_AF2);        //AOUT--P0.26
    GPIO0_PinFuncSel(GPIO_Pin_2,GPIOFunc_AF1);          //TXD0--P0.2
    GPIO0_PinFuncSel(GPIO_Pin_3,GPIOFunc_AF1);          //RXD0--P0.3
    
    GPIO3_PinFuncSel(GPIO_Pin_26, GPIOFunc_AF2);        //MAT0.1--P3.26
    GPIO0_PinFuncSel(GPIO_Pin_4, GPIOFunc_AF3);         //CAP2.0--P0.4
    LPC_PINCON->PINMODE0 |= BIT8|BIT9;                  //P0.4引脚下拉
    //GPIO0_PinFuncSel(GPIO_Pin_6, GPIOFunc_AF3);         //MAT2.0--P0.6

    //GPIO2_PinFuncSel(GPIO_Pin_10, GPIOFunc_PORT);     //IO -- P2.10
    //引脚方向设置
    GPIO_DirectionConfig(LPC_GPIO2,GPIO_Pin_LED1,GPIO_Output);
    //GPIO_DirectionConfig(LPC_GPIO2,GPIO_Pin_10,GPIO_Input); //P2.10 -- InputPin
    //GPIO_DirectionConfig(LPC_GPIO0,GPIO_Pin_4,GPIO_Input);    //P0.4 -- InputPin
    //GPIO_INTConfig(LPC_GPIO0,GPIO_Pin_4,GPIOIntEdge_Falling,ENABLE); //P0.4下降沿中断
}


/************************************************* 
    End of project.c     
*************************************************/
