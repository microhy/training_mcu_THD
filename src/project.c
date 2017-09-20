/************************************************************ 
    Copyright (C), 2013, Hnist FS_GCZX. 
  	FileName: project.c 
  	Author:Hnist FS_GCZX   Version: 13.11   Date: 2013.11.13
  	Description: LPC1768工程源程序模板，源程序对应头文件应自行建立
                 以此模板为例说明编程格式，详情参见编程规范 		
  	Function List: 
    	1. main(): 实现LED的闪烁
  	History:         
      	<author>  <time>   <version >   <desc> 
        huyong      2014-04-13
***********************************************************/

// -----Includes----- //
#include "../inc/LPC1768GCZX.h" // 自定义头文件，参见该头文件注释 //
#include "../inc/lpc_PGA2310.h" 

// -----Macros------ //
#define      LED_ON         LPC_GPIO2->FIOCLR = GPIO_Pin_0 /*低电平点亮*/
#define      LED_OFF        LPC_GPIO2->FIOSET = GPIO_Pin_0

#define      TP      2
// -----Function Prototypes----- //
void SysTick_Delay100ms(void );
void Hardware_Startup(void );
// -----Variables----- //
uint32_t Xk[NPOINTS/2] = {0};
uint32_t i = 0;

int main(void)
{
    
    float derat = 0;
    //PeripheralsClock_Divider(PCLK_TIMER0, CCLK_DIV1); //时钟分频要在PLL0启动前完成
    SystemInit(); // 定义于system-LPC17xx.c中，主要完成系统时钟初始化等功能 //

/********************PeripInit**************************/
    Hardware_Startup();
    PeripInit_UART0();
    PGA2310_PortInit();
//    printf("UART0 init OK!\n");
//     PeripInit_TIM();
//     PeripInit_ADC();
//     PeripInit_DMAChan2();
//    PeripInit_DAC();
//    DMA_ConfigCmd(ENABLE);//使能DMA模块
    PeripInit_TIM_GainAdjust();
    PeripInit_ADC_GainAdjust();
/******************NVIC INTERRUPT ENABLE****************/
//    NVIC_EnableIRQ(TIMER0_IRQn);
    NVIC_EnableIRQ(TIMER1_IRQn);
//     NVIC_EnableIRQ(RIT_IRQn);
    NVIC_EnableIRQ(ADC_IRQn);

/*********************************************************/
    PGA2310_GainConfig(192, 192);
    GainAdjust_Func();
    while(1)
    {
//         PeripInit_TIM();
//         PeripInit_ADC();
//         PeripInit_DMAChan2();
        
    }
}

void SysTick_Delay100ms()
{
    SysTick->LOAD = FAC_MS*100-1;
    SysTick->VAL = 0;
    SysTick->CTRL = (BIT0|BIT1|BIT2);       // 打开计数器开始倒数，开中断，选择CPU时钟
}


void TIMER1_IRQHandler(void )
{
    //1.clear intflagbit
    //2.start timer cnt
    //3.set cntflag
    LPC_TIM1->IR |= BIT0;
    GainAdjVal.flag_Timer10ms = SET;
//     ++i;
//     if(i%2) //!!!
//     {
//         LED_ON;
//     }
//     else
//     {
//         LED_OFF;
//     }
}

void ADC_IRQHandler(void )
{
    uint32_t regVal;
    regVal = LPC_ADC->ADSTAT;   /* Read ADC will clear the interrupt */
    if ( regVal & 0x0000FF00 )  /* check OVERRUN error first */
    {
        regVal = (regVal & 0x0000FF00) >> 0x08;
            /* if overrun, just read ADDR to clear */
            /* regVal variable has been reused. */
        regVal = LPC_ADC->ADDR0;
        LPC_ADC->ADCR &= 0xF8FFFFFF;    /* stop ADC now */
        LPC_ADC->ADCR |= ADC_StartSignal_Start;  //start
        return; //error return
    }
    if ( regVal & 0x00010000 ) //check interrupt
    {
        GainAdjVal.ADC0Value0 = ( LPC_ADC->ADDR0 >> 4 ) & 0xFFF;
        if(GainAdjVal.ADC0Value0 > GainAdjVal.ADC0ValMax)
        {
            GainAdjVal.ADC0ValMax = GainAdjVal.ADC0Value0;
        }
    }
    LPC_ADC->ADCR |= ADC_StartSignal_Start;  //start
    return;
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
    Peripherals_PowerControl(PCTIM0, ENABLE);
    Peripherals_PowerControl(PCTIM1, ENABLE);
    Peripherals_PowerControl(PCUART0, ENABLE);
    //外设时钟分频
    PeripheralsClock_Divider(PCLK_TIMER0, CCLK_DIV1);   //100MHZ
    PeripheralsClock_Divider(PCLK_TIMER1, CCLK_DIV4);   //25MHZ
    PeripheralsClock_Divider(PCLK_UART0, CCLK_DIV4);    //25MHZ
    //外设引脚功能配置
    GPIO0_PinFuncSel(GPIO_Pin_23, GPIOFunc_AF1);        //AD0.0--P0.23
    GPIO0_PinFuncSel(GPIO_Pin_24, GPIOFunc_AF1);        //AD0.1--P0.24
    GPIO0_PinFuncSel(GPIO_Pin_26, GPIOFunc_AF2);        //AOUT
    GPIO1_PinFuncSel(GPIO_Pin_29, GPIOFunc_AF3);        //MAT0.1
    GPIO0_PinFuncSel(GPIO_Pin_2,GPIOFunc_AF1);          //TXD0
    GPIO0_PinFuncSel(GPIO_Pin_3,GPIOFunc_AF1);          //RXD0
    GPIO1_PinFuncSel(GPIO_Pin_26, GPIOFunc_AF3);        //CAP0.0
    //引脚方向设置
    GPIO_DirectionConfig(LPC_GPIO2,GPIO_Pin_LED1,GPIO_Output);
}

/************************************************* 
    End of project.c     
*************************************************/
