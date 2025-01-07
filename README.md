# robosweeper
Robot Street Sweeper using FRDM-KL25Z board and C language for Bilkent EEE 212 - Microcontrollers and Embedded Systems Term Project
For my EEE212 Term project, I implemented a robot street sweeper controlled by FRDM-KL25Z. 

## Code
The project is made using Keil uVision 5 IDE compatible with uploading machine code to KL25Z. 
There is a main module and 2 header files in the uVision project (one for LCD commands and 1 for motor commands). 
LCDcommands.h include functions like initializing a 16x2 LCD, displaying status messages and time left. 
motorcommands.h include functions for the wheels like moving foward/back or turning left/right and the brushes (on and off). 
main.c counts the time and generate appropriate responses for inputs from IR sensors and limit switch located on the front of the robot

## Peripherals
- 1 x 16x2 LCD screen
- 3 x IR sensors
- 1 x limit switch
- 3 x DC motors
- 2 x L298N voltage converter / DC motor driver (1 can drive 2 DC motors)
- Plastic chassis and bumper for limit switch

## Project Video
https://youtu.be/uIou6MBt0Hw

## Car edit (but instead for robosweeper)
https://youtube.com/shorts/qGp7WotBsZs
