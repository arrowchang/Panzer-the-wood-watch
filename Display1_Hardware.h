#ifndef __DISPLAY1_HARDWARE_H
#define __DISPLAY1_HARDWARE_H
#include "myConfig.h"
#include "stm8s_gpio.h"

void GPIO_Init_Matrix(void);
void dspScan(char _col);
void dspLoad(unsigned char _r);
void KillShadow(void);
void TurnOffDSP(void);
void TurnOffDevice(void);

#endif
