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

/*
#define Btn BIT3                            // Define "Btn" as bit 3.
#define LED BIT0                            // Define "LED0" as bit 0.
#define PnB (P1IN & Btn)                    // Define "INP" for checking if there is an input on pin 1.3.
typedef int bool;
#define true 1
#define false 0
*/

/*
 * Code Definition
 */

#define RedLED      BIT3;
#define GreenLED    BIT4;
#define BlueLED     BIT5;

volatile int ByteCount = 0;

void UARTSetup();
void LEDSetup();
void TimerSetup(int rate);

int NumberOfBytes = 0;
int CurrentByte = 0;

/*
 * Main Function
 */

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;				// Stop watchdog timer
	UARTSetup();                           	// UARTSetup Function
    LEDSetup();                             // LEDSetup Function
	//TestCode();                           // TestCode Function

	__bis_SR_register(LPM0_bits + GIE); 	// Low-Power Mode 0 and Global Interrupt Enable

	return 0;
}


/*
 * Setting UART
 */

void UARTSetup()                            // Code from Lab 0 example code
{
	DCOCTL = 0;                           	// Select lowest DCOx and MODx settings
	BCSCTL1 = CALBC1_1MHZ;                  // Set DCO
	DCOCTL = CALDCO_1MHZ;
	P2SEL = RedLED + GreenLED;              // P2.3 = RXD, P2.4 = TXD
	P2SEL2 = RedLED + GreenLED;             // P2.3 = RXD, P2.4 = TXD
	UCA0CTL1 |= UCSSEL_2;                   // SMCLK
	UCA0BR0 = 104;                          // 1MHz 9600
	UCA0BR1 = 0;                            // 1MHz 9600
	UCA0MCTL = UCBRS0;                      // Modulation UCBRSx = 1
	UCA0CTL1 &= ~UCSWRST;                   // Initialize USCI state machine
	IE2 |= UCA0RXIE;                        // Enable USCI_A0 RX interrupt
}


/*
 * Setting LEDs
 */

void LEDSetup()
{
    P2DIR |= RedLED;                       	// P2.3 to output
    P2SEL |= RedLED;                        // P2.3 to TA0.1
    P2DIR |= GreenLED;                      // P2.4 to output
    P2SEL |= GreenLED;                      // P2.4 to TA0.2
    P2DIR |= BlueLED;                       // P2.5 to output
    P2SEL |= BlueLED;                       // P2.5 to TA0.3
}


/*
 * Setting Timer
 */

void TimerSetup(int rate)                  	// Subject to change
{
	CCTL0 = CCIE;
	TA0CTL = TASSEL_2 + MC_1 + ID_0;        // SMCLK divided by 1, Up
	// TA0CCR0 = 125000 / rate;             // 250000 / 10 = 25000, (10^6 [Hz] / 4) / (25000) = 10Hz
	TA0CCR0  = 0x00FF;                  	// Sets CCR0 to 255
	TA0CCTL1 = OUTMOD_7; 					// Reset or Set behavior
	TA0CCTL2 = OUTMOD_7; 					// Reset or Set behavior
	TA0CCTL3 = OUTMOD_7; 					// Reset or Set behavior
}


/*
 * Setting UART Interrupt
 */

//#pragma vector=USCI_A0_VECTOR           	// Interrupt Vector definition
//__interupt void USCI_A0_ISR(void)         // Interrupt function deceleration

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{       
    while (!(IFG2&UCA0TXIFG)){              // USCI_A0 TX buffer ready?

          switch(CurrentByte){
          case 0:
              NumberOfBytes = UCA0RXBUF;	// first byte received
              break;
          case 1:
              TA0CCR1 = 255 - UCA0RXBUF;	// red LED value
              break;
          case 2:
              TA1CCR1 = 255 -UCA0RXBUF;     // green LED value
              break;
          case 3:
              TA1CCR2 = 255 - UCA0RXBUF;    // blue LED value
              UCA0TXBUF = numOfBytes-3;     // send new numBytes
              break;
          default:
              if(CurrentByte<NumberOfBytes){
                  UCA0TXBUF = temp;         // send remaining bytes
              }
          }
          CurrentByte++;
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
