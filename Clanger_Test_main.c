#include <msp430.h> 

/*
 * main.c
 */
#define PERIOD (250000/4) // smclk/120 bpm /60 sec*2 ticks per beat
#define PULSE_DUR 250000/(1000/20) // 20ms pulse time
char cs_hat = BIT1; // closed hat out 2.1
char op_hat = BIT2;// open hat out 2.2
char cy = BIT5;//cymbal pin 1.4
int counter =0;
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    DCOCTL = CALDCO_16MHZ;
    	BCSCTL1 = CALBC1_16MHZ;
    	BCSCTL2 |= DIVS_3;						// SMCLK = MCLK / 8 = 2MHZ

    	WDTCTL = WDTPW + WDTHOLD;               // Stop WDT
    	_BIS_SR(GIE);							// enable interrupts
    	P1DIR |=  BIT0 + BIT1 + BIT2 + BIT5;           	//  output
    	P1OUT = 0;

    	TA1CCTL1 = OUTMOD_7;						// CCR1 reset/set
    	P2DIR |=  BIT1 + BIT2 + BIT5;
    	P2SEL=0;
    	P2OUT = 0;							 //output 2.2
    	P2REN = 0xFF;						///ren all pins on P2---new change!!!!
    	TA1CTL = TASSEL_2 + MC_1 + ID_3;         // SMCLK, up mode, /8 = 250KHz
    	TA1CCTL0 |=  CCIE;
    	TA1CCR0 = PERIOD; ///period should be SMCLK 500k /120bpm*2 ticks per beat
	return 0;
}
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0 (void){

 int i, dummy;
 char output;
	TA1CCR0 = PERIOD;
	if(counter == 6|| counter == 13)
		output = op_hat;
	else if(counter ==15)
		output = cy;
	else
		output = cs_hat;
	counter++;
	if(counter == 16)
		counter = 0;


	P1OUT |= BIT0 + output;
	//P2OUT |= output;
	for (i = PULSE_DUR; i>0; i--)
		;

	P1OUT &=~(BIT0 + output);
	//P2OUT &= ~output;
}
