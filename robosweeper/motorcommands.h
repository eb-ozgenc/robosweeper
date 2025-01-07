#include <MKL25Z4.H>
#include <stdio.h>
#include <math.h>

#define MASK(x) (1UL << (x))

//all motors in port e
#define rightwheelfront (20)
#define rightwheelback (21)
#define leftwheelfront (22)
#define leftwheelback (23)
#define mainbrush (29)
//#define cornerbrush (30)

void delay(int n){while(n > 0){n --;}}

void motorsinit (void) {
    PORTE->PCR[rightwheelfront] &= ~PORT_PCR_MUX_MASK; 
    PORTE->PCR[rightwheelfront] |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
		PORTE->PCR[rightwheelback] &= ~PORT_PCR_MUX_MASK; 
    PORTE->PCR[rightwheelback] |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	
    PORTE->PCR[leftwheelfront] &= ~PORT_PCR_MUX_MASK; 
    PORTE->PCR[leftwheelfront] |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	  PORTE->PCR[leftwheelback] &= ~PORT_PCR_MUX_MASK; 
    PORTE->PCR[leftwheelback] |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;

    PORTE->PCR[mainbrush] &= ~PORT_PCR_MUX_MASK; 
    PORTE->PCR[mainbrush] |= PORT_PCR_MUX(1)| PORT_PCR_DSE_MASK;
//    PORTE->PCR[cornerbrush] &= ~PORT_PCR_MUX_MASK;  
//    PORTE->PCR[cornerbrush] |= PORT_PCR_MUX(1);

		PTE->PDDR |= MASK(rightwheelfront) | MASK(leftwheelfront) |MASK(rightwheelback) | MASK(leftwheelback) | MASK(mainbrush); //| MASK(cornerbrush);
}

void mainbrushon (void) { PTE->PSOR = MASK(mainbrush);}
void mainbrushoff (void) { PTE->PCOR = MASK(mainbrush);}

void stop(void) {PTE->PCOR = MASK(rightwheelback) | MASK(leftwheelfront) | MASK(rightwheelfront)| MASK(leftwheelback);}


void moveforward (void) {
	PTE->PCOR = MASK(rightwheelback) | MASK(leftwheelback);
	PTE->PSOR = MASK(rightwheelfront) | MASK(leftwheelfront);
	}

void moveback (void) {
	mainbrushoff();
	PTE->PCOR = MASK(rightwheelfront) | MASK(leftwheelfront);
	PTE->PSOR = MASK(rightwheelback) | MASK(leftwheelback);
	delay(632600*2);
	stop();
	mainbrushon();}

void turnright (void) {
	stop();
	PTE->PSOR = MASK(rightwheelback) | MASK(leftwheelfront);
	PTE->PCOR =  MASK(rightwheelfront)| MASK(leftwheelback);
	delay(632600);
	moveforward();}

void turnleft (void) {
	PTE->PCOR = MASK(rightwheelback) | MASK(leftwheelfront);
	PTE->PSOR =  MASK(rightwheelfront)| MASK(leftwheelback);
	delay(632600);
	moveforward();}

//void cornerbrushon (void) { PTE->PSOR = MASK(cornerbrush);}
//void cornerbrushoff (void) { PTE->PCOR = MASK(cornerbrush);}
