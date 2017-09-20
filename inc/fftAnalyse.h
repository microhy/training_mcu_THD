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
#ifndef __FFTANALYSE_H
#define __FFTANALYSE_H

#ifdef __cplusplus
    extern "C" {
#endif 
/**********Includes*********************/
#include "../inc/LPC1768GCZX.h"
// #include ".h"

/**********Macros***********************/
#define NPOINTS_64			64
#define NPOINTS_256			256
#define NPOINTS_1024		1024
#define NPOINTS_4096		4096
#define NPOINTS				NPOINTS_256

typedef struct
{
    uint32_t cap0Val[2];
    FlagStatus Flag_Cap0;
    FlagStatus Flag_Cap0Done;
    uint32_t Signalperiod;
    uint32_t CntRisingEdge;
//    float Frequency;
}fftAnalyse_DataDef;

/**********Variables********************/
extern volatile fftAnalyse_DataDef fftVal;
extern volatile uint32_t adcSampVal[NPOINTS];
extern volatile short psi_Input[NPOINTS * 2];
extern volatile short psi_Output[NPOINTS * 2];

/**********Function Prototypes**********/
void vTest_PerformFFT(void);
void Measure_SignalPeriod(void );
void PeripInit_TIM(void );
void PeripInit_ADC(void );
void PeripInit_DMAChan2(void );

#ifdef __cplusplus
}
#endif

#endif
/************************************************************
End of filename
************************************************************/
