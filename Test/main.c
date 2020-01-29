#include <msp430.h> 
#include <stdlib.h>
//#include <stdio.h>

/**
 * main.c
 */
void p1_a(void) {


    unsigned int inVal, kk=1, mm=32767;
    int loop = 0;
    while(kk > 0 ) {
        inVal = (mm+kk)/kk;
        mm/=4;
        kk = kk << 1;
        loop++;
    }

    printf("Value of kk after looping: %d\nValue of mm after looping: %d\nValue of inVald after looping: %d",kk,mm,inVal);

}
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	p1_a();
	return 0;
}
