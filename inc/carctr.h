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
#ifndef __CARCTR_H
#define __CARCTR_H

#ifdef __cplusplus
    extern "C" {
#endif 
/**********Includes*********************/
#include "../inc/LPC1768GCZX.h"       
// #include ".h"
typedef struct
{
    uint32_t cap0Val[2];
    uint32_t cap1Val[2];
    uint32_t ChanVal[2];
    FlagStatus Flag_Cap0H;
    FlagStatus Flag_Cap1H;
    FlagStatus Flag_Dedone0;
    FlagStatus Flag_Dedone1;
}ppmDecode_DataDef;

typedef struct
{
    uint16_t SpeedLF;  //left fornt 左前马达
    uint16_t SpeedLB;  //left back  左后马达
    uint16_t SpeedRF;
    uint16_t SpeedRB;
    uint8_t  dirIN;    //方向控制 bit0~bit7 -- in1~in8
}CarCtr_DataDef;

extern volatile ppmDecode_DataDef ppmVal;
extern volatile CarCtr_DataDef carVal;
/**********Macros***********************/
#define     PWM_Frequency       1000
#define     _MEDI_VAL_          1500

#define     PPM_MEDIVAL_INIT    1500
#define     PPM_MEDIVAL_DEC50   1450
#define     PPM_MEDIVAL_ADD50   1550

#define     LPC_GPIOIN          LPC_GPIO1
#define     GPIO_Pin_IN1        GPIO_Pin_16
#define     GPIO_Pin_IN2        GPIO_Pin_14
#define     GPIO_Pin_IN3        GPIO_Pin_9
#define     GPIO_Pin_IN4        GPIO_Pin_4
#define     GPIO_Pin_IN5        GPIO_Pin_17
#define     GPIO_Pin_IN6        GPIO_Pin_15
#define     GPIO_Pin_IN7        GPIO_Pin_10
#define     GPIO_Pin_IN8        GPIO_Pin_8
#define     GPIO_Pin_INALL     ((uint32_t)0x0003C710)
#define     GPIO_Pin_INODD     ((uint32_t)0x00030600)   //IN_奇数引脚
#define     GPIO_Pin_INEVEN    ((uint32_t)0x0000C110)   //IN_偶数引脚

/**********Variables********************/

/**********Function Prototypes**********/
void PWM_CarCtrInit(void );
void MyTimer_InitConfig(void );
void PWM_CarChannelConfig(uint32_t pwmchanx, uint16_t duty);
void Car_GoForward(void );
void Car_GoBack(void );
void Car_Stop(void );

#ifdef __cplusplus
}
#endif

#endif
/************************************************************
End of filename
************************************************************/
