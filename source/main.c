/*	Author: Bryan Dang
 *  Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

// demo link : https://drive.google.com/file/d/1DxcjrzU8u1X8imdgK02lS6oYNTiE28Ag/view?usp=drivesdk
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum state{start, first, second, third} state;
unsigned char B = 0x00;

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;                
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff(){
	TCCR1B = 0x00;
}
void TimerISR(){
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void Tick(){
	switch(state){
		case start:
		state = first;
		break;

		case first:
		state = second;
		break;
		
		case second:
		state = third;
		break;

		case third:
		state = first;
		break;
	}

	switch(state){
		case start:
		break;

		case first:
		B = 0x01;
		break;

		case second:
		B = 0x02;
		break;

		case third:
		B = 0x04;
		break;
		
		default:
		break;
	}

}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF;
	PORTB = 0x00;
	TimerSet(1000);
	TimerOn();
    /* Insert your solution below */
    while (1) {
	PORTB = B;
	Tick();
	while (!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
