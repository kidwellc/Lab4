/**************************************************************************************
* Author: Christina Kidwell
* Course: EGR 226 - 905
* Date: 02/18/2021
* Project: Lab 4 Part 1
* File: main.c
* Description: A program that cycles the RGB LED through Red, Green & Blue colors,
* changing every time a button is pressed.
*
**************************************************************************************/
#include "msp.h"

void setupIO(void);
void delay_ms(int delay);
int state(void);
int waitForPress(void);

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	setupIO();

	while(1) {
        while(waitForPress()==0); // wait for button press
        // turn of RGB LED to Red
        P2->OUT |= BIT0;
        P2->OUT &= ~BIT1;
        P2->OUT &= ~BIT2;
        while(waitForPress()==0); // wait for button press
        // turn of RGB LED to Green
        P2->OUT &= ~BIT0;
        P2->OUT |= BIT1;
        P2->OUT &= ~BIT2;
        while(waitForPress()==0); // wait for button press
        // turn of RGB LED to Blue
        P2->OUT &= ~BIT0;
        P2->OUT &= ~BIT1;
        P2->OUT |= BIT2;
	}

}

/****| setupIO | ***********************************************
* Brief: Function to configure the input and output bits used
* for the LEDs and switches
* Param:
* none
* return:
* N/A
*************************************************************/
void setupIO(void){
    // Configure Red LED, special function bits to 0, direction to output, resistor off
    P1->SEL0 &= ~BIT0;
    P1->SEL1 &= ~BIT0;
    P1->DIR |= BIT0;
    P1->REN &= ~BIT0;

    // Configure RGB Red LED, special function bits to 0, direction to output, resistor off
    P2->SEL0 &= ~BIT0;
    P2->SEL1 &= ~BIT0;
    P2->DIR |= BIT0;
    P2->REN &= ~BIT0;

    // Configure RGB Green LED, special function bits to 0, direction to output, resistor off
    P2->SEL0 &= ~BIT1;
    P2->SEL1 &= ~BIT1;
    P2->DIR |= BIT1;
    P2->REN &= ~BIT1;

    // Configure RGB Blue LED, special function bits to 0, direction to output, resistor off
    P2->SEL0 &= ~BIT2;
    P2->SEL1 &= ~BIT2;
    P2->DIR |= BIT2;
    P2->REN &= ~BIT2;

    // Configure Switch 1, special function bits to 0, direction to input, resistor enabled as a pull up
    P1->SEL0 &= ~BIT1;
    P1->SEL1 &= ~BIT1;
    P1->DIR &= ~BIT1;
    P1->REN |= BIT1;
    P1->OUT |= BIT1;

    // Configure Switch 2, special function bits to 0, direction to input, resistor enabled as a pull up
    P1->SEL0 &= ~BIT4;
    P1->SEL1 &= ~BIT4;
    P1->DIR &= ~BIT4;
    P1->REN |= BIT4;
    P1->OUT |= BIT4;

    // turn off Red LED
    P1->OUT &= ~BIT0;

    // turn off RGB LED
    P2->OUT &= ~BIT0;
    P2->OUT &= ~BIT1;
    P2->OUT &= ~BIT2;
}

/****| delay_ms | ***********************************************
* Brief: Function that uses __delay_cycles to generate a delay
* of a given number of milliseconds
* Param:
* int delay = number of milliseconds to delay
* return:
* N/A
*************************************************************/
void delay_ms(int delay){
    int i;
    for(i=0;i<delay;i++) __delay_cycles(3000);
}

/****| state | ***********************************************
* Brief: loop until the condition of the switch is consistent
* for two consecutive readings that are separated by 15ms
* Param:
* none
* return:
* 0 if button pressed
* 1 if button not pressed
*************************************************************/
int state(void){
    int temp,temp2;
    temp=P1->IN & BIT1; // get initial switch condition and save it
    while(1){
        delay_ms(15); // delay a bit
        temp2 = P1->IN & BIT1; // get another switch condition
        if (temp==temp2){ // if the new switch condition matches the saved one
            if(temp==0) return 0;
            else return 1; // then return the saved value
        }
        else {
            temp = temp2; // otherwise, update saved condition with new one
        }
    }
}


/****| waitForPress | ***********************************************
* Brief:
* if the button is not being pressed, return a 0
* if the button is being pressed, wait for release, then return a 1
* Param:
* none
* return:
* 0 if button pressed
* 1 if button not pressed
*************************************************************/
int waitForPress(void) {
    if(state()==0) return 0; //if button is pressed, return 0
    while (state()==1);      //if button is not pressed, wait for it to be pressed
    return 1;                //then return 1;
}
