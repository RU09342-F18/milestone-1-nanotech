/*
 * main.c for Milestone
 *
 *    Created on: October 10, 2018
 *   Last Edited: October 18, 2018
 *        Author: Tyler M. and Helen P.
 *         Board: MSP430G2553
 */

#include <msp430g2553.h>

/*
Test Code Definitions
*/
/*
#define Btn BIT3                                // Define "Btn" as bit 3.
#define LED BIT0                                // Define "LED0" as bit 0.
#define PnB (P1IN & Btn)                        // Define "INP" for checking if there is an input on pin 1.3.
typedef int bool;
#define true 1
#define false 0
*/


/*
 * Code Definition
 */
#define RedLED      BIT6;
#define GreenLED    BIT1;
#define BlueLED     BIT4;
// volatile int ByteCount = 0;
void UARTSetup();
void LEDSetup();

void TimerSetup();
char NumberOfBytes = 0;
char CurrentByte = 0;

/*
 * Main Function
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                   // Stop watchdog timer
    UARTSetup();                                // UARTSetup Function
    LEDSetup();                              	// LEDSetup Function
    TimerSetup();
    __bis_SR_register(GIE);
    while(1);
    return 0;
}

/*
 * Setting UART
 */
void UARTSetup()								// Code from Lab 0 example code
{
    DCOCTL = 0;                            		// Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;                   	// Set DCO
    DCOCTL = CALDCO_1MHZ;
    P1SEL = BIT1 + BIT2 ;                     	// P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 + BIT2 ;                    	// P1.1 = RXD, P1.2=TXD
    UCA0CTL1 |= UCSSEL_2;                     	// SMCLK
    UCA0BR0 = 104;                            	// 1MHz 9600
    UCA0BR1 = 0;                              	// 1MHz 9600
    UCA0MCTL = UCBRS0;                        	// Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;                     	// **Initialize USCI state machine**
    IE2 |= UCA0RXIE;                          	// Enable USCI_A0 RX interrupt
}

/*
 * Setting LEDs
 */
void LEDSetup()
{
    P1DIR |= RedLED;                            // P1.6 to output
    P1SEL |= RedLED;                            // P1.6 to TA0.1
    P1SEL2 |= ~RedLED;
    P2DIR |= GreenLED;                          // P2.1 to output
    P2SEL |= GreenLED;                          // P2.1 to TA0.2
    P2SEL2 &= ~GreenLED;
    P2SEL2 &= ~BlueLED;
    P2DIR |= BlueLED;                           // P2.4 to output
    P2SEL |= BlueLED;                           // P2.4 to TA0.3
}

/*
 * Setting Timer
 */
void TimerSetup()                       		// Subject to change
{
    //CCTL0 = CCIE;
    TA0CTL = TASSEL_2 + MC_1 + ID_0;            // SMCLK divided by 1, Up
    TA0CCR0  = 255;                         	// Sets CCR0 to 255
    TA0CCTL1 = OUTMOD_7;                        // Reset or Set behavior
    TA1CTL = TASSEL_2 + MC_1 + ID_0;            // SMCLK divided by 1, Up
    TA1CCR0  = 0x00FF;                          // Sets CCR0 to 255
    TA1CCTL1 = OUTMOD_7;                        // Reset or Set behavior
    TA1CCTL2 = OUTMOD_7;                        // Reset or Set behavior

    TA0CCR1 = 0x00FF;
    TA1CCR1 = 0x00FF;
    TA1CCR2 = 0x00FF;
}

/*
 * Setting UART Interrupt
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    while (!(IFG2&UCA0TXIFG));                	// USCI_A0 TX buffer ready?
    //UCA0TXBUF = UCA0RXBUF;                    // TX -> RXed character
        switch(CurrentByte){
            case 0:
                NumberOfBytes = UCA0RXBUF;      // first byte received
                //UCA0TXBUF = UCA0RXBUF;      	// Remove Later
                break;
            case 1:
                TA0CCR1 = 255 - UCA0RXBUF;      // red LED value
                CurrentByte++;
                break;
            case 2:
                TA1CCR1 = 255 - UCA0RXBUF;      // green LED value
                CurrentByte++;
                break;
            case 3:
                TA1CCR2 = 255 - UCA0RXBUF;      // blue LED value
                if(NumberOfBytes - 3 > 0){
                UCA0TXBUF = NumberOfBytes - 3;  // send new numBytes
                }
                CurrentByte++;
                if (NumberOfBytes == 3){
                    UCA0TXBUF = 0x00;
                    CurrentByte = 0;
                    NumberOfBytes = 0;
                }
                break;
            default:
                if(CurrentByte<=NumberOfBytes){
                    UCA0TXBUF = UCA0RXBUF;      // send remaining bytes
                }else{
                    CurrentByte = 0;
                }
            }
        CurrentByte++;
}

/* LED Test Code
P1.0 Will SWITCH when you push the button, use this to test LEDs
void TestCode()
{


    P1OUT ^= BIT0;                            // P1.0 = toggle
    //P1IFG &= ~BIT3; // P1.3 IFG cleared

}

