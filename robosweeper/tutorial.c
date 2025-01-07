
#include <MKL25Z4.H>
#include <stdio.h>
#include <math.h>
 
#include "motorcommands.h"
#include "lcdcommands.h"


#define MASK(x) (1UL << (x))

//port D - sensors
#define limitsw (0)
#define irleft (3)
#define irright (5)
#define irfront (2)
//#define irback (1)

//PORT A - buttons
#define swstartstop (2)
#define swtimeset (1)




 volatile unsigned long timeleft = 0;
 volatile unsigned long startstop = 0;
 volatile unsigned long icount = 0;

 
void sensorsinit (void) {
  PORTD->PCR[limitsw] &= ~PORT_PCR_MUX_MASK; 
	PORTD->PCR[limitsw] |= PORT_PCR_MUX(1)  |2UL | PORT_PCR_IRQC(0x9); //assigned as GPIO and pullup enabled, interrupt enabled 

  PORTD->PCR[irleft] &= ~PORT_PCR_MUX_MASK; 
	PORTD->PCR[irleft] |= PORT_PCR_MUX(1)  |2UL| PORT_PCR_IRQC(0x9); 
  PORTD->PCR[irright] &= ~PORT_PCR_MUX_MASK; 
	PORTD->PCR[irright] |= PORT_PCR_MUX(1)  |2UL| PORT_PCR_IRQC(0x9); 
  PORTD->PCR[irfront] &= ~PORT_PCR_MUX_MASK; 
	PORTD->PCR[irfront] |= PORT_PCR_MUX(1)  |2UL| PORT_PCR_IRQC(0x9); 


	PTA->PDDR &= ~(MASK(limitsw)  | MASK(irright) | MASK(irleft) | MASK(irfront));
}


void buttonsinit (void){
	PORTA->PCR[swstartstop] &= ~PORT_PCR_MUX_MASK; 
	PORTA->PCR[swstartstop] |= PORT_PCR_MUX(1) | 3UL | PORT_PCR_IRQC(0xA);
	
	PORTA->PCR[swtimeset] &= ~PORT_PCR_MUX_MASK; 
	PORTA->PCR[swtimeset] |= PORT_PCR_MUX(1) | 3UL | PORT_PCR_IRQC(0xA);
	
	PTA->PDDR &= ~(MASK(swstartstop) | MASK(swtimeset));
}





int main (void) {
    SIM->SCGC5 |=  SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK ;	// enable clock to Port A , C and D, E
    motorsinit();
		buttonsinit();
	  sensorsinit();
	
		__disable_irq();
		__enable_irq();
		NVIC_EnableIRQ(PORTD_IRQn);
		NVIC_EnableIRQ(PORTA_IRQn);
		
		NVIC_SetPriority(PORTA_IRQn, 1);
		NVIC_SetPriority(PORTD_IRQn, 0);

		PORTD->PCR[1] &= ~PORT_PCR_MUX_MASK; //PTD1 BLUE LED FOR DEBUGGING
		PORTD->PCR[1] |= PORT_PCR_MUX(1);
		PTD->PDDR |= MASK(1) ;

		stop();
		mainbrushoff();
		
		LCD_setup();
		LCD_initialize();
		
		startstop = 0;
		timeleft = 0;
		send_command(0x0C);
		
			while (1){
							
				if (timeleft && startstop) {					
					mainbrushon();
					moveforward();
					onmsg();
					timeleft--;
					delay(1582000);
					timeleftmsg();
					writetime(timeleft);}
				
				else {
					timeleftmsg();
					writetime(timeleft);
					stop();
					mainbrushoff();
					offmsg();
					__WFI();}}
				
}

void PORTA_IRQHandler(void){
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	//PTD->PTOR =MASK(1);
	if (PORTA->ISFR & MASK(swstartstop)){
		if (startstop) {startstop = 0;}
		else {startstop = 1;}}
	if (PORTA->ISFR & MASK(swtimeset)){
		timeleft += 60;}
	PORTA->ISFR = 0xFFFFFFFF;
}
 
void PORTD_IRQHandler(void) {
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	icount++;
	if (icount >0) {
	icount = 0;
	PTD->PTOR =MASK(1);}
	if (startstop){
	if (PORTD->ISFR & MASK(limitsw) ){
		moveback();
		turnleft();
		}
	else if (PORTD->ISFR & MASK(irleft)){
		turnright();
		}
	else if (PORTD->ISFR & MASK(irright)){
		turnleft();
		}
	else if (PORTD->ISFR & MASK(irfront)){
		//moveback();
		turnleft();
		}}
	PORTD->ISFR = 0xFFFFFFFF; //clear pending interrupt and status flags
}




