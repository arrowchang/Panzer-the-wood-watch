#ifndef __DISPLAY3_APP_H
#define __DISPLAY3_APP_H

#include "display2_mapping.h"
#include "myConfig.h"
#include "assistant.h"

void lightenDot(unsigned char *layer,unsigned char cooX,unsigned char cooY,FunctionalState _en);
void twinkleDot(unsigned char *layer,unsigned char cooX,unsigned char cooY);

void writeDigtal(unsigned char tens,unsigned char unit,unsigned char *layer);

void numDsp_Write(unsigned char num,Orientation_TypeDef ori,unsigned char *dspLayer,unsigned char font,FunctionalState randomFontEnable);
void TimeDspUpdate(unsigned char _num1,unsigned char _num2,unsigned char _num3,unsigned char _num4,FunctionalState randomFontEnable);
void DspPowerOnFlash(void);

static void numDspAdjust(unsigned char *decodeArr,Orientation_TypeDef ori);
static void getNumPointer(unsigned char **p,unsigned char _num,unsigned char font);

#endif