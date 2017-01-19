#include "stm8s.h"

/************************NOTE**************************
20161221.
1. TimeSetting: Changed the game rule to "plus"/"minus".
2. TWINKLE MODE: EOL
3. TIME DISPLAY: random font every time checking. New method to getRandomNum function.
4. RTC offset Val = 16 for Bocote-Case
/******************************************************/

/*I2C for RTC*/
#define I2C_SCL GPIOB,GPIO_PIN_4
#define I2C_SDA GPIOB,GPIO_PIN_5
#define INT_RTC GPIOB,GPIO_PIN_7
#define CLK_RTC GPIOD,GPIO_PIN_4//[PCB V4.0]
/*UART for Debug*/
#define UART_Tx GPIOD,GPIO_PIN_5
#define UART_Rx GPIOD,GPIO_PIN_6
/*NAVIGATION RPI1031 DETECTION/CONTROL GPIOs*/
#define RPI_ON GPIOB,GPIO_PIN_1

#define NAV_PORT GPIOB
#define NAV_1 NAV_PORT,GPIO_PIN_2
#define NAV_2 NAV_PORT,GPIO_PIN_6
#define NAV_MASK 0x44

/*BATTERY MONITOR GPIO*/
#define ADC_VBAT GPIOB,GPIO_PIN_3

/*CHARGER TP4055 STATUS DETECTION*/
#define CHARGE_STATUS GPIOD,GPIO_PIN_3//[PCB VERSION 4.0]

/*POWER ON/OFF DETECTION/CONTROL GPIOs*/
#define SW_PWR_ON GPIOD,GPIO_PIN_7//GPIO_PIN_0
#define BUT_DETECTION GPIOA,GPIO_PIN_3
#define BUT_DETECTION_EXTI_PORT EXTI_PORT_GPIOA

/*LED MATRIX RELATED GPIOs*/
#define LVC138_EN GPIOD,GPIO_PIN_2
#define LVC138_CS0 GPIOC,GPIO_PIN_1
#define LVC138_CS1 GPIOE,GPIO_PIN_5
#define LVC138_CS2 GPIOB,GPIO_PIN_0

#define MATRIX_R_PORT1 GPIOC
#define MATRIX_R_PORT2 GPIOD
//[PCB V4.0]
#define PANZER 1
#ifdef PANZER//PCB V4.0 Bug (Matrix placing reversed)
	#define MATRIX_R0 MATRIX_R_PORT1,GPIO_PIN_2
	#define MATRIX_R1 MATRIX_R_PORT1,GPIO_PIN_3
	#define MATRIX_R2 MATRIX_R_PORT1,GPIO_PIN_4
	#define MATRIX_R3 MATRIX_R_PORT1,GPIO_PIN_5
	#define MATRIX_R4 MATRIX_R_PORT1,GPIO_PIN_6
	#define MATRIX_R5 MATRIX_R_PORT1,GPIO_PIN_7
	#define MATRIX_R6 MATRIX_R_PORT2,GPIO_PIN_0
	
#else
	#define MATRIX_R6 MATRIX_R_PORT1,GPIO_PIN_2
	#define MATRIX_R5 MATRIX_R_PORT1,GPIO_PIN_3
	#define MATRIX_R4 MATRIX_R_PORT1,GPIO_PIN_4
	#define MATRIX_R3 MATRIX_R_PORT1,GPIO_PIN_5
	#define MATRIX_R2 MATRIX_R_PORT1,GPIO_PIN_6
	#define MATRIX_R1 MATRIX_R_PORT1,GPIO_PIN_7
	#define MATRIX_R0 MATRIX_R_PORT2,GPIO_PIN_0
#endif

#define MATRIX_X_LEN 7//LED Matrix Width
#define MATRIX_Y_LEN 7//LED Matrix Hight,should <=8

/*For Num_x const compact code*/
#define LEN0 1
#define LEN1 2
#define LEN2 2
#define LEN3 5
#define LEN4 5
#define LEN5 6
#define LEN6 6
#define LEN7 3
#define LEN8 4
#define LEN9 1

/*For NAV value*/
#define CHARGING 0
#define CHARGED 1//
#define BAT_IS_CHARGING (GPIO_ReadInputPin(CHARGE_STATUS) == CHARGING)

#define UNIT_BAT (((651-512)/MATRIX_Y_LEN)+1)
#define GET_VBAT_FAIL 255

#define NO_LIMITION 255
#define NO_SHIFT 0

#define CLR_ALL  0x00
#define CLR_HIGH_NIBBLE 0x0f
#define CLR_LOW_NIBBLE 0xf0

#define EASTWARDS_1 ((unsigned char)0x10)
#define WESTWARDS_1 EASTWARDS_1
#define NORTHWARDS_1 ((unsigned char)1)
#define SOUTHWARDS_1 NORTHWARDS_1

#define DEFAULT_LAYER BG
#define FLAG_360MS BAT_TWINKLE_FLAG

#define DONOTCARE 0x00
/*=========================I2C================================*/
#define I2C_SPEED 100000

/* definition of 10-bit or default 7-bit slave address */
#define OWN_ADDRESS 0xaa//can be any unique address
#define RTC_ADDRESS 0xA2

#define RTC_CR0_ADDR 0x00
#define RTC_CR1_ADDR 0x01
#define RTC_OFFSET_ADDR 0x02
#define SEC_REG_ADDR 0x04
#define MIN_REG_ADDR 0x05
#define HOUR_REG_ADDR 0x06
#define DAY_REG_ADDR 0x07
#define WEEKDAY_REG_ADDR 0x08
#define MOUTH_REG_ADDR 0x09
#define YEARS_REG_ADDR 0x0a

#define RTC_RST 0x58
#define RTC_CR0_VAL 0x01//NormalMode,RTC Runs,No SW RST,No Correction INT,
												//24h mode,Load CAP 12.5pF
#define RTC_CR1_VAL 0x27//INT every 1 min,No CLKOUT
#define RTC_OFFSET_VAL (0x80 | 16)//for Bocote-Case//0x00//for PCB V4.0 TEST//(0x80 | 14)//for +5s @ 24h calibration//
//#endif

/* This define is used in master receiver */
/* Uncomment the line below if you want to use the safe procedure */
//#define SAFE_PROCEDURE


#define BUFFERSIZE  2//10
#define BUFFERSIZE_2 7

//typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
