#ifndef __DISPLAY4_STATEMACHINE_H
#define __DISPLAY4_STATEMACHINE_H

#include "stm8s_awu.h"
#include "Display1_hardware.h"
#include "display3_app.h"
#include "clockSet.h"
#include "NAV_Detection.h"
#include "PowerManagement.h"
#include "RTC.h"
#include "stm8s_gpio.h"
#include "assistant.h"
#include "myConfig.h"

typedef enum{
	DSP_TIME = (unsigned char)0x01,
	DSP_TIMESET = (unsigned char)0x02,
	DSP_BATVALUE = (unsigned char)0x03,
	DSP_SEC = (unsigned char)0x04,
	DSP_HEARTBEAT = (unsigned char)0x05
	
} DspHMIFlag_TypeDef;

void assignTask(void);
void setDspStatus(DspHMIFlag_TypeDef dspFlag);
void AutoTurnOffDSP_Settings(unsigned char prolong,FunctionalState flag);

#endif