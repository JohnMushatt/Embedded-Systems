/*
 * HW2_P3.c
 *
 *  Created on: Feb 6, 2020
 *      Author: johnm
 */




void setupP4_in();
void setupP4_out();
void P4inOut();


void setupP4() {
    P4SEL &= ~(BIT0 | BIT2 | BIT4 |BIT6);

    P4DIR &= ~(BIT0 | BIT2 | BIT4 |BIT6);

}
void setupP4_out() {
    P4SEL &= ~(BIT1 | BIT3 | BIT5 |BIT7);

    P4DIR |=  (BIT1 | BIT3 | BIT5 |BIT7);

}

void P4inOut() {
    char input = P4IN &(BIT0| BIT2|BIT4|BIT6);


    char complement = (~input <<4);

    P6OUT = comp;
}
void main() {
    setupP4();
    setupP4_out();
    P4inOut();
    //P4IN = 0xF1 -> 0xE0
    //P4IN = 0x5C -> 0x30
}
