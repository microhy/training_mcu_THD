#include "../inc/ili9320.h"
#include "../inc/SysTick.h"
#include "../inc/font.h"
#include "../inc/picture.h"
#include "../inc/GB2424.h"                       //24x24像素大小的汉字
#include "../inc/ili9320_api.h"

// LCD default font 
static sFONT *LCD_Currentfonts=&Font16x08;
/* Global variables to set the written text color */
static  __IO uint16_t TextColor = 0x0000, BackColor = 0xFFFF;

#define Line(x)  ((x) * (((sFONT *)LCD_GetFont())->Height))
#define Column(x)((x) * (((sFONT *)LCD_GetFont())->Width))
extern uint32_t Xk[NPOINTS/2];
extern uint8_t TP;

void LCD_UpdateFre_THD(float fre, float thd)
{
    uint32_t i,dx;
    float percent;
    //LCD_Init();
    LCD_Clear(White);
    dx = 2*300/NPOINTS;
    //LCD_SetPoint(15+2*0,219,Red);
    for(i=1; i<NPOINTS/2; ++i) //注意这里的点数NPOINTS
    {
        percent = (float)Xk[i]*219*2/Xk[TP]; //最高显示幅度219
        percent = (uint16_t)percent;          //只显示谐波分量，其它频率成分不显示
        if(percent >219)
        {
            percent = 219;
        }
        //LCD_SetPoint(15+2*i,219-percent,Red);
        GUI_Line(15+dx*i,219,15+dx*i,219-percent,Red);
        GUI_Line(16+dx*i,219,16+dx*i,219-percent,Red);
        //GUI_Line(17+dx*i,219,17+dx*i,219-percent,Red);
    }
    //画纵轴
    GUI_Line(10,0,10,239,Cyan);
    GUI_Line(11,0,11,239,Cyan);
    GUI_Line(12,0,12,239,Cyan);
    //画横轴
    GUI_Line(0,220,319,220,Cyan);
    GUI_Line(0,221,319,221,Cyan);
    GUI_Line(0,222,319,222,Cyan);
    LCD_SetColors(Magenta, White);
    LCD_ShowChar(20,223,'F');
    LCD_ShowChar(20+8,223,'r');
    LCD_ShowChar(20+16,223,'e');
    LCD_ShowChar(20+24,223,'q');
    LCD_ShowChar(20+32,223,':');
    
    LCD_Display_FloatNum(223, 20+40, fftVal.SignalFreq);  //fre
    LCD_ShowChar(20+120,223,'H');
    LCD_ShowChar(20+128,223,'z');
    
    LCD_SetColors(Blue, White);
    LCD_ShowChar(170,223,'T');
    LCD_ShowChar(170+8,223,'H');
    LCD_ShowChar(170+16,223,'D');
    LCD_ShowChar(170+24,223,':');
    LCD_Display_FloatNum(223, 170+32, fftVal.SignalTHD); //thd
    LCD_ShowChar(170+100,223,'%');
}

void LCD_Config(void )
{
    //TFT控制线IO设置
    LPC_GPIO0->FIODIR |= (Pin_TFT_RS|Pin_TFT_RW|Pin_TFT_RD|Pin_TFT_CS);
    LPC_GPIO0->FIOSET = (Pin_TFT_RS|Pin_TFT_RW|Pin_TFT_RD|Pin_TFT_CS);
    LPC_GPIO2->FIODIR |= Pin_TFT_RST;  //P2.13-RST output
    LPC_GPIO2->FIOSET = Pin_TFT_RST;
    //TFT数据线IO设置
    LPC_GPIO1->FIODIRH = 0xFFFF;
    LPC_GPIO1->FIOPINH = 0xFFFF;
    
    LCD_Init();
    LCD_Clear(White);
    GUI_Line(10,0,10,239,Cyan);
    GUI_Line(11,0,11,239,Cyan);
    GUI_Line(12,0,12,239,Cyan);
    
    GUI_Line(0,220,319,220,Cyan);
    GUI_Line(0,221,319,221,Cyan);
    GUI_Line(0,222,319,222,Cyan);
    LCD_SetColors(Magenta, White);
    LCD_ShowChar(20,223,'F');
    LCD_ShowChar(20+8,223,'r');
    LCD_ShowChar(20+16,223,'e');
    LCD_ShowChar(20+24,223,'q');
    LCD_ShowChar(20+32,223,':');
    
    LCD_Display_FloatNum(223, 20+40, 0);  //fre
    LCD_ShowChar(20+120,223,'H');
    LCD_ShowChar(20+128,223,'z');
    
    LCD_SetColors(Blue, White);
    LCD_ShowChar(170,223,'T');
    LCD_ShowChar(170+8,223,'H');
    LCD_ShowChar(170+16,223,'D');
    LCD_ShowChar(170+24,223,':');
    LCD_Display_FloatNum(223, 170+32, 0); //thd
    LCD_ShowChar(170+100,223,'%');
}

//LCD延时函数 10MS
void Delay (uint32_t nCount)
{
    volatile int i;
    for (i=0;i<nCount*300;i++);
}
static __inline void LCD_WR_REG(unsigned short int reg)
{
    CS_CLR;
    RS_CLR; //RS=0写地址
    RW_CLR;
    GPIOTFT_DATA = reg;
    RW_SET;
    CS_SET;
}
static __inline void LCD_WR_DATA(unsigned short int dat)
{
    CS_CLR;
    RS_SET; //RS=1写数据
    __NOP();
    RW_CLR;
    GPIOTFT_DATA = dat;
    RW_SET;
    CS_SET;
}

static __inline void LCD_WriteReg(unsigned short int reg, unsigned short int dat)
{
    LCD_WR_REG(reg);
    LCD_WR_DATA(dat);
}
static __inline unsigned short int LCD_ReadReg(unsigned short int cmd)
{
    unsigned short int rdat;
    LCD_WR_REG(cmd);
    GPIOTFT_DIR = 0X0000;
    RS_SET;
    CS_CLR;
    RD_CLR;
    delayus(1);
    rdat = GPIOTFT_DATA;
    RD_SET;
    CS_SET;
    GPIOTFT_DIR = 0XFFFF;
    //GPIOTFT_DATA = 0XFFFF;
    return(rdat);
}
static __inline void LCD_WriteRAM(unsigned short int dat)
{
    CS_CLR;
    RS_SET; //RS=1写数据
    __NOP();
    RW_CLR;
    GPIOTFT_DATA = dat;
    RW_SET;
    CS_SET;
}

static __inline void LCD_WriteRAM_Prepare(void)
{
    CS_CLR;
    RS_CLR; //RS=0写地址
    RW_CLR;
    GPIOTFT_DATA = LCD_REG_34;
    RW_SET;
    CS_SET;
}
/**
  * @brief  设置文本色和背景色.
  * @param  _TextColor: specifies the Text Color.
  * @param  _BackColor: specifies the Background Color.
  * @retval None
  */
void LCD_SetColors(__IO uint16_t _TextColor, __IO uint16_t _BackColor)
{
  TextColor = _TextColor; 
  BackColor = _BackColor;
}
/**
  * @brief  设置字体.
  * @param  fonts: specifies the font to be used.
  * @retval None
  */
void LCD_SetFont(sFONT *fonts,uint8_t Mode)
{
  LCD_Currentfonts = fonts;
  LCD_Currentfonts->Mode=Mode;
}
/**
  * @brief  获取当前字体.
  * @param  None.
  * @retval the used font.
  */
sFONT *LCD_GetFont(void)
{
  return LCD_Currentfonts;
}
/**
  * @brief  Sets the cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position. 
  * @retval None  
  */
 void LCD_SetCursor(uint16_t Xpos, uint8_t Ypos)
{
  /*  px，py 是物理坐标， Xpos，Ypos是虚拟坐标
   *  转换公式: px=Ypos py=319-Xpos		  	
   */
  LCD_WriteReg(0x4e,Ypos);
  LCD_WriteReg(0x4f,319-Xpos);
}
/**
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void LCD_DisplayOn(void)
{
  /* Display On */
  LCD_WriteReg(LCD_REG_7, 0x0173); /* 262K color and display ON */
}
/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void LCD_DisplayOff(void)
{
  /* Display Off */
  LCD_WriteReg(LCD_REG_7, 0x0); 
}
void LCD_Clear(uint16_t Color)
{
    unsigned int index;
    LCD_SetCursor(0,0);
    LCD_WR_REG(LCD_REG_34);
    for(index=0;index<76800;index++)
    {
        LCD_WR_DATA(Color);
    }
}
/****************************************************************************
* 名    称：void ili9320_SetPoint(u16 x,u16 y,u16 point)
* 功    能：在指定座标画点
* 入口参数：x      行座标
*           y      列座标
*           point  点的颜色
* 出口参数：无
* 说    明：
* 调用方法：ili9320_SetPoint(10,10,0x0fe0);
****************************************************************************/
void LCD_SetPoint(uint16_t x,uint16_t y,uint16_t point)
{
    if ( (x>320)||(y>240) ) 
        return;
    LCD_SetCursor(x,y);
    LCD_WR_REG(LCD_REG_34);
    LCD_WR_DATA(point);
}

/****************************************************************************
* 名    称：void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
* 功    能：设置窗口区域
* 入口参数：StartX     行起始座标
*           StartY     列起始座标
*           EndX       行结束座标
*           EndY       列结束座标
* 出口参数：无
* 说    明：
* 调用方法：ili9320_SetWindows(0,0,100,100)；
****************************************************************************/
//inline void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
void LCD_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
{
  LCD_SetCursor(StartX,StartY);
  LCD_WriteReg(0x0050, StartX);
  LCD_WriteReg(0x0052, StartY);
  LCD_WriteReg(0x0051, EndX);
  LCD_WriteReg(0x0053, EndY);
}
/**
  * @brief  指定位置写一个字符
  * @param  X： X坐标;
  *			Y： Y坐标；
  *		 char： 指向字符ascii代码的指针
  * @retval None.
  */
void LCD_ShowChar(uint16_t x,uint8_t y,uint8_t Char)
{   
    uint8_t temp,dy,dx;//位置偏量		   
	Char=Char-32;//得到偏移后的值
	for(dy=0;dy<LCD_Currentfonts->Height;dy++)
	{
		temp=LCD_Currentfonts->table[Char][dy];//调用字体
		for(dx=0;dx<LCD_Currentfonts->Height/2;dx++)
	    {                 
	        if(temp&0x01)
			{ 
			   LCD_SetCursor(x+dx,y+dy);//设置光标位置 
			   LCD_WriteRAM_Prepare(); 
			   LCD_WR_DATA(TextColor);
			}
			else
			if(LCD_Currentfonts->Mode)
			{
			   LCD_SetCursor(x+dx,y+dy);//设置光标位置 
			   LCD_WriteRAM_Prepare(); 
			   LCD_WR_DATA(BackColor);
			}     
	        temp>>=1; 
	    }
	} 
}
/**
  * @brief  显示{数字数值(0~4294967295)}.
  * @param  Line,Column：行与列位置.
  * @param  num:数值(0~4294967295).
  * @retval None
  */
void LCD_DisplayNum(uint8_t Line,uint16_t Column, uint32_t num)
{ 
    uint32_t temp=num;
    uint16_t refcolumn =Column;
    uint8_t  refLine =Line,len=0; 
    //计算数值长度
    do{
        temp = temp/10;
        len++;
    }while(temp); 
    len--;
/********************************/
    refcolumn +=len*LCD_Currentfonts->Width;//从后往前显示
    //显示数字
    do{ 
      temp=num%10;
      LCD_ShowChar(refcolumn,refLine,temp+'0'); 
      refcolumn-=LCD_Currentfonts->Width;
      num=num/10;
    }while(len--);
}
void LCD_Display_FloatNum(uint8_t Line,uint16_t Column, double num)
{
	 uint64_t Temp;
	 uint32_t Temp1;
	 uint8_t len=0;
	 Temp=(uint64_t)(num*100000);//化为整数
	 //整数部分显示
	 Temp1=(uint32_t)(Temp/100000);
	 //显示整数部分
	 LCD_DisplayNum(Line,Column+Column(len),Temp1);
	 //计算整数部分长度
	do{
		Temp1 = Temp1/10;
		len++;
	}while(Temp1);
	//小数部分
	Temp1=(uint32_t)(Temp%100000);
	//去掉末尾多余0
	while(Temp1%10==0&&Temp1!=0)
	{
		Temp1=Temp1/10;
	}
	//显示小数点
	if(Temp1!=0)//判断是否是小数，是则显示
	{
		LCD_ShowChar(Column+Column(len),Line,'.');
		len++;
	    LCD_DisplayNum(Line,Column+Column(len),Temp1);
	}
}

void LCD_Init(void )
{
    uint16_t DeviceCode;
    RST_SET;
    delayms(1);                    // Delay 1ms 
    RST_CLR;  
    delayms(10);                   // Delay 10ms            
    RST_SET;  
    delayms(50);                   // Delay 50 ms
    //LCD_WriteReg(0x0000,0x0001);Delay(5);//打开晶振
    DeviceCode = LCD_ReadReg(0x0000);
    delayms(50);
    DeviceCode = LCD_ReadReg(0x0000);
    LCD_WriteReg(0x0000,0x0001);		//打开晶振
    delayms(50);                   // Delay 50 ms
    LCD_WriteReg(0x0003,0xA8A4);Delay(5);//0xA8A4
    LCD_WriteReg(0x000C,0x0000);Delay(5);    
    LCD_WriteReg(0x000D,0x080C);Delay(5);    
    LCD_WriteReg(0x000E,0x2B00);Delay(5);    
    LCD_WriteReg(0x001E,0x00B0);Delay(5);    
    LCD_WriteReg(0x0001,0x2B3F);Delay(5);//驱动输出控制320*240  0x6B3F
    LCD_WriteReg(0x0002,0x0600);Delay(5); 
    LCD_WriteReg(0x0010,0x0000);Delay(5); 
    LCD_WriteReg(0x0011,0x6070);Delay(5);//定义数据格式  16位色 		横屏 0x6058
    LCD_WriteReg(0x0005,0x0000);Delay(5); 
    LCD_WriteReg(0x0006,0x0000);Delay(5); 
    LCD_WriteReg(0x0016,0xEF1C);Delay(5); 
    LCD_WriteReg(0x0017,0x0003);Delay(5); 
    LCD_WriteReg(0x0007,0x0233);Delay(5);//0x0233       
    LCD_WriteReg(0x000B,0x0000);Delay(5); 
    LCD_WriteReg(0x000F,0x0000);Delay(5);//扫描开始地址
    LCD_WriteReg(0x0041,0x0000);Delay(5); 
    LCD_WriteReg(0x0042,0x0000);Delay(5); 
    LCD_WriteReg(0x0048,0x0000);Delay(5); 
    LCD_WriteReg(0x0049,0x013F);Delay(5); 
    LCD_WriteReg(0x004A,0x0000);Delay(5); 
    LCD_WriteReg(0x004B,0x0000);Delay(5); 
    LCD_WriteReg(0x0044,0xEF00);Delay(5); 
    LCD_WriteReg(0x0045,0x0000);Delay(5); 
    LCD_WriteReg(0x0046,0x013F);Delay(5); 
    LCD_WriteReg(0x0030,0x0707);Delay(5); 
    LCD_WriteReg(0x0031,0x0204);Delay(5); 
    LCD_WriteReg(0x0032,0x0204);Delay(5); 
    LCD_WriteReg(0x0033,0x0502);Delay(5); 
    LCD_WriteReg(0x0034,0x0507);Delay(5); 
    LCD_WriteReg(0x0035,0x0204);Delay(5); 
    LCD_WriteReg(0x0036,0x0204);Delay(5); 
    LCD_WriteReg(0x0037,0x0502);Delay(5); 
    LCD_WriteReg(0x003A,0x0302);Delay(5); 
    LCD_WriteReg(0x003B,0x0302);Delay(5); 
    LCD_WriteReg(0x0023,0x0000);Delay(5); 
    LCD_WriteReg(0x0024,0x0000);Delay(5); 
    LCD_WriteReg(0x0025,0x8000);Delay(5); 
    LCD_WriteReg(0x004f,0);        //行首址0
    LCD_WriteReg(0x004e,0);        //列首址0
    //LCD_Clear(Blue);
    delayms(50);
    LCD_WriteReg(LCD_REG_7, 0x0173); //26万色显示开启
}


