/*
 * main.c for Milestone
 * 
 *    Created on: October 10, 2018
 *  Last Edited: 
 *       Author: Tyler M. and Helen P.
 *        Board: MSP430G2553
 */


#include <msp430.h>


/*
 * Code Definitions
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
	WDTCTL = WDTPW | WDTHOLD;	            // stop watchdog timer
	UARTSetup();
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

void TimerSetup(int rate)                   //Subject to change
{
    CCTL0 = CCIE;
    TA0CTL = TASSEL_2 + MC_1 + ID_2;        // SMCLK/4, Up
    TA0CCR0 = 125000 / rate;                // 250000 / 10 = 25000, (10^6 [Hz] / 4) / (25000) = 10Hz
}


/*
 * UART Interrupt
 */

#pragma vector=USCI_A0_VECTOR               //Interrupt Vector definition
__interupt void USCI_A0_ISR(void)           //Interrupt function deceleration
{       
    switch(ByteCount)
    {
        case 0:                                 //calculate and send package size
            
            break;
        case 1:                                 //Set Red LED
            
            break;
        case 2:                                 //Set Green LED

            break;
        case 3;                                 //Set Blue LED
            
            break;
        default:                                //send the rest of the package
            
            break;
    }
}
