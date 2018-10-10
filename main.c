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

#define Btn BIT3                 //Define "Btn" as bit 3.
#define LED BIT0                   //Define "LED0" as bit 0.
#define PnB (P1IN & Btn)        //Define "INP" for checking if there is an input on pin 1.3.
typedef int bool;
#define true 1
#define false 0


/*
 * Code Definition
 */

#define RedLED      BIT1;
#define GreenLED    BIT2;
#define BlueLED     BIT3;

int ByteCount = 0;
void TimerSetup(int rate);
void UARTSetup();


/*
 * Main Function
 */

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	            // Stop watchdog timer
	UARTSetup();
	//TestCode();
	return 0;
}


/*
 * UART Setup
 */

void UARTSetup()                            //Code from Lab 0 example code
{
	DCOCTL = 0;                             // Select lowest DCOx and MODx settings
	BCSCTL1 = CALBC1_1MHZ;                  // Set DCO
	DCOCTL = CALDCO_1MHZ;
	P1SEL = RedLED + GreenLED;              // P1.1 = RXD, P1.2=TXD
	P1SEL2 = RedLED + GreenLED;             // P1.1 = RXD, P1.2=TXD
	UCA0CTL1 |= UCSSEL_2;                   // SMCLK
	UCA0BR0 = 104;                          // 1MHz 9600
	UCA0BR1 = 0;                            // 1MHz 9600
	UCA0MCTL = UCBRS0;                      // Modulation UCBRSx = 1
	UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
	IE2 |= UCA0RXIE;                        // Enable USCI_A0 RX interrupt
}


/*
 * Timer Setup
 */

void TimerSetup(int rate)                   // Subject to change
{
	CCTL0 = CCIE;
	TA0CTL = TASSEL_2 + MC_1 + ID_2;        // SMCLK/4, Up
	TA0CCR0 = 125000 / rate;                // 250000 / 10 = 25000, (10^6 [Hz] / 4) / (25000) = 10Hz
}


/*
 * UART Interrupt
 */

#pragma vector=USCI_A0_VECTOR               // Interrupt Vector definition
__interupt void USCI_A0_ISR(void)           // Interrupt function deceleration
{       
    switch(ByteCount)
    {
        case 0:                             // Calculate and send package size
            
            break;
        case 1:                             // Set Red LED
            
            break;
        case 2:                             // Set Green LED

            break;
        case 3;                             // Set Blue LED
            
            break;
        default:                            // Send the rest of the package
            
            break;
    }
}


/* LED Test Code

P1.0 Will SWITCH when you push the button, use this to test LEDs
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
*/
