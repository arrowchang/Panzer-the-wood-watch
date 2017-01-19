/**
  ****************************************************************************** 
  * @author   eastwood.work
  * @version  /
  * @date     /
  * @brief    Manage the tasks by setting status falgs.
  */ 
/* Includes ------------------------------------------------------------------*/
#include "Display4_StateMachine.h"

bool DSP_EN = TRUE,
		 DSP_TIME_EN = TRUE,
		 DSP_TIMESET_EN = FALSE,
		 DSP_BATVALUE_EN = FALSE,
		 DSP_SEC_EN = FALSE;

extern bool TIME_DSP_REFRESH;
extern bool DSP_HEARTBEAT_EN;
extern bool AutoTurnOffDsp_EN,BUTTON_PRESS_DETECTED;
extern unsigned char NAV,AutoTurnOffDsp,ProlongDSP;
extern unsigned char BG[];
/**
  * @brief  Set the status flag (CMD) to switch DSP Mode.
  * @param  dspFlag: can be one of the DspHMIFlag_TypeDef defined.
  * @retval None
	*	@note		ONLY ALLOW ONE FLAG IS SET AT ONE TIME
	* @date		2016-x-x; 2016-05-29: attached "TIME_DSP_REFRESH = TRUE" if
						DSP_TIME is enabled.
  */
void setDspStatus(DspHMIFlag_TypeDef dspFlag){
	DSP_TIME_EN = FALSE;
	DSP_TIMESET_EN = FALSE;
	DSP_BATVALUE_EN = FALSE;
	DSP_SEC_EN = FALSE;
	DSP_HEARTBEAT_EN =FALSE;
	
	switch(dspFlag){
		case DSP_TIME: 
				DSP_TIME_EN = TRUE;
				timeDisplay_Init();
				break;
				
		case DSP_TIMESET: 
				DSP_TIMESET_EN = TRUE;
				clockSet_Init();
				break;
				
		case DSP_BATVALUE:
				DSP_BATVALUE_EN = TRUE;
				dspBatteryValue_Init();
				break;
				
		case DSP_SEC:
				DSP_SEC_EN = TRUE;
				secDisplay_Init();
				break;
				
		case DSP_HEARTBEAT:
				DSP_HEARTBEAT_EN = TRUE;
				heartBeating_Init();
				break;
		default:break;
	}
}
/**
  * @brief  Core function. Acts as an ¡°OS" to assign tasks.
  * @param  None
  * @retval None
	*	@note		the function is a state-machine. waked up by button pressing.
	* @date		2016-x-x;
*/	
void assignTask(void){
	ButtonStatus_TypeDef bt;
	
	bt = getButtonStatus();
	if(bt == SHORT_PRESS){
		if(!DSP_EN){
			DSP_EN = TRUE;//turn on diaplay to continue the work
			
		}else{
			if(DSP_TIME_EN){
				getNAV_Value();// get navigation value from sensor
				
				switch(NAV){
					case NAV_NORTH: 
						setDspStatus(DSP_BATVALUE);break;
					case NAV_WEST:
						setDspStatus(DSP_TIMESET);break;
					case NAV_EAST:
						setDspStatus(DSP_SEC);break;
					case NAV_SOUTH:
						DSP_EN = FALSE;break;	
					default:break;
				}
			}else	if(DSP_BATVALUE_EN){//cannnot without "else",will get logic mistake
				setDspStatus(DSP_TIME);//go to desktop
			}else if(DSP_TIMESET_EN){
				getNAV_Value();
			}else if(DSP_SEC_EN){
				getNAV_Value();
				switch(NAV){
					case NAV_WEST:
						setDspStatus(DSP_TIME);break;
					case NAV_SOUTH:
						TurnOffDevice();break;
					default:break;
				}
			}else if(DSP_HEARTBEAT_EN){
				setDspStatus(DSP_TIME);
				
			}
			
		}
	}else if(bt == LONG_PRESS){
		CLR_DSPLayer(DEFAULT_LAYER);
		/*display flash while button is still pressed*/
		//DspPowerOnFlash();
		while(GPIO_ReadInputPin(BUT_DETECTION));
		TurnOffDevice();//wait untill released the button
														 // to turn off the system pwr
														 //(a real pwr off)
		
	}
	
}

/**
  * @brief  Auto turn off diaplay flags setting.
  * @param  prolong: the time prolonged before turning off.
						flag:	EN/DISABLE AutoTurnOffDsp function. 
  * @retval None
	*	@note		if flag=DISABLE, "prolong" can be any value.
	* @date		 2016-x-x;
*/	
void AutoTurnOffDSP_Settings(unsigned char prolong,FunctionalState flag){
	ProlongDSP = prolong;
	AutoTurnOffDsp = prolong;
	AutoTurnOffDsp_EN = flag;
	
}

