/**
  ****************************************************************************** 
  * @author   eastwood.work
  * @version  /
  * @date     /
  * @brief    Main Interrupt Service Routines.
  */ 
/* Includes ------------------------------------------------------------------*/
#include "stm8s_it.h"
#include "stm8s_gpio.h"
#include "stm8s_tim4.h"
#include "stm8s_adc1.h"
#include "stm8s_awu.h"

#include "myConfig.h"
#include "assistant.h"
#include "display1_hardware.h"
#include "rtc.h"
#include "nav_detection.h"


bool BUTTON_PRESS_DETECTED = FALSE;
bool CHARGE_DETECTED = TRUE;
bool RTC_UPDATE_EN = TRUE;
bool BAT_TWINKLE_FLAG = FALSE;
unsigned char iScan = 0;
signed char AutoTurnOffDsp = 0;
bool AutoTurnOffDsp_EN = TRUE;
unsigned char ProlongDSP = 0;


extern bool DSP_EN,
						DSP_LAYER1_EN,
						DSP_LAYER2_EN,
						DSP_LAYER3_EN;
extern bool DSP_BOMB_SHOOT_PATH_EN,
						DSP_BOMB_DROP_PATH_EN,
						DSP_EJet_PATH_EN,
						PATH_MOVE_EN_1,
						PATH_MOVE_EN_2;
extern unsigned char BOMB_SHOOT_SPEED,
										 BOMB_DROP_SPEED;
extern unsigned char Sec;
extern unsigned char BG[],LAYER_1[],LAYER_2[],LAYER_3[];

/**
  * @brief  Auto Wake Up Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(AWU_IRQHandler, 1)
{
   /* Clear AWU peripheral pending bit */
  AWU_GetFlagStatus();
}

/**
  * @brief  Button INT
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)
{
  BUTTON_PRESS_DETECTED = TRUE;
	AutoTurnOffDsp = ProlongDSP;//any button pressing will re-load AutoTurnOffDsp
	
}

/**
  * @brief  External Interrupt PORTB Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
{
	RTC_UPDATE_EN = TRUE;
	
}
/**
  * @brief  Charger status INT
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{	
	CHARGE_DETECTED = TRUE;
	AutoTurnOffDsp = ProlongDSP;//any button pressing will re-load AutoTurnOffDsp
	
}

/**
  * @brief  Timer4 Update/Overflow Interrupt routine
  * @param  None
  * @retval None
	*	@note   f(cpu) should > 128KHz,else system will get stuck due to 
						the interruption has too much work to deal.
  */
 INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
	unsigned char merged, col;
	
	/*[.1] matrix scanning*/
	if(DSP_EN){
		col = iScan % MATRIX_X_LEN;
		
		merged = BG[col];
		if(DSP_LAYER1_EN){
			merged |= LAYER_1[col];
		}
		if(DSP_LAYER2_EN){
			merged |= LAYER_2[col];
		}
		if(DSP_LAYER3_EN){
			merged |= LAYER_3[col];
		}
		
		KillShadow();
		dspLoad(merged);
		dspScan(col);
		
	}
	
	/*[.2] for bomb path dsp*/
	if(DSP_BOMB_SHOOT_PATH_EN){
		if((iScan & BOMB_SHOOT_SPEED) == 0)
			PATH_MOVE_EN_1 = TRUE;
	}
	
	if(DSP_BOMB_DROP_PATH_EN){
		if((iScan & BOMB_DROP_SPEED) == 0){
			PATH_MOVE_EN_2 = TRUE;
			
		}
	}
	
	/*Twinkle and Auto Turn Off*/
	if((iScan % 126) ==0){//360ms
		BAT_TWINKLE_FLAG = TRUE;
		
		if(AutoTurnOffDsp_EN){
			AutoTurnOffDsp--;
			if(AutoTurnOffDsp == 0)
				AutoTurnOffDsp_EN = FALSE;
			
		}
		
	}
	/*[.3] clear pending bit*/
	iScan++;
	if(iScan==252)//for that : 252%7=0, is the max value close to 255;
								//else the display will "shake".
		iScan=0;
	
	TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
}

