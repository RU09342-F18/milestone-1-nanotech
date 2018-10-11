/*
 * main.c for Milestone
 *
 *    Created on: October 10, 2018
 *   Last Edited:
 *        Author: Tyler M. and Helen P.
 *         Board: MSP430G2553
 */


#include <msp430.h>


/*
Test Code Definitions
*/


#define Btn BIT3                            // Define "Btn" as bit 3.
#define LED BIT0                            // Define "LED0" as bit 0.
#define PnB (P1IN & Btn)                    // Define "INP" for checking if there is an input on pin 1.3.
typedef int bool;
#define true 1
#define false 0


/*
 * Main Function
 */

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	            // Stop watchdog timer
	TestCode();                             // TestCode Function
	return 0;
}


// LED Test Code

// P1.0 Will SWITCH when you push the button, use this to test LEDs
void TestCode()
{
	WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
	bool i = 0;                             // Create a boolean called "i"

	P1SEL &= (~LED & ~Btn);                 // On P1 Select, set bits "0" and "3" to the value "0"

	P1DIR |= LED;                           // Set Port 1.0 as an output
	P1OUT &= ~LED;                          // Set the initial value of port 1.0 as "0"

	P1DIR &= ~Btn;                          // Set Port 1.3 as an input
	P1REN |= Btn;                           // Turn on the pull-up resistor for port 1.3
	P1OUT |= Btn;                           // Set the initial value of port 1.3 as "1"

	while(1)                                // Start an infinite while loop.
    {                   
		if(!PnB && !i)                      // If the button is pushed and the boolean is false
        {         
					P1OUT ^= LED;           // Flip the value of the LED
					i = 1;                  // Flip the Value of the Boolean
		}
		else if(PnB && i)                   // If the button is pushed and the boolean is true
        {     
				   i = 0;                   // Set the Boolean to be true
	    }

	}
}

