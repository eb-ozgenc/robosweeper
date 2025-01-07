#include "MKL25Z4.h"
#include <stdio.h>
#include <math.h>


#define MASK(x) (1UL << (x))

#define rs (16)
#define rw (13)
#define e (12)
#define d0 (11)
#define d1 (10)
#define d2 (6)
#define d3 (5)
#define d4 (4)
#define d5 (3)
#define d6 (0)
#define d7 (7)


void Delay(int n){while(n > 0){n --;}}

void LCD_setup(void){
	//SIM->SCGC5 |=  SIM_SCGC5_PORTC_MASK;
	PORTC->PCR[rs] |= PORT_PCR_MUX(1);
	PORTC->PCR[rw] |= PORT_PCR_MUX(1);
	PORTC->PCR[e] |= PORT_PCR_MUX(1);
	PORTC->PCR[d0] |= PORT_PCR_MUX(1);
	PORTC->PCR[d1] |= PORT_PCR_MUX(1);
	PORTC->PCR[d2] |= PORT_PCR_MUX(1);
	PORTC->PCR[d3] |= PORT_PCR_MUX(1);
	PORTC->PCR[d4] |= PORT_PCR_MUX(1);
	PORTC->PCR[d5] |= PORT_PCR_MUX(1);
	PORTC->PCR[d6] |= PORT_PCR_MUX(1);
	PORTC->PCR[d7] |= PORT_PCR_MUX(1);
	PTC->PDDR |= MASK(rs)|MASK(rw)|MASK(e)|MASK(d0)|MASK(d1)|MASK(d2)|MASK(d3)|MASK(d4)|MASK(d5)|MASK(d6)|MASK(d7);
	
}

void enable_pulse(void){
	PTC->PDOR |= MASK(e);
	Delay(5000);
	PTC->PDOR &= ~MASK(e);
	Delay(5000);
}

void send_command(char command){
	PTC->PDOR = 0;		
	PTC->PDOR &= ~MASK(rw); 
	PTC->PDOR &= ~MASK(rs);
	PTC->PDOR |= (((command & 0x1) >> 0) << d0);
	PTC->PDOR |= (((command & 0x2) >> 1) << d1);
	PTC->PDOR |= (((command & 0x4) >> 2) << d2);
	PTC->PDOR |= (((command & 0x8) >> 3) << d3);
	PTC->PDOR |= (((command & 0x10) >> 4) << d4);
	PTC->PDOR |= (((command & 0x20) >> 5) << d5);
	PTC->PDOR |= (((command & 0x40) >> 6) << d6);
	PTC->PDOR |= (((command & 0x80) >> 7) << d7);
	enable_pulse();
}	


void send_num(unsigned long command){
	command += 48;
	PTC->PDOR = 0;		
	PTC->PDOR &= ~MASK(rw); 
	PTC->PDOR |= MASK(rs);
	PTC->PDOR |= (((command & 0x1) >> 0) << d0);
	PTC->PDOR |= (((command & 0x2) >> 1) << d1);
	PTC->PDOR |= (((command & 0x4) >> 2) << d2);
	PTC->PDOR |= (((command & 0x8) >> 3) << d3);
	PTC->PDOR |= (((command & 0x10) >> 4) << d4);
	PTC->PDOR |= (((command & 0x20) >> 5) << d5);
	PTC->PDOR |= (((command & 0x40) >> 6) << d6);
	PTC->PDOR |= (((command & 0x80) >> 7) << d7);
	enable_pulse();
}

void send_data(char command){
	PTC->PDOR = 0;		
	PTC->PDOR &= ~MASK(rw); 
	PTC->PDOR |= MASK(rs);
	PTC->PDOR |= (((command & 0x1) >> 0) << d0);
	PTC->PDOR |= (((command & 0x2) >> 1) << d1);
	PTC->PDOR |= (((command & 0x4) >> 2) << d2);
	PTC->PDOR |= (((command & 0x8) >> 3) << d3);
	PTC->PDOR |= (((command & 0x10) >> 4) << d4);
	PTC->PDOR |= (((command & 0x20) >> 5) << d5);
	PTC->PDOR |= (((command & 0x40) >> 6) << d6);
	PTC->PDOR |= (((command & 0x80) >> 7) << d7);
	enable_pulse();
}	

void LCD_initialize(void){
	send_command(0x38);
	send_command(0x01);
	send_command(0x0F);
}

void LCD_write(unsigned long n){

	send_num((n%100)/10);
	send_num((n%10)/1);
}

void writetime(unsigned long sec) {
	LCD_write(sec/60);
	send_data(':');
	LCD_write(sec%60);}

void timeleftmsg(void){
	send_command(0xC0);
	send_data('T');
	send_data('i');
	send_data('m');
	send_data('e');
	send_data(' ');
	send_data('L');
	send_data('e');
	send_data('f');
	send_data('t');
	send_data(':');
}	

void onmsg(void){
	send_command(0x80);
	send_data('S');
	send_data('w');
	send_data('e');
	send_data('e');
	send_data('p');
	send_data('i');
	send_data('n');
	send_data('g');
}	

void offmsg(void){
	send_command(0x80);
	send_data('S');
	send_data('t');
	send_data('o');
	send_data('p');
	send_data('p');
	send_data('e');
	send_data('d');
	send_data(' ');
}	

