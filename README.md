# Panzer-the-wood-watch
# The code is based on ST lib STM8S_StdPeriph_Lib_V2.1.0
# Please refer to schematic to understanding the bottom layer algorithm
myConfig.h                Mainly defined the MCU ports serving for LED matrix and other constants for Typedef.
main.c                    A loop switched by flags. Display time when powered on; Power the system off if nothing to do.
Display1_Hardware.c       Driver for LED Matrix, works with TIM_INT server function.
Display2_Mapping.c        Mapping the LED matrix to arrays. A pixel is controled by a bit.
Display3_app.c            Functions to manage Dice font display.
Display4_StateMachine.c   A state machine to deploy tasks.
stm8s_it.c                INT server. Multiple tasks for TIM4_INT(matrix scanning, timer unit,...,etc.)
# Plaese note following files not given:
clockSet.c                Setting RTC by a game-like play.
NAV_Detection.c           Get the orientation of the watch.
PowerManagement.c         Power management to prolong battery lifetime.
RTC.c                     RTC chip setting/RD/WT.
