#include <msp430.h>				

int global_val = 1;
/**
 * blink.c
 */
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer
	int a =2;
	int b = 2;

	b = b+a;
	return 0;
}
