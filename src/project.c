/************************************************************ 
    Copyright (C), 2013, Hnist FS_GCZX. 
  	FileName: project.c 
  	Author:Hnist FS_GCZX   Version: 13.11   Date: 2013.11.13
  	Description: LPC1768����Դ����ģ�壬Դ�����Ӧͷ�ļ�Ӧ���н���
                 �Դ�ģ��Ϊ��˵����̸�ʽ������μ���̹淶 		
  	Function List: 
    	1. main(): ʵ��LED����˸
  	History:         
      	<author>  <time>   <version >   <desc> 
        huyong      2014-04-13
***********************************************************/

// -----Includes----- //
#include "../inc/LPC1768GCZX.h" // �Զ���ͷ�ļ����μ���ͷ�ļ�ע�� //
#include "../inc/lpc_PGA2310.h" 

// -----Macros------ //
#define      LED_ON         LPC_GPIO2->FIOCLR = GPIO_Pin_0 /*�͵�ƽ����*/
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
    //PeripheralsClock_Divider(PCLK_TIMER0, CCLK_DIV1); //ʱ�ӷ�ƵҪ��PLL0����ǰ���
    SystemInit(); // ������system-LPC17xx.c�У���Ҫ���ϵͳʱ�ӳ�ʼ���ȹ��� //

/********************PeripInit**************************/
    Hardware_Startup();
    PeripInit_UART0();
    PGA2310_PortInit();
//    printf("UART0 init OK!\n");
//     PeripInit_TIM();
//     PeripInit_ADC();
//     PeripInit_DMAChan2();
//    PeripInit_DAC();
//    DMA_ConfigCmd(ENABLE);//ʹ��DMAģ��
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
    SysTick->CTRL = (BIT0|BIT1|BIT2);       // �򿪼�������ʼ���������жϣ�ѡ��CPUʱ��
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
    Peripherals_PowerControl(PCTIM0, ENABLE);
    Peripherals_PowerControl(PCTIM1, ENABLE);
    Peripherals_PowerControl(PCUART0, ENABLE);
    //����ʱ�ӷ�Ƶ
    PeripheralsClock_Divider(PCLK_TIMER0, CCLK_DIV1);   //100MHZ
    PeripheralsClock_Divider(PCLK_TIMER1, CCLK_DIV4);   //25MHZ
    PeripheralsClock_Divider(PCLK_UART0, CCLK_DIV4);    //25MHZ
    //�������Ź�������
    GPIO0_PinFuncSel(GPIO_Pin_23, GPIOFunc_AF1);        //AD0.0--P0.23
    GPIO0_PinFuncSel(GPIO_Pin_24, GPIOFunc_AF1);        //AD0.1--P0.24
    GPIO0_PinFuncSel(GPIO_Pin_26, GPIOFunc_AF2);        //AOUT
    GPIO1_PinFuncSel(GPIO_Pin_29, GPIOFunc_AF3);        //MAT0.1
    GPIO0_PinFuncSel(GPIO_Pin_2,GPIOFunc_AF1);          //TXD0
    GPIO0_PinFuncSel(GPIO_Pin_3,GPIOFunc_AF1);          //RXD0
    GPIO1_PinFuncSel(GPIO_Pin_26, GPIOFunc_AF3);        //CAP0.0
    //���ŷ�������
    GPIO_DirectionConfig(LPC_GPIO2,GPIO_Pin_LED1,GPIO_Output);
}

/************************************************* 
    End of project.c     
*************************************************/
