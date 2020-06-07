/*	Author: Jarvis Bui
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "bit.h"
#include "io.c"
#include "io.h"
#include "timer.h"
#include "scheduler.h"
#include "keypad.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
unsigned char key = 0x00;
unsigned char cursor = 0x01;
enum states{write}state;
int tick(int state){
	switch(state){
		case write:
			key = GetKeypadKey();
			if(key != '\0'){
				LCD_WriteData(key);
				if(cursor > 15){
					cursor = 1;
					LCD_Cursor(cursor);
				}
				else{
					cursor++;
					LCD_Cursor(cursor);
				}
			}
			break;
		default:
			break;
	}
	return state;

}

int main(void) {
    /* Insert DDR and PORT initializations */

	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0xFF; PORTA = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
	
	unsigned long int SMTick1_calc = 500;
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(tmpGCD, SMTick1_calc);
	unsigned long int GCD = tmpGCD;
	unsigned long int SMTick1_period = SMTick1_calc/GCD;
	
	static task task1;
	task *tasks[] = {&task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	task1.state = 0;
	task1.period =SMTick1_period;;
	task1.elapsedTime = SMTick1_period; 
	task1.TickFct = &tick;

	TimerSet(1000);
	TimerOn();
	LCD_init();
	const char* msg = "Congrations!";
	LCD_DisplayString(1,msg);
	LCD_Cursor(cursor);

    while (1) {
	    int i;
	for( i = 0; i < numTasks; i++){
		if(tasks[i]->elapsedTime == tasks[i]->period){
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += 1;
	}

	switch(key){
		case '\0': PORTB = 0x1F; break; //all 5 leds on 
		case '1': PORTB = 0x01; break; //hex values from here on out 
		case '2': PORTB = 0x02; break;
		case '3': PORTB = 0x03; break;
		case '4': PORTB = 0x04; break;
		case '5': PORTB = 0x05; break;
		case '6': PORTB = 0x06; break;
		case '7': PORTB = 0x07; break;
		case '8': PORTB = 0x08; break;
		case '9': PORTB = 0x09; break;
		case 'A': PORTB = 0x0A; break;
		case 'B': PORTB = 0x0B; break;
		case 'C': PORTB = 0x0C; break;
		case 'D': PORTB = 0x0D; break;
		case '*': PORTB = 0x0E; break;
		case '0': PORTB = 0x00; break;
		case '#': PORTB = 0x0F; break;
		default: PORTB = 0x1B; break; // should never ovvur
	
		while(!TimerFlag);
		TimerFlag = 0;
	}
    }
    return 1;
}
