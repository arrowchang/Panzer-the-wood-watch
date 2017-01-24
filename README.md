# Panzer-the-wood-watch
<br>The code is based on ST lib STM8S_StdPeriph_Lib_V2.1.0<br>
<br>Please refer to schematic to understanding the bottom layer algorithm.
<br>myConfig.h                <br>Mainly defined the MCU ports serving for LED matrix and other constants for Typedef.
<br>main.c                    <br>A loop switched by flags. Display time when powered on; Power the system off if nothing to do.
<br>Display1_Hardware.c       <br>Driver for LED Matrix, works with TIM_INT server function.
<br>Display2_Mapping.c        <br>Mapping the LED matrix to arrays. A pixel is controled by a bit.
<br>Display3_app.c            <br>Functions to manage Dice font display.
<br>Display4_StateMachine.c   <br>A state machine to deploy tasks.
<br>stm8s_it.c                <br>INT server. Multiple tasks for TIM4_INT(matrix scanning, timer unit,...,etc.)<br>
<br>Plaese note following files not given:
<br>clockSet.c                <br>Setting RTC by a game-like play.
<br>NAV_Detection.c           <br>Get the orientation of the watch.
<br>PowerManagement.c         <br>Power management to prolong battery lifetime.
<br>RTC.c                     <br>RTC chip setting/RD/WT.<br>
