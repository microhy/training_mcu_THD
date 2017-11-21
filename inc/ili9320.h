

/************************************************************
    Copyright (C), 2013, Hnist FS_GCZX.
    
    File name: .h
    Author:         Version:        Date:
    Description: 
    Others: 
    Function List:
    History: 
    1.Date:
      Author:
      Modification:
    2. ...
*************************************************************/
#ifndef __ILI9320_H
#define __ILI9320_H

#ifdef __cplusplus
    extern "C" {
#endif 
/**********Includes*********************/
      
#include "../system_h/stdint.h"
#include "../inc/lpc17xx_gpio.h"
/**********Macros***********************/
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;
        
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;
        
/**********Function Prototypes**********/
void LCD_Config(void );
void LCD_UpdateFre_THD(float fre, float thd);
void LCD_Init(void );
void LCD_SetColors(__IO uint16_t _TextColor, __IO uint16_t _BackColor);
void LCD_SetCursor(uint16_t Xpos, uint8_t Ypos);
void LCD_SetPoint(uint16_t x,uint16_t y,uint16_t point);
void LCD_Clear(uint16_t Color);
void LCD_ShowChar(uint16_t x,uint8_t y,uint8_t Char);
void LCD_DisplayNum(uint8_t Line,uint16_t Column, uint32_t num);
void LCD_Display_FloatNum(uint8_t Line,uint16_t Column, double num);

/**********Variables********************/
//PORT0
#define Pin_TFT_CS     GPIO_Pin_0
#define Pin_TFT_RS     GPIO_Pin_1
#define Pin_TFT_RW     GPIO_Pin_10
#define Pin_TFT_RD     GPIO_Pin_11
//PORT2
#define Pin_TFT_RST    GPIO_Pin_13
//PORT1
#define GPIOTFT_DIR       LPC_GPIO1->FIODIRH
#define GPIOTFT_DATA      LPC_GPIO1->FIOPINH

//PORT0
#define CS_CLR         LPC_GPIO0->FIOCLR = Pin_TFT_CS      //CSÖÃµÍ
#define CS_SET         LPC_GPIO0->FIOSET = Pin_TFT_CS      //CSÖÃ¸ß

#define RS_CLR         LPC_GPIO0->FIOCLR = Pin_TFT_RS     //RSÖÃµÍ
#define RS_SET         LPC_GPIO0->FIOSET = Pin_TFT_RS     //RSÖÃ¸ß
                        
#define RW_CLR         LPC_GPIO0->FIOCLR = Pin_TFT_RW     //RWÖÃµÍ
#define RW_SET         LPC_GPIO0->FIOSET = Pin_TFT_RW     //RWÖÃ¸ß
                        
#define RD_CLR         LPC_GPIO0->FIOCLR = Pin_TFT_RD    
#define RD_SET         LPC_GPIO0->FIOSET = Pin_TFT_RD     
//PORT2
#define RST_CLR        LPC_GPIO2->FIOCLR = Pin_TFT_RST     //RSTÖÃµÍ
#define RST_SET        LPC_GPIO2->FIOSET = Pin_TFT_RST     //RSTÖÃ¸ß

#define HyalineBackColor 0x0001  // Lihao Í¸Ã÷±³¾°
#define Line0          0
#define Line1          24
#define Line2          48
#define Line3          72
#define Line4          96
#define Line5          120
#define Line6          144
#define Line7          168
#define Line8          192
#define Line9          216



/* LCD color */
#define White          0xFFFF //°×É«
#define Black          0x0000 //ºÚÉ«
#define Grey           0xF7DE //»ÒÉ«
#define Blue           0x001F //À¶É«
#define Blue2          0x051F //ÉîÀ¶É«
#define Red            0xF800 //ºìÉ«
#define Magenta        0xF81F //Æ·ºì
#define Green          0x07E0 //ÂÌÉ«
#define Cyan           0x7FFF //À¶ÂÌÉ«
#define Yellow         0xFFE0 //»ÆÉ«

#define Horizontal     0x00
#define Vertical       0x01
#define LCD_REG_0             0x00
#define LCD_REG_1             0x01
#define LCD_REG_2             0x02
#define LCD_REG_3             0x03
#define LCD_REG_4             0x04
#define LCD_REG_5             0x05
#define LCD_REG_6             0x06
#define LCD_REG_7             0x07
#define LCD_REG_8             0x08
#define LCD_REG_9             0x09
#define LCD_REG_10            0x0A
#define LCD_REG_12            0x0C
#define LCD_REG_13            0x0D
#define LCD_REG_14            0x0E
#define LCD_REG_15            0x0F
#define LCD_REG_16            0x10
#define LCD_REG_17            0x11
#define LCD_REG_18            0x12
#define LCD_REG_19            0x13
#define LCD_REG_20            0x14
#define LCD_REG_21            0x15
#define LCD_REG_22            0x16
#define LCD_REG_23            0x17
#define LCD_REG_24            0x18
#define LCD_REG_25            0x19
#define LCD_REG_26            0x1A
#define LCD_REG_27            0x1B
#define LCD_REG_28            0x1C
#define LCD_REG_29            0x1D
#define LCD_REG_30            0x1E
#define LCD_REG_31            0x1F
#define LCD_REG_32            0x20
#define LCD_REG_33            0x21
#define LCD_REG_34            0x22
#define LCD_REG_36            0x24
#define LCD_REG_37            0x25
#define LCD_REG_40            0x28
#define LCD_REG_41            0x29
#define LCD_REG_43            0x2B
#define LCD_REG_45            0x2D
#define LCD_REG_48            0x30
#define LCD_REG_49            0x31
#define LCD_REG_50            0x32
#define LCD_REG_51            0x33
#define LCD_REG_52            0x34
#define LCD_REG_53            0x35
#define LCD_REG_54            0x36
#define LCD_REG_55            0x37
#define LCD_REG_56            0x38
#define LCD_REG_57            0x39
#define LCD_REG_58            0x3A
#define LCD_REG_59            0x3B
#define LCD_REG_60            0x3C
#define LCD_REG_61            0x3D
#define LCD_REG_62            0x3E
#define LCD_REG_63            0x3F
#define LCD_REG_64            0x40
#define LCD_REG_65            0x41
#define LCD_REG_66            0x42
#define LCD_REG_67            0x43
#define LCD_REG_68            0x44
#define LCD_REG_69            0x45
#define LCD_REG_70            0x46
#define LCD_REG_71            0x47
#define LCD_REG_72            0x48
#define LCD_REG_73            0x49
#define LCD_REG_74            0x4A
#define LCD_REG_75            0x4B
#define LCD_REG_76            0x4C
#define LCD_REG_77            0x4D
#define LCD_REG_78            0x4E
#define LCD_REG_79            0x4F
#define LCD_REG_80            0x50
#define LCD_REG_81            0x51
#define LCD_REG_82            0x52
#define LCD_REG_83            0x53
#define LCD_REG_96            0x60
#define LCD_REG_97            0x61
#define LCD_REG_106           0x6A
#define LCD_REG_118           0x76
#define LCD_REG_128           0x80
#define LCD_REG_129           0x81
#define LCD_REG_130           0x82
#define LCD_REG_131           0x83
#define LCD_REG_132           0x84
#define LCD_REG_133           0x85
#define LCD_REG_134           0x86
#define LCD_REG_135           0x87
#define LCD_REG_136           0x88
#define LCD_REG_137           0x89
#define LCD_REG_139           0x8B
#define LCD_REG_140           0x8C
#define LCD_REG_141           0x8D
#define LCD_REG_143           0x8F
#define LCD_REG_144           0x90
#define LCD_REG_145           0x91
#define LCD_REG_146           0x92
#define LCD_REG_147           0x93
#define LCD_REG_148           0x94
#define LCD_REG_149           0x95
#define LCD_REG_150           0x96
#define LCD_REG_151           0x97
#define LCD_REG_152           0x98
#define LCD_REG_153           0x99
#define LCD_REG_154           0x9A
#define LCD_REG_157           0x9D
#define LCD_REG_192           0xC0
#define LCD_REG_193           0xC1
#define LCD_REG_229           0xE5


#ifdef __cplusplus
}
#endif

#endif
/************************************************************
End of filename
************************************************************/

